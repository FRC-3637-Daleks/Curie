/*
 * SimplePath.cpp
 *
 *  Created on: Feb 8, 2018
 *      Author: FLL2
 */

#include "SimplePath.h"

SimplePath::SimplePath(size_t len) {
	steps.reserve(len);
	state=AutonWaiting;
}

void
SimplePath::AddStep(Step newStep)
{
	steps.push_back(newStep);
}

//TODO need to add proximity sensor!

bool
SimplePath::RunPath(DalekDrive *d, IMU *imu)
{
	bool complete = false;
	return complete;

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
		state = steps.at(currentStepNumber).ExecuteStep(d, imu);
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
}

SimplePath::~SimplePath() {
	// TODO Auto-generated destructor stub
}

