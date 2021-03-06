/*
 * SimplePath.cpp
 *
 *  Created on: Feb 8, 2018
 *      Author: FLL2
 */

#include <SimplePath.h>
#include <WPILib.h>
#include <AHRS.h>

SimplePath::SimplePath(size_t len)
{
	steps.reserve(len);
	state = AutonWaiting;
}

SimplePath::SimplePath(StartPositions_t startPos, TargetType_t target)
{
	state = AutonWaiting;
	switch(target) {
		case BaseLine:
			CreateBaselinePath(startPos);
			break;
		case LeftSwitch:
			switch(startPos) {
				case Left:
					CreateSameSideSwitchPath(startPos);
					break;
				case Right:
					steps.reserve(1);
					AddStep(Step(DriveIt, BASELINE_TARGET_DISTANCE - ROBOT_BASE_DEPTH));
					//CreateOppSideSwitchPath(startPos);
					break;
				case Center:
					CreateCenterSwitchPath(Left);
					break;
				default:
					break;
			}
			break;
		case RightSwitch:
			switch(startPos) {
				case Right:
					CreateSameSideSwitchPath(startPos);
					break;
				case Left:
					//CreateOppSideSwitchPath(startPos);
					steps.reserve(1);
					AddStep(Step(DriveIt, BASELINE_TARGET_DISTANCE - ROBOT_BASE_DEPTH));
					break;
				case Center:
					CreateCenterSwitchPath(Right);
					break;
				default:
					break;
			}
			break;
		case LeftScale:
			switch(startPos) {
				case Left:
					CreateSameSideScalePath(startPos);
					break;
				case Right:
					//CreateOppSideScalePath(startPos);
					steps.reserve(1);
					AddStep(Step(DriveIt, BASELINE_TARGET_DISTANCE - ROBOT_BASE_DEPTH));
					break;
				default:
					break;
			}
			break;
		case RightScale:
			switch(startPos) {
				case Right:
					CreateSameSideScalePath(startPos);
					break;
				case Left:
					//CreateOppSideScalePath(startPos);
					steps.reserve(1);
					AddStep(Step(DriveIt, BASELINE_TARGET_DISTANCE - ROBOT_BASE_DEPTH));
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
}

double
SimplePath::DetermineAngle(StartPositions_t p, double left, double right)
{
	return (p == Left) ? left : right;
}

void
SimplePath::CreateBaselinePath(StartPositions_t startPos)
{
	steps.reserve(1);
	// Have to subtract robot depth as only front of robot needs to cross baseline
	AddStep(Step(DriveIt, BASELINE_TARGET_DISTANCE - ROBOT_BASE_DEPTH));
}

// This will create the path to the switch when the robot is starting on same side of field as assigned switch side
void
SimplePath::CreateSameSideSwitchPath(StartPositions_t startPos)
{
	steps.reserve(6);
	// Distance to travel = distance to fence + 1/2 depth of switch
	// distance = 140" + 1/2(56") = 168"

	AddStep(Step(DriveIt, 115.0));
	AddStep(Step(LiftLowerIt, SWITCH_DELIVERY_HEIGHT));
	AddStep(Step(TurnIt, DetermineAngle(startPos, 90.0, 270.0)));
	AddStep(Step(DriveItSlow, 15.0));
	AddStep(Step(DeliverIt, DELIVERY_POWER));
}

// This will create the path to the switch when the robot is starting on opposite side of field as assigned switch side
void
SimplePath::CreateOppSideSwitchPath(StartPositions_t startPos)
{
	// Path is through area between starting positions and switch, assuming robot starts at center of player station.
	// End position is perpendicular to switch.
	// Distance from start to switch not accounting for robot base is 140 in.
	//	steps.reserve(1);
	// Have to subtract robot depth as only front of robot needs to cross baseline
	//	AddStep(Step(DriveIt,
	//			BASELINE_TARGET_DISTANCE - ROBOT_BASE_DEPTH));


	steps.reserve(8);

	AddStep(Step(DriveIt, 190.0));
	AddStep(Step(TurnIt, DetermineAngle(startPos, 90.0, 270.0)));
	// Range goes from 98.5 in. to 170.5 in. depending on how far you start from the side wall.
	AddStep(Step(DriveIt, 150.0));
	AddStep(Step(TurnIt, 180.0));
	AddStep(Step(DriveIt, 26.0));
	AddStep(Step(LiftLowerIt, SWITCH_DELIVERY_HEIGHT));
	AddStep(Step(TurnIt, DetermineAngle(startPos, 270.0, 90.0)));
	AddStep(Step(DriveItSlow, 12.0));
	AddStep(Step(DeliverIt, DELIVERY_POWER));

}

// This will create the path to the scale when the robot is starting on same side of field as assigned scale side
void
SimplePath::CreateSameSideScalePath(StartPositions_t startPos)
{
	// Path is straight until reaches scale, turns, moves forward, places cube.
	// Assumes robot is placed at center.
	// Halfway mark is 324 in., not accounting for robot base.

	steps.reserve(6);

/*
	AddStep(Step(DriveIt, 300.0 - ROBOT_BASE_DEPTH));
	AddStep(Step(LiftLowerIt, SCALE_DELIVERY_HEIGHT));
	AddStep(Step(TurnIt, DetermineAngle(startPos, 90.0, 270.0)));
	AddStep(Step(DriveItSlow, 10.0));
	AddStep(Step(DeliverIt, DELIVERY_POWER));
*/



	AddStep(Step(DriveIt, 280.0 - ROBOT_BASE_DEPTH));
	AddStep(Step(LiftLowerIt, SCALE_DELIVERY_HEIGHT));
	AddStep(Step(TurnIt, DetermineAngle(startPos, 45.0, 315.0)));
	Wait(0.5);
	AddStep(Step(DriveItSlow, 5.0));
	AddStep(Step(DeliverIt, DELIVERY_POWER));


}

//This will create the path to the scale when the robot is starting on opposite side of field as assigned scale side
void
SimplePath::CreateOppSideScalePath(StartPositions_t startPos)
{
	// Path goes between switch and scale, drives to other end,
	// turns appropriately, and drops off directly facing the end
	// of the scale.
	steps.reserve(9);
	AddStep(Step(DriveIt, 190.0));
	AddStep(Step(TurnIt, DetermineAngle(startPos, 90.0, 270.0)));
	// Range goes from 216 in. to 288 in. depending on how far you start from the side wall.
	// Expected position should be halfway between scale end and side wall.
	AddStep(Step(DriveIt, 180.0));
	AddStep(Step(TurnIt, 0.0));
	AddStep(Step(LiftLowerIt, SCALE_DELIVERY_HEIGHT));
	AddStep(Step(DriveIt, 35.0));
	AddStep(Step(DeliverIt, 0.3));

}

// This will create the path to the right switch when the robot is starting in center
void
SimplePath::CreateCenterSwitchPath(StartPositions_t target)
{
/*
	steps.reserve(8);
	AddStep(Step(DriveIt, 36.0));
	AddStep(Step(TurnIt, DetermineAngle(target, 270.0, 90.0)));
	//Originally was DriveIt, 50.0, changed to 48.0 stay closer to center.
	AddStep(Step(DriveIt, 48.0));
	AddStep(Step(TurnIt, DetermineAngle(target, 345.0, 15.0)));
	AddStep(Step(TurnIt,DetermineAngle(target, 345.0, 15.0)));
	AddStep(Step(LiftLowerIt, SWITCH_DELIVERY_HEIGHT));
	AddStep(Step(DriveItSlow, 48.0));
	AddStep(Step(DeliverIt, DELIVERY_POWER));
*/


	steps.reserve(7);
	AddStep(Step(LiftLowerIt, SWITCH_DELIVERY_HEIGHT));
	AddStep(Step(DriveIt, 15.0));
	AddStep(Step(TurnIt, DetermineAngle(target, 325.0, 35.0)));
	AddStep(Step(DriveIt, 78.0));
	AddStep(Step(TurnIt, DetermineAngle(target, 0.0, 0.0)));
	AddStep(Step(DeliverIt, DELIVERY_POWER));

}

void
SimplePath::AddStep(Step newStep)
{
	steps.push_back(newStep);
}

AutonState_t
SimplePath::RunPath(DalekDrive *d, AHRS *ahrs, Intake *i, Lifter *l,
		Ultrasonic *ul, Ultrasonic *ur)
{
	frc::SmartDashboard::PutNumber("Step", currentStepNumber);
	frc::SmartDashboard::PutNumber("SimplePath: Heading",
			ahrs->GetCompassHeading());
	switch (state) {
		case AutonWaiting:
			// initialize stuff to start path execution
			currentStepNumber = 0;
			state = AutonExecuting;
			// set current position/distance???
			distanceTraveled = 0.0;
			break;
		case AutonExecuting:
			// Need to pass distance
			frc::SmartDashboard::PutNumber("Current Step Executing:",
					currentStepNumber);
			state = steps.at(currentStepNumber).ExecuteStep(d, ahrs, i, l);
			if (state == AutonComplete) {
				currentStepNumber++;
				// Check if any more steps to execute
				if (currentStepNumber < (int)steps.size()) {
					steps.at(currentStepNumber).start = d->GetDistance();
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
}
