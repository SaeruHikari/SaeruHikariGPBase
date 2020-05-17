// Fill out your copyright notice in the Description page of Project Settings.


#include "SFSM.h"

FDefaultStateNode::FDefaultStateNode(FGuid guid)
	:FSMachineState<FGuid, FGuid, FGuid>(guid, guid)
{

}
