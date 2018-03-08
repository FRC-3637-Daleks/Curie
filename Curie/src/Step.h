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

#define angleDiffLimit 0.5

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
};

class Step {
public:
	typedef enum Commands Commands_t;
	Step();
	Step(Commands_t com, double howFar);
	//TODO need to add proximity sensor!
	AutonState_t ExecuteStep(DalekDrive *d, AHRS *ahrs);
	virtual ~Step();
private:
	Commands_t command;
	//In inches
	float distance;
	//In degrees, relative
	float angle;
	bool Travel(double dist);
	bool Turn (double angle);
};

#endif /* STEP_H_ */
