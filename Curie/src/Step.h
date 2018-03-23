/*
 * Step.h
 *
 *  Created on: Feb 8, 2018
 *      Author: FLL2
 */

#ifndef STEP_H_
#define STEP_H_

#include <AHRS.h>
#include <DalekDrive.h>
#include <math.h>
#include <Intake.h>
#include <Lifter.h>

enum AutonState {
	/* Auton States
	 */
	AutonWaiting = 0,
	AutonExecuting,
	AutonComplete,
	AutonBlocked,
};

typedef enum AutonState AutonState_t;

enum Commands {
	/* Auton Commands
	 */
	DoNothing = 0,
	DriveIt,
	DriveItSlow,
	TurnIt,
	LiftIt,
	DeliverIt,
	LowerWrist,
};

class Step {
public:
	typedef enum Commands Commands_t;
	float start;
	Commands_t command;

	Step();
	Step(Commands_t com, double howFar);
	// TODO need to add proximity sensor!
	AutonState_t ExecuteStep(DalekDrive *d, AHRS *ahrs, Intake *i, Lifter *l );
	virtual ~Step();
private:
	//Commands_t command;
	// In inches
	float distance;

	// In degrees, relative
	float angle;
	// In encoder ticks
	float position;
	bool Travel(double dist);
	bool Turn (double angle);
	bool Lift (double height);
	bool Deliver();
};

#endif /* STEP_H_ */
