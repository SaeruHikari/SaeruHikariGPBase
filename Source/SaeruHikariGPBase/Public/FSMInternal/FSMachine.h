#pragma once
#include "FSMachineState.h"

template<typename NodeId, typename OpCode, typename FSMData>
struct FSMachine
{
	using StateType = FSMachineState<NodeId, OpCode, FSMData>;
	inline FSMachine(const FSMachine& rhs) = default;
	inline FSMachine& operator=(const FSMachine& rhs) = default;
	FSMachine(TUniquePtr<StateType> startState)
	{
		auto id = startState.Get()->GetNodeId();
		nodes.Add(id, std::move(startState));
		StartState = nodes.Find(id)->Get();
		CurrentState = StartState;
		StartState->machine = this;
	}

	~FSMachine()
	{
		nodes.Empty();
	}

	bool hasNode(NodeId id)
	{
		return (nodes.Find(id) != nullptr);
	}

	bool Command(OpCode operation, bool resetOnFail = true)
	{
		if (CurrentState->Linkage(operation) != nullptr)
		{
			CurrentState = CurrentState->Linkage(operation);
			return true;
		}
		if (resetOnFail)
			CurrentState = StartState;
		return false;
	}

	bool isFinished(void) const
	{
		return CurrentState->isEnd();
	}

	void Reset(void)
	{
		CurrentState = StartState;
	}

	void Link(NodeId src, OpCode operation, NodeId id, FSMData data)
	{
		check(nodes.Find(src) != nullptr);
		StateType* srcState = nodes.Find(src)->Get();
		if (nodes.Find(id) != nullptr)
		{
			// if existed state with the id found, add linkage directly.
			StateType* state = nodes.Find(id)->Get();
			srcState->Link(operation, state);
		}
		else
		{
			// if no existed, construct a new one.
			auto newState = MakeUnique<StateType>(id, data);
			newState.Get()->machine = this;
			nodes.Add(newState.Get()->GetNodeId(), std::move(newState));
			srcState->Link(operation, nodes.Find(id)->Get());
		}
	}

	void Link(NodeId src, OpCode operation, NodeId id)
	{
		check(nodes.Find(src) != nullptr);
		check(nodes.Find(id) != nullptr);
		StateType* srcState = nodes.Find(src)->Get();
		StateType* state = nodes.Find(id)->Get();
		srcState->Link(operation, state);
	}

	size_t GetStatesSize(void) const
	{
		return nodes.Num();
	}

	StateType* GetStartState(void) const
	{
		return StartState;
	}

	StateType* GetCurrentState(void) const
	{
		return CurrentState;
	}

	StateType* GetState(NodeId id)
	{
		return nodes.Find(id)->Get();
	}

	void IslandState(NodeId id)
	{
		check(id != StartState->GetNodeId());
		if (CurrentState->GetNodeId() == id)
			CurrentState = StartState;
		nodes.Find(id)->Get()->Island();
	}

	void RemoveStateSafely(NodeId id)
	{
		IslandState(id);
		nodes.Remove(id);
	}

	int RemoveAllIslands(void)
	{
		int i = 0;
		TArray<StateType*> states;
		for (auto& state : nodes)
		{
			states.Add(state.Value.Get());
		}
		for (auto& ptr : states)
		{
			if (ptr->isIsland() && (ptr != StartState))
			{
				RemoveStateSafely(ptr->GetNodeId());
				i++;
			}
		}
		return i;
	}

protected:
	StateType* StartState;
	StateType* CurrentState;
	TMap<NodeId, TUniquePtr<StateType>> nodes;
};
