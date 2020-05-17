#pragma once
#include "FSMachineState.h"

template<typename NodeId, typename OpCode, typename FSMData>
struct FSMachine
{
	using StateType = FSMachineState<NodeId, OpCode, FSMData>;

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

	bool Transition(OpCode operation, bool resetOnFail = true)
	{
		if (CurrentState.Get().Linkage(operation) != nullptr)
		{
			CurrentState = CurrentState.Get().Linkage(operation);
			return true;
		}
		if (resetOnFail)
			CurrentState = StartState;
		return false;
	}

	bool isFinished(void) const
	{
		return CurrentState.Get().isEnd();
	}

	void Reset(void)
	{
		CurrentState = StartState;
	}

	void AddPath(NodeId src, OpCode operation, NodeId id, FSMData data)
	{
		check(nodes.Find(src) != nullptr);
		StateType* srcState = nodes.Find(src)->Get();
		if (nodes.Find(id) != nullptr)
		{
			// if existed state with the id found, add linkage directly.
			StateType* state = nodes.Find(id)->Get();
			srcState->AddPath(operation, state);
		}
		else
		{
			// if no existed, construct a new one.
			auto newState = MakeUnique<StateType>(id, data);
			newState.Get()->machine = this;
			nodes.Add(newState.Get()->GetNodeId(), std::move(newState));
			srcState->AddPath(operation, nodes.Find(id)->Get());
		}
	}

	void AddPath(NodeId src, OpCode operation, NodeId id)
	{
		check(nodes.Find(src) != nullptr);
		check(nodes.Find(id) != nullptr);
		StateType* state = nodes.Find(id);
		srcState->AddPath(operation, state);
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
