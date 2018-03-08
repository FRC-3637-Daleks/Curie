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

SimplePath::SimplePath(StartPositions_t start, TargetType_t target)
{
	state=AutonWaiting;
	switch(target) {
	case BaseLine :
		steps.reserve(1);
		//Baseline is 120 inches from driver station wall, so give 10 inch allowance to make sure we cross it
		//but have to subtract robot depth as only front of robot needs to cross
		AddStep(Step(DriveIt,125.0 - ROBOT_BASE_DEPTH));
		break;
	case LeftSwitch :
		switch(start) {
		case(Left) :
			steps.reserve(3);
			//Distance to travel = distance to fence + 1/2 depth of switch
			//distance = 140" + 1/2(56") = 168"
			AddStep(Step(DriveIt,168.0));
			AddStep(Step(TurnIt, 90.0));
			AddStep(Step(DriveItSlow,12.0));
			break;
		default:
			break;
		}
		break;
	case RightSwitch :
		switch(start) {
		case(Right) :
			steps.reserve(3);
			//Distance to travel = distance to fence + 1/2 depth of switch
			//distance = 140" + 1/2(56") = 168"
			AddStep(Step(DriveIt,168.0));
			AddStep(Step(TurnIt, 270.0));
			AddStep(Step(DriveItSlow,12.0));
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
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
