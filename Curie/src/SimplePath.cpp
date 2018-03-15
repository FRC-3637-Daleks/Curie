/*
 * SimplePath.cpp
 *
 *  Created on: Feb 8, 2018
 *      Author: FLL2
 */

#include "SimplePath.h"

SimplePath::SimplePath(size_t len)
{
	steps.reserve(len);
	state=AutonWaiting;
}

SimplePath::SimplePath(StartPositions_t startPos, TargetType_t target)
{
	state=AutonWaiting;
	switch(target) {
	case BaseLine :
		steps.reserve(1);
		//Have to subtract robot depth as only front of robot needs to cross baseline
		AddStep(Step(DriveIt,BASELINE_TARGET_DISTANCE - ROBOT_BASE_DEPTH));
		break;
	case LeftSwitch :
		switch(startPos) {
		case(Left) :
			CreateSameSideSwitchPath(startPos);
			break;
		case(Right) :
			//TODO: Need to develop path for traveling from left start position to the switch on the right
			steps.reserve(10);
			break;
		default:
			break;
		}
		break;
	case RightSwitch :
		switch(startPos) {
		case(Right) :
			CreateSameSideSwitchPath(startPos);
			break;
		case(Left) :
			//TODO: Need to develop path for traveling from right start position to the switch on the left
			steps.reserve(10);
			break;
		default:
			break;
		}
		break;
	case LeftScale :
		switch(startPos) {
		case(Left) :
			steps.reserve(10);
			break;
		}
	default:
		break;
	}
}

//This will create the path to the switch when the robot is starting on same side of field as assigned switch side
void
SimplePath::CreateSameSideSwitchPath(StartPositions_t startPos)
{
	steps.reserve(5);
	//Distance to travel = distance to fence + 1/2 depth of switch
	//distance = 140" + 1/2(56") = 168"
	AddStep(Step(DriveIt,80.0));
	if (startPos == Left) {
		AddStep(Step(TurnIt, 90.0));
	} else {
		AddStep(Step(TurnIt, 270.0));
	}
//	AddStep(Step(LiftIt,SWITCH_DELIVERY_HEIGHT));
//	AddStep(Step(DriveItSlow,12.0));
//	AddStep(Step(DeliverIt,DELIVERY_POWER));
}

//This will create the path to the switch when the robot is starting on opposite side of field as assigned switch side
void
SimplePath::CreateOppSideSwitchPath(StartPositions_t startPos)
{
	//TODO - Path needs to be developed, for now this is just crossing the baseline
	steps.reserve(10);
	AddStep(Step(DriveIt,BASELINE_TARGET_DISTANCE - ROBOT_BASE_DEPTH));
}

//This will create the path to the scale when the robot is starting on same side of field as assigned scale side
void
SimplePath::CreateSameSideScalePath(StartPositions_t startPos)
{
	//TODO - Path needs to be developed, for now this is just crossing the baseline
	steps.reserve(10);
	AddStep(Step(DriveIt,BASELINE_TARGET_DISTANCE - ROBOT_BASE_DEPTH));
}

//This will create the path to the scale when the robot is starting on opposite side of field as assigned scale side
void
SimplePath::CreateOppSideScalePath(StartPositions_t startPos)
{
	//TODO - Path needs to be developed, for now this is just crossing the baseline
	steps.reserve(10);
	AddStep(Step(DriveIt,BASELINE_TARGET_DISTANCE - ROBOT_BASE_DEPTH));
}
void
SimplePath::AddStep(Step newStep)
{
	steps.push_back(newStep);
}

//TODO need to add proximity sensor!

AutonState_t
SimplePath::RunPath(DalekDrive *d, AHRS *ahrs)
{

	frc::SmartDashboard::PutNumber("SimplePath: Heading",
			ahrs->GetCompassHeading());
	switch (state) {
	case AutonWaiting:
		//initialize stuff to start path execution
		currentStepNumber = 0;
		state = AutonExecuting;
		//set curr position/distance???
		distanceTraveled = 0.0;
		break;
	case AutonExecuting:
		//Need to pass distance
		frc::SmartDashboard::PutNumber("Current Step Executing:",
				currentStepNumber);
		state = steps.at(currentStepNumber).ExecuteStep(d, ahrs);
		if (state == AutonComplete) {
			currentStepNumber++;
			//Check if any more steps to execute
			if (currentStepNumber < (int)steps.size()) {
				distanceTraveled = 0.0;
				state = AutonExecuting;
			}
		}
		break;
	case AutonBlocked:
		//TODO Do we try an alternate path? or just stop?
		break;
	default:
		break;
	}
	return state;
}

SimplePath::~SimplePath()
{
	// TODO Auto-generated destructor stub
}
