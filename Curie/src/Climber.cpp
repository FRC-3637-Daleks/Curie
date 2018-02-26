/*
 * Climber.cpp
 *
 *  Created on: Jan 29, 2018
 *      Author: Kent
 */

#include <Curie.h>
#include <WPILib.h>
#include <ctre/Phoenix.h>

#include <Climber.h>

Climber::Climber(Lifter *lift, int brace, int lock,	int ultra)
{
	m_lifter = lift;
	m_lock   = new Solenoid(PCMID, lock);
	m_brace  = new Solenoid(PCMID, brace);
	m_ultra  = new AnalogInput(ultra);

	InitClimber();
	m_needFree = true;
}

Climber::Climber(Lifter *lift, Solenoid *brace, Solenoid* lock,	AnalogInput* ultra)
{
	m_lifter = lift;
	m_brace  = brace;
	m_lock   = lock;
	m_ultra  = ultra;

	InitClimber();
	m_needFree = false;
}

Climber::Climber(Lifter& lift, Solenoid& brace, Solenoid& lock, AnalogInput& ultra)
{
	m_lifter = &lift;
	m_brace  = &brace;
	m_lock   = &lock;
	m_ultra  = &ultra;
	m_climbLimit = 0;

	InitClimber();
	m_needFree = false;
}

Climber::~Climber()
{
	if(m_needFree) {
		delete m_brace;
		delete m_lock;
		delete m_ultra;
	}
	m_needFree = false;
	return;
}

void
Climber::SetClimbLimit(int newLimit)
{
	m_climbLimit = newLimit;
}

void
Climber::InitClimber(void)
{
	state = PRECLIMB;
}

void
Climber::ManualDown()
{
	if(m_lifter->GetOperatingMode() == Lifter::CLIMBING_MODE)
		m_lifter->Set(CLIMB_DOWN_SPEED);
}

void
Climber::ManualUp()
{
	if(m_lifter->GetOperatingMode() == Lifter::CLIMBING_MODE)
		m_lifter->Set(CLIMB_UP_SPEED);
}

void
Climber::StopClimbing(void)
{
	if(m_lifter->GetOperatingMode() == Lifter::CLIMBING_MODE)
		m_lifter->Set(0.0);
}

void
Climber::DeployHook(void)
{
	//No idea here.
	state = HOOKDEPLOYED;
}

void
Climber::DeployBrace(void)
{
	m_brace->Set(true);
}

float
Climber::GetHeight()
{
	return (m_ultra->GetVoltage()*INCHES_PER_VOLTS);
}

void
Climber::DoClimb(void)
{
	state = CLIMBING;
	m_lifter->SetOperatingMode(Lifter::CLIMBING_MODE);
	m_lifter->SetTalonMode(Lifter::PERCENT_VBUS);
	if( GetHeight() > m_climbLimit) {
		Hold();
	}
	else
		m_lifter->Set(CLIMB_UP_SPEED);
}

void
Climber::Hold(void)
{
	if (state == CLIMBING) {
		state = HOLDING;
		m_lock->Set(true);
		m_lifter->Set(0.0);
	}
}
