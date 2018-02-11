/*
 * Step.h
 *
 *  Created on: Feb 8, 2018
 *      Author: FLL2
 */

#ifndef STEP_H_
#define STEP_H_

#include <IMU.h>
#include <DalekDrive.h>

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
	TurnIt,
};

class Step {
public:
	typedef enum Commands Commands_t;
	Step();
	Step(Commands_t com, double dist);
	//TODO need to add proximity sensor!
	AutonState_t ExecuteStep(DalekDrive *d, IMU *imu);
	virtual ~Step();
private:
	Commands_t command;
	float distance;
	bool Travel(double dist);
	bool Turn (double angle);
};

#endif /* STEP_H_ */
