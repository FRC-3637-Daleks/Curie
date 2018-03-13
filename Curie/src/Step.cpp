/*
 * Step.cpp
 *
 *  Created on: Feb 8, 2018
 *      Author: FLL2
 */
#include "WPILib.h"
#include "SimplePath.h"
#include "Step.h"

Step::Step()
{
	command = DoNothing;
	distance = 0.0;
	angle = 0.0;
}

Step::Step(Commands_t com, double howFar)
{
	command = com;
	if (command == DriveIt) {
		distance = howFar;
	} else {
		angle = howFar;
	}
}

//TODO need to add proximity sensor and check it!
//     for  traveling, how are we determining distance traveled????  IMU, encoders???
//     do we need the motors passed so can do like we did in Brahe?
//       or use DalekDrive->GetPosition? but even that needs one of the motors
AutonState_t
Step::ExecuteStep(DalekDrive *d, AHRS *ahrs)
{
	AutonState_t state = AutonExecuting;
	//TODO: how to figure out how far we've traveled?
	// What kind of measurement is this giving us?
	double distanceTraveled = d->GetDistance();
	double currAngle, diff, newdiff = 0.0;
	double motorPower = 0.5;

	switch (command) {
	case DriveItSlow:
		motorPower = 0.25;
	case DriveIt:
		frc::SmartDashboard::PutNumber("Driveit: dist trav",
						distanceTraveled);
		frc::SmartDashboard::PutNumber("Driveit: dist targ",
						distance);
		//check if we've reached target distance for this step
		if (distanceTraveled < distance) {
			//if (sensor indicates obstacle) {
			// state = AutonBlocked;
			//else
			if (distance - distanceTraveled > 12.0) {
				motorPower = 0.5;
			}
			d->TankDrive(-1 * motorPower, -1 * motorPower);
		} else {
			state = AutonComplete;
			d->TankDrive(0.0, 0.0);
		}
		break;
	case TurnIt:
		motorPower = 0.25;
		//TODO what kind of measurement does this give? 0-360? what if turn more than 360? did we start at 0?
		frc::SmartDashboard::PutNumber("Driveit: Heading",
				ahrs->GetCompassHeading());
		currAngle = fmod(ahrs->GetCompassHeading(), 360.0);
		diff = angle - currAngle;
		// We are within tolerance to turn is considered complete
		if (fabs(diff) < angleDiffLimit) {
			state = AutonComplete;
		} else {
			if (fabs(diff) > 180) {
				newdiff = fabs(diff) - 180;
				if (diff > 0) {
					diff = newdiff * -1;
				}
			}
			if (diff > 0) {
			//TODO is this the right power?  do we need to check for obstacle while turning?
			//     Decrease power if diff is close to 0???
				d->TankDrive(-1 * motorPower, motorPower);
			} else {
				d->TankDrive(motorPower, -1 * motorPower);
			}
		}
		break;
	case LiftIt:
		break;
	case DeliverIt:
		break;
	case DoNothing:
		break;
	default:
		break;
	}
	return state;
}

bool
Step::Travel(double dist)
{
	bool success = true;
	return success;
}

bool
Step::Turn (double angle)
{
	bool success = true;
	return success;
}

Step::~Step()
{
	// TODO Auto-generated destructor stub
}
