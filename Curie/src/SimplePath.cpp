/*
 * SimplePath.cpp
 *
 *  Created on: Feb 8, 2018
 *      Author: FLL2
 */

#include <SimplePath.h>
#include <WPILib.h>

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
			steps.reserve(1);
			// Have to subtract robot depth as only front of robot needs to cross baseline
			AddStep(Step(DriveIt,
					BASELINE_TARGET_DISTANCE - ROBOT_BASE_DEPTH));
			break;
		case LeftSwitch:
			switch(startPos) {
				case Left:
					CreateSameSideSwitchPath(startPos);
					break;
				case Right:
					// TODO: Need to develop path for traveling from left start position to the switch on the right
					CreateOppSideSwitchPath(startPos);
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
					// TODO: Need to develop path for traveling from right start position to the switch on the left
					CreateOppSideSwitchPath(startPos);
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
					CreateOppSideScalePath(startPos);
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
					CreateOppSideScalePath(startPos);
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

// This will create the path to the switch when the robot is starting on same side of field as assigned switch side
void
SimplePath::CreateSameSideSwitchPath(StartPositions_t startPos)
{
	steps.reserve(6);
	// Distance to travel = distance to fence + 1/2 depth of switch
	// distance = 140" + 1/2(56") = 168"

	AddStep(Step(DriveIt, 140.0));
    //AddStep(Step(LowerWrist, 1000));
	AddStep(Step(LiftIt, SWITCH_DELIVERY_HEIGHT));
	AddStep(Step(TurnIt, DetermineAngle(startPos, 90.0, 270.0)));
	AddStep(Step(DriveItSlow, 20.0));
	AddStep(Step(DeliverIt, DELIVERY_POWER));
}

// This will create the path to the switch when the robot is starting on opposite side of field as assigned switch side
void
SimplePath::CreateOppSideSwitchPath(StartPositions_t startPos)
{
	// Path is through area between starting positions and switch, assuming robot starts at center of player station.
	// End position is perpendicular to switch.
	// Distance from start to switch not accounting for robot base is 140 in.
	steps.reserve(1);
	// Have to subtract robot depth as only front of robot needs to cross baseline
	AddStep(Step(DriveIt,
			BASELINE_TARGET_DISTANCE - ROBOT_BASE_DEPTH));

//	steps.reserve(9);
//
//	AddStep(Step(DriveIt, 70.0));
//	AddStep(Step(TurnIt, DetermineAngle(startPos, 90.0, 270.0)));
//	// Range goes from 98.5 in. to 170.5 in. depending on how far you start from the side wall.
//	AddStep(Step(DriveIt, 134.5));
//	AddStep(Step(TurnIt, DetermineAngle(startPos, 270.0, 90.0)));
//	AddStep(Step(DriveIt, 26.0));
//	AddStep(Step(LowerWrist, 1000));
//	AddStep(Step(LiftIt, SWITCH_DELIVERY_HEIGHT));
//	AddStep(Step(DriveItSlow, 12.0));
//	AddStep(Step(DeliverIt, DELIVERY_POWER));

}

// This will create the path to the scale when the robot is starting on same side of field as assigned scale side
void
SimplePath::CreateSameSideScalePath(StartPositions_t startPos)
{
	// Path is straight until reaches scale, turns, moves forward, places cube.
	// Assumes robot is placed at center.
	// Halfway mark is 324 in., not accounting for robot base.

	steps.reserve(6);
	AddStep(Step(DriveIt, 324.0));
	AddStep(Step(TurnIt, DetermineAngle(startPos, 90.0, 270.0)));
	AddStep(Step(LowerWrist, 1000));
	AddStep(Step(LiftIt, SCALE_DELIVERY_HEIGHT));
	AddStep(Step(DriveItSlow, 30.0));
	AddStep(Step(DeliverIt, DELIVERY_POWER));

}

//This will create the path to the scale when the robot is starting on opposite side of field as assigned scale side
void
SimplePath::CreateOppSideScalePath(StartPositions_t startPos)
{
	// Path goes between switch and scale, drives to other end, turns appropriately, and drops off directly facing the end of the scale.
	steps.reserve(9);

	AddStep(Step(DriveIt, 248.0));
	AddStep(Step(TurnIt, DetermineAngle(startPos, 90.0, 270.0)));
	// Range goes from 216 in. to 288 in. depending on how far you start from the side wall.
	// Expected position should be halfway between scale end and side wall.
	AddStep(Step(DriveIt, 252));
	AddStep(Step(TurnIt, DetermineAngle(startPos, 270.0, 90.0)));
	AddStep(Step(DriveIt, 76.0));
	AddStep(Step(TurnIt, DetermineAngle(startPos, 270.0, 90.0)));
	AddStep(Step(LowerWrist, 1000));
	AddStep(Step(LiftIt, SWITCH_DELIVERY_HEIGHT));
	AddStep(Step(DriveItSlow, 30.0));
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
	frc::SmartDashboard::PutNumber("Step",
			currentStepNumber);
	frc::SmartDashboard::PutNumber("SimplePath: Heading",
			ahrs->GetCompassHeading());
	switch (state) {
		case AutonWaiting:
			// initialize stuff to start path execution
			currentStepNumber = 0;
			state = AutonExecuting;
			// set curr position/distance???
			distanceTraveled = 0.0;
			break;
		case AutonExecuting:
			// Need to pass distanc
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
