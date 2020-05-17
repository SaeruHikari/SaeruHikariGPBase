// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <FSMInternal/FSMachine.h>
#include <Templates/SharedPointer.h>
#include <Templates/UniquePtr.h>
#include "SFSM.generated.h"

using GuidMachineState = FSMachineState<FGuid, FGuid, FGuid>;
struct DefaultStateMachine;

struct FDefaultStateNode : public GuidMachineState
{
	friend struct FDefaultStateNode;

	FDefaultStateNode(FGuid guid);
protected:
	void _AddPath(FDefaultStateNode* target)
	{
		return AddPath(target->GetNodeId(), target);
	}
};

struct DefaultStateMachine : public FSMachine<FGuid, FGuid, FGuid>
{
	DefaultStateMachine(TUniquePtr<FDefaultStateNode> starter)
		:FSMachine<FGuid, FGuid, FGuid>(std::move(starter))
	{

	}
};
/**
 *
 */
UCLASS(Blueprintable)
class SAERUHIKARIGPBASE_API USFSM : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
		void AddPath(FGuid src, FGuid target)
	{
		return machine.AddPath(src, target, target, target);
	}

	UFUNCTION(BlueprintCallable)
		void IslandState(FGuid target)
	{
		return machine.IslandState(target);
	}

	UFUNCTION(BlueprintCallable)
		int RemoveAllIslands()
	{
		return machine.RemoveAllIslands();
	}

	UFUNCTION(BlueprintCallable)
		void RemoveStateSafely(FGuid target)
	{
		return machine.RemoveStateSafely(target);
	}

	UFUNCTION(BlueprintPure)
		int GetCount()
	{
		return machine.GetStatesSize();
	}

	UFUNCTION(BlueprintPure)
		FGuid GetStarterNode()
	{
		return machine.GetStartState()->GetNodeId();
	}

	UFUNCTION(BlueprintPure)
		FGuid GetCurrentNode()
	{
		return machine.GetCurrentState()->GetNodeId();
	}
protected:
	DefaultStateMachine machine
		= DefaultStateMachine(MakeUnique<FDefaultStateNode>(FGuid::NewGuid()));
};
