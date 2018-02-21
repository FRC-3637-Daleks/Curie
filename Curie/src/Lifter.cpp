/*
 * Lifter.cpp
 *
 *  Created on: Feb. 10, 2018
 *      Author: Michael
 */

#include <Curie.h>
#include <WPILib.h>
#include <ctre/Phoenix.h>

#include <Lifter.h>

Lifter::Lifter(int masterChannel, int slaveChannel, int shifterChannel, int pot)
{
	m_master  = new WPI_TalonSRX(masterChannel);
	m_slave   = new WPI_TalonSRX(slaveChannel);
	m_shifter = new Solenoid(PCMID, shifterChannel);
	m_pot     = new AnalogInput(pot);
	m_omode   = ELEVATOR_MODE;
	m_tmode   = PERCENT_VBUS;
	initLifter();
	m_needFree = true;
}

Lifter::Lifter(WPI_TalonSRX *master, WPI_TalonSRX *slave, Solenoid *shifter, int pot)
{
	m_master  = master;
	m_slave   = slave;
	m_shifter = shifter;
	m_pot     = new AnalogInput(pot);
	m_omode   = ELEVATOR_MODE;
	m_tmode   = PERCENT_VBUS;
	initLifter();
	m_needFree = false;
}

Lifter::Lifter(WPI_TalonSRX &master, WPI_TalonSRX &slave, Solenoid &shifter, int pot)
{
	m_master  = &master;
	m_slave   = &slave;
	m_shifter = &shifter;
	m_pot     = new AnalogInput(pot);
	m_omode   = ELEVATOR_MODE;
	m_tmode   = PERCENT_VBUS;
	initLifter();
	m_needFree = false;
}

Lifter::~Lifter()
{
	if(m_needFree) {
		delete m_master;
		delete m_slave;
		delete m_shifter;
	}
	delete m_pot;
	m_needFree = false;
}

void
Lifter::initLifter()
{
	m_master->SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
	m_master->ConfigNominalOutputForward(0.0, CANTimeoutMs);
	m_master->ConfigNominalOutputReverse(-0.0, CANTimeoutMs);
	m_master->ConfigPeakOutputForward(1.0, CANTimeoutMs);
	m_master->ConfigPeakOutputReverse(-1.0, CANTimeoutMs);
	m_master->ConfigOpenloopRamp(RAMP_RATE, CANTimeoutMs);
	m_master->SetInverted(false);
	m_slave->Set(ControlMode::Follower, m_master->GetDeviceID());
	m_slave->SetInverted(false);
	m_slave->ConfigOpenloopRamp(RAMP_RATE, CANTimeoutMs);

	m_shifter->Set(true);
	return;
}

void
Lifter::invertMotor(bool invert)
{
	m_master->SetInverted(invert);
	m_slave->SetInverted(invert);
	return;
}

void
Lifter::setOperatingMode(opMode_t newMode)
{
	// set the operating mode, and shift the lifter into the correct gear
	switch(newMode) {
	case ELEVATOR_MODE:
		m_omode = newMode;
		m_shifter->Set(true);
		break;
	case CLIMBING_MODE:
		m_omode = newMode;
		m_shifter->Set(false);
		break;
	default:
		break;
	}
}

Lifter::opMode_t
Lifter::getOperatingMode()
{
	return m_omode;
}

void
Lifter::Set(double value)
{
	switch(m_tmode) {
	case PERCENT_VBUS:
		m_master->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput,
				value);
		break;
	case POSITION:
		m_master->Set(ctre::phoenix::motorcontrol::ControlMode::Position,
				value);
		break;
	default:
		break;
	}
	return;
}

void
Lifter::setTalonMode(tMode_t newMode)
{
	m_tmode = newMode;
}

Lifter::tMode_t
Lifter::getTalonMode()
{
	return m_tmode;
}

void
Lifter::SetP(double p)
{
	// set talon pid p value
}

void
Lifter::SetI(double i)
{
	// set talon pid i value
}

void
Lifter::SetD(double d)
{
	// set talon pid d value
}
