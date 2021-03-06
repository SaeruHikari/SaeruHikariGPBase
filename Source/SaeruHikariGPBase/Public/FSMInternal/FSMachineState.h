﻿#pragma once

template<typename NodeId, typename OpCode, typename FSMData>
struct FSMachine;

template<typename NodeId, typename OpCode, typename FSMData>
struct FSMachineState
{
	friend struct FSMachine<NodeId, OpCode, FSMData>;

	FSMachineState(NodeId _id, FSMData _data)
		:data(_data), id(_id)
	{

	}

	bool isEnd() const
	{
		return (paths.Num() == 0);
	}

	template<typename Func>
	void ForEachPath(Func functor)
	{
		functor(paths);
	}

	// Check linkage with opCode.
	FSMachineState* Linkage(OpCode code)
	{
		auto res = paths.Find(code);
		if (res == nullptr)
			return nullptr;
		return *res;
	}

	void RemovePath(OpCode code)
	{
		(*paths.Find(code))->prevPaths.Remove(code);
		paths.Remove(code);
	}

	NodeId GetNodeId(void) const
	{
		return id;
	}

	// Add path to a state which is not signed into the machine
	// would cause problems.
	void Link(OpCode code, FSMachineState* target)
	{
		check(machine->hasNode(target->GetNodeId()));
		paths.Add(code, target);
		target->prevPaths.Add(code, this);
	}

	// island this node
	void Island(void)
	{
		for (auto link : paths)
		{
			(*paths.Find(link.Key))->prevPaths.Remove(link.Key);
		}
		paths.Empty();
		for (auto prevLink : prevPaths)
		{
			prevLink.Value->paths.Remove(prevLink.Key);
		}
		prevPaths.Empty();
	}

	bool isIsland(void)
	{
		return (paths.Num() == 0) & (prevPaths.Num() == 0);
	}

	FSMData data;
protected:
	NodeId id;
	TMap<OpCode, FSMachineState*> paths;
	TMap<OpCode, FSMachineState*> prevPaths;
	FSMachine<NodeId, OpCode, FSMData>* machine = nullptr;
};
