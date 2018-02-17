/*
 * SimplePath.h
 *
 *  Created on: Feb 8, 2018
 *      Author: FLL2
 */

#ifndef SIMPLEPATH_H_
#define SIMPLEPATH_H_

#include <vector>
#include <cstddef>
#include <Step.h>




class SimplePath {
public:
	SimplePath(size_t len);
	virtual ~SimplePath();
	void AddStep(Step newStep);
	//TODO need to add proximity sensor!
	AutonState_t RunPath(DalekDrive *d, IMU *imu);
private:
	std::vector<Step> steps;
	AutonState_t state;
	int currentStepNumber = 0;
	double distanceTraveled = 0.0;
};

#endif /* SIMPLEPATH_H_ */
