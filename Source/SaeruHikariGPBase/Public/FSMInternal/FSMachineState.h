#pragma once

template<typename NodeId, typename OpCode, typename FSMData>
struct FSMachine;

template<typename NodeId, typename OpCode, typename FSMData>
struct FSMachineState
{
	friend struct FSMachine<NodeId, OpCode, FSMData>;

	FSMachineState(NodeId _id, FSMData _data)
		:id(_id), data(_data)
	{

	}

	bool isEnd() const
	{
		return (paths.Num() == 0);
	}

	// Check linkage with opCode.
	auto Linkage(OpCode code)
	{
		return paths.Find(code);
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
	void AddPath(OpCode code, FSMachineState* target)
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
