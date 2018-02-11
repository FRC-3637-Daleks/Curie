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

Climber::Climber(Lifter *lift, int brace, int lock, int wings,
		int encoderPortA, int encoderPortB)
{
	m_lifter  = lift;
	m_lock    = new Solenoid(lock);
	m_wings   = new Solenoid(wings);
	m_encoder = new Encoder(encoderPortA, encoderPortB, false,
			Encoder::EncodingType::k4X);

	InitClimber();

	m_needFree = true;
}

Climber::Climber(Lifter *lift, Solenoid *brace, Solenoid* lock, Solenoid* wings,
		Encoder* encoder)
{
	m_lifter  = lift;
	m_brace   = brace;
	m_lock    = lock;
	m_wings   = wings;
	m_encoder = encoder;

	InitClimber();

	m_needFree = false;
}

Climber::Climber(Lifter& lift, Solenoid& brace, Solenoid& lock, Solenoid& wings,
		Encoder& encoder)
{
	m_lifter  = &lift;
	m_brace   = &brace;
	m_lock    = &lock;
	m_wings   = &wings;
	m_encoder = &encoder;
	m_climbLimit = 0;
	InitClimber();

	m_needFree = false;
}

Climber::~Climber()
{
	if(m_needFree) {
		delete m_brace;
		delete m_lock;
		delete m_wings;
		delete m_encoder;
	}
	m_needFree = false;
	return;
}

void
Climber::setClimbLimit(int newLimit)
{
	m_climbLimit = newLimit;
}

void
Climber::InitClimber(void)
{
	state = PRECLIMB;
}

void
Climber::DeployHook(void)
{
	//No idea here.
	state = HOOKDEPLOYED;
}

void
Climber::DeployWings(void)
{
	m_wings->Set(true);
}

void
Climber::DeployBrace(void)
{
	m_brace->Set(true);
}

void
Climber::DoClimb(void)
{
	//if (time is in last 30 seconds of Teleop && (state == HOOKDEPLOYED || state == CLIMBING))
	state = CLIMBING;
	m_lifter->setOperatingMode(Lifter::CLIMBING_MODE);
	m_lifter->setTalonMode(Lifter::PERCENT_VBUS);
	if(m_encoder->Get() > m_climbLimit) {
		Hold();
	}
	else
		m_lifter->Set(0.5);
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
