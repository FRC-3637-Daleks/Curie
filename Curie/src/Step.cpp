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
	command  = DoNothing;
	distance = 0.0;
	angle    = 0.0;
	position = 0.0;
	start    = 0.0;
}

Step::Step(Commands_t com, double howFar)
{
	distance = 0.0;
	position = 0.0;
	angle    = 0.0;
	start    = 0.0;
	switch(com) {
		case DriveIt:
			command = DriveIt;
			distance = howFar;
			break;
		case DriveItSlow:
			command = DriveItSlow;
			distance = howFar;
			break;
		case LiftIt:
			command = LiftIt;
			position = howFar;
			break;
		case TurnIt:
			command = TurnIt;
			angle = howFar;
			break;
		case LowerWrist:
			command = LowerWrist;
			position = howFar;
			break;
		case DeliverIt:
			command = DeliverIt;
			break;
		default:
			command = DoNothing;
			break;
	}
	return;
}

// TODO need to add proximity sensor and check it!
//     for  traveling, how are we determining distance traveled????  IMU, encoders???
//     do we need the motors passed so can do like we did in Brahe?
//       or use DalekDrive->GetPosition? but even that needs one of the motors
AutonState_t
Step::ExecuteStep(DalekDrive *d, AHRS *ahrs,  Intake *i, Lifter *l)
{
	AutonState_t state = AutonExecuting;
	double distanceTraveled = d->GetDistance() - start;
	float currAngle;
	float diff;
	double motorPower = 0.5;

	switch (command) {
		case DriveItSlow:
			motorPower = 0.4;
			/* no break */
		case DriveIt:
			// check if we've reached target distance for this step
			if (distanceTraveled > distance) {
				state = AutonComplete;
				d->DisableTurnControl();
				d->TankDrive(0.0, 0.0);
				break;
			}
			d->DriveStraight(motorPower);
			// d->TankDrive(-1 * motorPower, -1 * motorPower);
			break;
		//New Version of TurnIt-> does not work
		/**case TurnIt:
			motorPower = 0.3;
			currAngle = fmod(ahrs->GetFusedHeading(), 360.0f);
			// diff = angle - currAngle;
			// We are within tolerance to turn is considered complete
			if (fabs(angle - currAngle) < ANGLE_DIFF_LIMIT) {
				state = AutonComplete;
				d->TankDrive(0.0, 0.0);
				break;
			}
			d->TurnToHeading(motorPower, angle);
			if(diff > 180.0) {
				diff -= 180.0;
				if(diff > 0)
					diff *= -1.0;
			}
			if (diff > 0.0f)
				d->TankDrive(-1 * motorPower, motorPower, false);
			else
				d->TankDrive(motorPower, -1 * motorPower, false);*/
		//This version of TurnIt has been restored from March 19th code base.
		case TurnIt:
				motorPower = 0.50;
				motorPower = 0.475;
		 		//TODO what kind of measurement does this give? 0-360? what if turn more than 360? did we start at 0?
				frc::SmartDashboard::PutNumber("Driveit: Heading",
						ahrs->GetFusedHeading());
		 		currAngle = fmod(ahrs->GetFusedHeading(), 360.0);
		 		diff = angle - currAngle;
		 		// We are within tolerance to turn is considered complete
		 		if (fabs(diff) < ANGLE_DIFF_LIMIT) {
		 			state = AutonComplete;
		 		 			d->TankDrive(0.0, 0.0);
		 		 		} else {
//		 		 			if (fabs(diff) > 180) {
//		 		 				newdiff = fabs(diff) - 180;
//		 		 				if (diff > 0) {
//		 		 					diff = newdiff * -1;
//		 		 				}
//		 		 			frc::SmartDashboard::PutNumber("Driveit: Diff",
//		 		 					diff);
//		 		 			}
//		 		 			if (diff > 0) {
		 		 			if (d->isTurnCW(angle, currAngle)) {
		 		 			//TODO is this the right power?  do we need to check for obstacle while turning?
		 		 			//     Decrease power if diff is close to 0???
		 		 				d->TankDrive(-1 * motorPower, motorPower);
		 		 			} else {
		 		 				d->TankDrive(motorPower, -1 * motorPower);
		 		 			}
		 		 		}
		 	break;

		case LiftIt:
			d->TankDrive(0.0, 0.0);

			if ((fabs(i->WristPosition() - WRIST_POSITION) < 100) && ((WRIST_POSITION - l->GetPosition()) < 500)) {
				state = AutonComplete;
				break;
			}
			if (fabs(i->WristPosition() - WRIST_POSITION) < 100) {
				i->StopWrist();
			}
			else {
	//				if(i->WristUpperLimit() == true)
						l->AutonUp(position);
					i->Lower();
			}
			;
			break;

		case DeliverIt:
			d->TankDrive(0.0, 0.0);
			i->Push();
			Wait(1.0);
			i->StopRoller();
			state = AutonComplete;
			break;
		case LowerWrist:
			d->TankDrive(0.0, 0.0);
			if (fabs(i->WristPosition() - position) < 100) {
				state = AutonComplete;
				i->StopWrist();
				break;
			}
			i->Lower();
			break;
		case DoNothing:
		default:
			d->TankDrive(0.0, 0.0);
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
