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

Intake::Intake(int wristMotorChannel, int rollerMotorChannel,
			   int intakeLowerLimit, int intakeUpperLimit, int intakeProximity)
{

	m_wristMotor    = new WPI_TalonSRX(wristMotorChannel);
	m_rollerMotor   = new WPI_TalonSRX(rollerMotorChannel);
	m_lowerLimit    = new DigitalInput(intakeLowerLimit);
	m_upperLimit    = new DigitalInput(intakeUpperLimit);
	m_proximity     = new DigitalInput(intakeProximity);

	m_needFree      = true;
}

Intake::Intake(WPI_TalonSRX* wristMotor, WPI_TalonSRX* rollerMotor,
			   int intakeLowerLimit, int intakeUpperLimit, int intakeProximity)
{

	m_wristMotor    = wristMotor;
	m_rollerMotor   = rollerMotor;
	m_lowerLimit    = new DigitalInput(intakeLowerLimit);
	m_upperLimit    = new DigitalInput(intakeUpperLimit);
	m_proximity     = new DigitalInput(intakeProximity);

	m_needFree      = false;
}

Intake::Intake(WPI_TalonSRX& wristMotor, WPI_TalonSRX& rollerMotor,
			   int intakeLowerLimit, int intakeUpperLimit, int intakeProximity)
{

	m_wristMotor    = &wristMotor;
	m_rollerMotor   = &rollerMotor;
	m_lowerLimit    = new DigitalInput(intakeLowerLimit);
	m_upperLimit    = new DigitalInput(intakeUpperLimit);
	m_proximity     = new DigitalInput(intakeProximity);

	m_needFree      = false;
}

void
Intake::Raise()
{
#ifdef PRACTICE_BOT
	m_wristMotor->Set(0.2);
#else
	if (m_upperLimit->Get() == 0) {
		m_wristMotor->Set(0.2);
	} else {
		m_wristMotor->Set(0.0);
	}
#endif
}

void
Intake::Lower()
{
#ifdef PRACTICE_BOT
	m_wristMotor->Set(-0.2);
#else
	if (m_lowerLimit->Get() == 0) {
		m_wristMotor->Set(-0.2);
	} else {
		m_wristMotor->Set(0.0);
	}
#endif
}

void
Intake::Push()
{
#ifdef PRACTICE_BOT
	m_rollerMotor->Set(0.75);
#else
	if (m_proximity->Get() == 0) {
		m_rollerMotor->Set(0.75);
	} else {
		m_rollerMotor->Set(0.0);
	}
#endif
}

void
Intake::Pull()
{
	m_rollerMotor->Set(-0.75);
}

void
Intake::StopWrist()
{
	m_wristMotor->Set(0.0);
}

void
Intake::StopRoller()
{
	m_rollerMotor->Set(0.0);
}

bool
Intake::Proximity()
{
	return (m_proximity->Get() == 1);
}

bool
Intake::WristUpperLimit()
{
	return (m_upperLimit->Get() == 1);
}

bool
Intake::WristLowerLimit()
{
	return (m_lowerLimit->Get() == 1);
}

Intake::~Intake()
{
	if(m_needFree) {
		delete m_wristMotor;
		delete m_rollerMotor;
	}
	if (true) {
		delete m_lowerLimit;
		delete m_upperLimit;
		delete m_proximity;
	}
	m_needFree      = false;
	return;
}

void
Intake::InitIntake()
{
	m_wristMotor->SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
	m_wristMotor->SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);

	m_wristMotor->ConfigNominalOutputForward(0.0, CANTimeoutMs);
	m_wristMotor->ConfigNominalOutputReverse(-0.0, CANTimeoutMs);
	m_rollerMotor->ConfigNominalOutputForward(0.0, CANTimeoutMs);
	m_rollerMotor->ConfigNominalOutputReverse(-0.0, CANTimeoutMs);

	m_wristMotor->ConfigPeakOutputForward(1.0, CANTimeoutMs);
	m_wristMotor->ConfigPeakOutputReverse(-1.0, CANTimeoutMs);
	m_rollerMotor->ConfigPeakOutputForward(1.0, CANTimeoutMs);
	m_rollerMotor->ConfigPeakOutputReverse(-1.0, CANTimeoutMs);

	m_wristMotor->ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder, DriveSlotIdx, CANTimeoutMs);
	m_rollerMotor->ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder, DriveSlotIdx, CANTimeoutMs);
	m_wristMotor->SetSensorPhase(true);
	m_rollerMotor->SetSensorPhase(false);
	m_wristMotor->SetSelectedSensorPosition(0, 0, CANTimeoutMs);
	m_rollerMotor->SetSelectedSensorPosition(0, 0, CANTimeoutMs);
	m_wristMotor->SetInverted(false);
	m_rollerMotor->SetInverted(false);
}

void
Intake::IntakeOK()
{
	frc::SmartDashboard::PutNumber("Wrist Motor current",
			m_wristMotor->GetOutputCurrent());
	frc::SmartDashboard::PutNumber("Roller Motor current",
			m_rollerMotor->GetOutputCurrent());

}

