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
#include <Curie.h>
#include <AHRS.h>
#include <Step.h>
#include <WPILib.h>
#include <Intake.h>
#include <Lifter.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>

enum TargetType {
	/* Target Types
	 */
	LeftSwitch=0,
	RightSwitch,
	LeftScale,
	RightScale,
	BaseLine,
};

enum StartPositions {
	/* Starting Driver Station Positions
	 *
	 */
	Left=0,
	Center,
	Right,
};

typedef enum TargetType TargetType_t;
typedef enum StartPositions StartPositions_t;

class SimplePath {
public:
	SimplePath(size_t len);
	SimplePath(StartPositions_t start, TargetType_t target);
	virtual ~SimplePath();
	void AddStep(Step newStep);
	// TODO need to add proximity sensor!
	AutonState_t RunPath(DalekDrive *d, AHRS *ahrs, Intake *i, Lifter *l,
			Ultrasonic *ul, Ultrasonic *ur);

private:
	void CreateSameSideSwitchPath(StartPositions_t start);
	void CreateSameSideScalePath(StartPositions_t start);
	void CreateOppSideSwitchPath(StartPositions_t start);
	void CreateOppSideScalePath(StartPositions_t start);
	double DetermineAngle(StartPositions_t start, double left, double right);
	std::vector<Step> steps;
	AutonState_t state;
	int currentStepNumber = 0;
	double distanceTraveled = 0.0;
};

#endif /* SIMPLEPATH_H_ */
