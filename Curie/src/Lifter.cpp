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

Lifter::Lifter(int masterChannel, int slaveChannel, int shifterChannel)
{
	m_master  = new WPI_TalonSRX(masterChannel);
	m_slave   = new WPI_TalonSRX(slaveChannel);
	m_shifter = new Solenoid(shifterChannel);
	m_omode   = ELEVATOR_MODE;
	m_tmode   = PERCENT_VBUS;
	initLifter();
	m_needFree = true;
}

Lifter::Lifter(WPI_TalonSRX *master, WPI_TalonSRX *slave, Solenoid *shifter)
{
	m_master  = master;
	m_slave   = slave;
	m_shifter = shifter;
	m_omode   = ELEVATOR_MODE;
	m_tmode   = PERCENT_VBUS;
	initLifter();
	m_needFree = false;
}

Lifter::Lifter(WPI_TalonSRX &master, WPI_TalonSRX &slave, Solenoid &shifter)
{
	m_master  = &master;
	m_slave   = &slave;
	m_shifter = &shifter;
	m_omode   = ELEVATOR_MODE;
	m_tmode   = PERCENT_VBUS;
	initLifter();
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
	m_master->ConfigOpenloopRamp(RAMP_RATE, CANTimeoutMs); // TBD: how many MS ???
	m_master->ConfigSelectedFeedbackSensor(ctre::phoenix::motorcontrol::FeedbackDevice::Analog, DriveSlotIdx, CANTimeoutMs);
	m_master->ConfigSetParameter(ParamEnum::eFeedbackNotContinuous, 1, 0x00,0x00, 0x00);
	m_master->SetInverted(false);

	if(m_slave) {
		m_slave->Set(ctre::phoenix::motorcontrol::ControlMode::Follower, m_master->GetDeviceID());
		m_slave->SetInverted(false);
		m_slave->ConfigOpenloopRamp(RAMP_RATE, CANTimeoutMs);
	}
	m_shifter->Set(true);
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
Lifter::setTalonMOde(tMode_t newMode)
{
	m_tmode = newMode;
}

Lifter::tMode_t
Lifter::getTalonMode()
{
	return m_tmode;
}
