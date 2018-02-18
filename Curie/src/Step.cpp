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
	angle = 0.0;
}

Step::Step(Commands_t com, double howFar) {
	command = com;
	if (command == DriveIt) {
		distance = howFar;
	} else {
		angle = howFar;
	}
}

//TODO need to add proximity sensor and check it!
//     need "distance" passed in/updated
//     for turning, query IMU to get angle and see if turn complete
//     for  traveling, how are we determining distance traveled????  IMU, encoders???
AutonState_t
Step::ExecuteStep(DalekDrive *d, IMU *imu) {
	AutonState_t state = AutonExecuting;
	//TODO: how to figure out how far we've traveled?
	double distanceTraveled = 0.0;
	double currAngle, diff = 0.0;
	double motorPower = 0.5;
	switch (command) {
	case DriveItSlow:
		motorPower = 0.25;
	case DriveIt:
		//TODO: how to figure out how far we've traveled?
		// distanceTraveled = ??;
		//check if we've reached target distance for this step
		if (distanceTraveled < distance) {
			//if (sensor indicates obstacle) {
			// state = AutonBlocked;
			//else
			if (distance - distanceTraveled < 12.0) {

			}
			  d->SetLeftRightMotorOutputs(motorPower, motorPower);
		} else {
			state = AutonComplete;
		}
		break;
	case TurnIt:
		motorPower = 0.25;
		//TODO what kind of measurement does this give? 0-360? what if turn more than 360? did we start at 0?
		currAngle = imu->GetYaw();
		diff = angle - currAngle;
		if (fabs(diff) < angleDiffLimit) {
			state = AutonComplete;
		} else if (fabs(diff) < 180) {
			//TODO is this the right power? the right way to turn? do we need to check for obstacle while turning?
			d->SetLeftRightMotorOutputs(motorPower, -motorPower);
		} else {
			d->SetLeftRightMotorOutputs(-motorPower, motorPower);
		}
		break;
	case DoNothing:
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

