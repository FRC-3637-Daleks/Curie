/*
 * Intake.cpp
 *
 *  Created on: Feb 6, 2018
 *      Author: GEM
 */

#include <Curie.h>

#include <WPILib.h>
#include <ctre/Phoenix.h>
#include <Intake.h>

Intake::Intake(int wristMotorChannel, int rollerMotorChannel, int intakeLowerLimit, int intakeUpperLimit, int intakeProximity) {

	m_wristMotor = new WPI_TalonSRX(wristMotorChannel);
	m_rollerMotor = new WPI_TalonSRX(rollerMotorChannel);
	m_lowerLimit = new DigitalInput(intakeLowerLimit);
	m_upperLimit = new DigitalInput(intakeUpperLimit);
	m_proximity = new DigitalInput(intakeProximity);

	m_needFree = true;
}

Intake::Intake(WPI_TalonSRX* wristMotor, WPI_TalonSRX* rollerMotor, int intakeLowerLimit, int intakeUpperLimit, int intakeProximity) {

	m_wristMotor = wristMotor;
	m_rollerMotor = rollerMotor;
	m_lowerLimit = new DigitalInput(intakeLowerLimit);
	m_upperLimit = new DigitalInput(intakeUpperLimit);
	m_proximity = new DigitalInput(intakeProximity);

	m_needFree = true;
}

Intake::Intake(WPI_TalonSRX& wristMotor, WPI_TalonSRX& rollerMotor, int intakeLowerLimit, int intakeUpperLimit, int intakeProximity) {

	m_wristMotor = &wristMotor;
	m_rollerMotor = &rollerMotor;
	m_lowerLimit = new DigitalInput(intakeLowerLimit);
	m_upperLimit = new DigitalInput(intakeUpperLimit);
	m_proximity = new DigitalInput(intakeProximity);

	m_needFree = true;
}

void Intake::Raise() {
	m_wristMotor->Set(0.2);
}

void Intake::Lower() {
	m_wristMotor->Set(-0.2);
}

void Intake::Push() {
	m_rollerMotor->Set(1.0);
}

void Intake::Pull() {
	m_rollerMotor->Set(1.0);
}

void Intake::StopWrist() {
	m_wristMotor->Set(0.0);
}

void Intake::StopRoller() {
	m_rollerMotor->Set(0.0);
}

bool Intake::Proximity() {
	return (m_proximity->Get() == 1);
}

bool Intake::WristUpperLimit() {
	return (m_upperLimit->Get() == 1);
}

bool Intake::WristLowerLimit() {
	return (m_lowerLimit->Get() == 1);
}

Intake::~Intake()
{
	if(m_needFree) {
		delete m_wristMotor;
		delete m_rollerMotor;
	}
	m_needFree = false;
	return;
}

