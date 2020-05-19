#include "NestedNamespace.h"

void UNamespaceTree::Initialize(FName _spliter)
{
	spliter = _spliter;
}

void UNamespaceTree::MergeSequence(FString sequence)
{
	if (sequence.IsEmpty())
		return;
	FString left, right;
	TArray<FNameSpaceTreeNode> spaces;
	FNameSpaceTreeNode _space;
	while (sequence.Split(spliter.ToString(), &left, &right))
	{
		_space.name = FName(left);
		spaces.Add(_space);
		sequence = right;
	}
	_space.name = FName(sequence);
	spaces.Add(_space);
	roots.AddUnique(spaces[0]);
	// Root Hierarchy
	auto rtIdx = roots.Find(spaces[0]);
	FNameSpaceTreeNode* currentLayer = &roots[rtIdx];
	// Sons Hierarchy
	for (auto i = 1; i < spaces.Num(); i++)
	{
		currentLayer->sons.AddUnique(spaces[i]);
		rtIdx = currentLayer->sons.Find(spaces[i]);
		currentLayer = &currentLayer->sons[rtIdx];
	}
	currentLayer->bReached = true;
}


const TArray<FNameSpaceTreeNode> UNamespaceTree::GetRoots()
{
	return roots;
}

bool UNamespaceTree::Has(FString sequence)
{
	if (sequence.IsEmpty())
		return false;
	FString left, right;
	TArray<FNameSpaceTreeNode> spaces;
	FNameSpaceTreeNode _space;
	while (sequence.Split(spliter.ToString(), &left, &right))
	{
		_space.name = FName(left);
		spaces.Add(_space);
		sequence = right;
	}
	_space.name = FName(sequence);
	spaces.Add(_space);
	auto rtIdx = roots.Find(spaces[0]);
	if (rtIdx < 0)
		return false;
	else
	{
		FNameSpaceTreeNode* currentLayer = &roots[rtIdx];
		for (auto i = 1; i < spaces.Num(); i++)
		{
			rtIdx = currentLayer->sons.Find(spaces[i]);
			if (rtIdx < 0)
				return false;
			currentLayer = &currentLayer->sons[rtIdx];
		}
		return true & currentLayer->bReached;
	}
}
