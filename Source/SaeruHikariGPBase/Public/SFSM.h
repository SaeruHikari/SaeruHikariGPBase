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
	UFUNCTION(BlueprintCallable, Category = "SaeruHikari | FSM")
		void AddPath(FGuid src, FGuid target)
	{
		return machine.Link(src, target, target, target);
	}

	UFUNCTION(BlueprintCallable, Category = "SaeruHikari | FSM")
		void IslandState(FGuid target)
	{
		return machine.IslandState(target);
	}

	UFUNCTION(BlueprintCallable, Category = "SaeruHikari | FSM")
		int RemoveAllIslands()
	{
		return machine.RemoveAllIslands();
	}

	UFUNCTION(BlueprintPure, Category = "SaeruHikari | FSM")
		bool isFinished()
	{
		return machine.isFinished();
	}

	UFUNCTION(BlueprintCallable, Category = "SaeruHikari | FSM")
		bool Command(FGuid command)
	{
		return machine.Command(command);
	}

	UFUNCTION(BlueprintCallable, Category = "SaeruHikari | FSM")
		void RemoveStateSafely(FGuid target)
	{
		return machine.RemoveStateSafely(target);
	}

	UFUNCTION(BlueprintPure, Category = "SaeruHikari | FSM")
		int GetCount()
	{
		return machine.GetStatesSize();
	}

	UFUNCTION(BlueprintPure, Category = "SaeruHikari | FSM")
		FGuid GetStarterNode()
	{
		return machine.GetStartState()->GetNodeId();
	}

	UFUNCTION(BlueprintPure, Category = "SaeruHikari | FSM")
		FGuid GetCurrentNode()
	{
		return machine.GetCurrentState()->GetNodeId();
	}
protected:
	DefaultStateMachine machine{ MakeUnique<FDefaultStateNode>(FGuid::NewGuid()) };
};
