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
		case LiftLowerIt:
			command = LiftLowerIt;
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
			d->DriveStraight(motorPower, distance+start);
			// d->TankDrive(-1 * motorPower, -1 * motorPower);
			break;
		case TurnIt:
			motorPower = 0.3;
			currAngle = fmod(ahrs->GetFusedHeading(), 360.0f);
			// We are within tolerance to turn is considered complete
			if (fabs(angle - currAngle) < ANGLE_DIFF_LIMIT) {
				state = AutonComplete;
				d->TankDrive(0.0, 0.0);
				break;
			}
			d->TurnToHeading(motorPower, angle);
		 	break;
		case LiftIt:
			d->TankDrive(0.0, 0.0);
			if (fabs(l->GetPosition() - position) < 100) {
				state = AutonComplete;
				break;
			}
			l->AutonUp(position);
			break;
		case LiftLowerIt:
			d->TankDrive(0.0, 0.0);
			if ((position - l->GetPosition()) < 100) {
				i->StopWrist();
				state = AutonComplete;
				break;
			}
			i->Lower();
			l->AutonUp(position);
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
