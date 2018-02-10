/*
 * Step.cpp
 *
 *  Created on: Feb 8, 2018
 *      Author: FLL2
 */

#include "Step.h"

Step::Step() {
	command = DoNothing;
	distance = 0.0;
}

Step::Step(Commands_t com, double dist) {
	command = com;
	distance = dist;
}

//TODO need to add proximity sensor and check it!
//     need "distance" passed in/updated
//     for turning, query IMU to get angle and see if turn complete
//     for  traveling, how are we determining distance traveled????  IMU, encoders???
AutonState_t
Step::ExecuteStep(DalekDrive *d, IMU *imu) {
	AutonState_t state = AutonExecuting;
	switch (command) {
	case DriveIt:
		//check if we've reached target distance for this step
		//if yes, return AutonComplete
		//else check for obstacle and return AutonBlocked if so
		//else continue moving
		break;
	case TurnIt:
		//do some stuff
		break;
	default:
		break;
	}
	return state;
}

bool
Step::Travel(double dist) {
	bool success = true;
	return success;
}

bool
Step::Turn (double angle){
	bool success = true;
	return success;
}

Step::~Step() {
	// TODO Auto-generated destructor stub
}

