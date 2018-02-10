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

// I have no idea what to instantiate the encoders with or utilize them so yeah.
Climber::Climber(int motorChannel, int shifter, int lock, int wings, int encoderPortA, int encoderPortB)
{
	m_motor = new WPI_TalonSRX(motorChannel);
	m_shifter = new Solenoid(shifter);
	m_lock = new Solenoid(lock);
	m_wings = new Solenoid(wings);
	m_encoder = new Encoder(encoderPortA,encoderPortB,false,Encoder::EncodingType::k4X);

	InitClimber();

	m_needFree = true;
}

Climber::Climber(WPI_TalonSRX* motor, Solenoid* shifter, Solenoid* lock, Solenoid* wings, Encoder* encoder)
{
	m_motor = motor;
	m_shifter = shifter;
	m_lock = lock;
	m_wings = wings;
	m_encoder = encoder;

	InitClimber();

	m_needFree = true;
}

Climber::Climber(WPI_TalonSRX& motor, Solenoid& shifter, Solenoid& lock, Solenoid& wings, Encoder& encoder)
{
	m_motor = &motor;
	m_shifter = &shifter;
	m_lock = &lock;
	m_wings = &wings;
	m_encoder = &encoder;

	InitClimber();

	m_needFree = true;
}

Climber::~Climber()
{
	delete m_motor;
	delete m_shifter;
	delete m_lock;
	delete m_wings;
	delete m_encoder;

	m_needFree = false;

	return;
}

//Need to configure Encoder
void
Climber::InitClimber(void) {
	state = PRECLIMB;
}

void
Climber::DeployHook(void) {
	//No idea here.
	state = HOOKDEPLOYED;
}

void
Climber::DeployWings(void) {
	m_wings->Set(true);
}

void
Climber::DoClimb(void){
	//if (time is in last 30 seconds of Teleop && (state == HOOKDEPLOYED || state == CLIMBING))
	state = CLIMBING;
	m_motor->Set(0.5);
}


void
Climber::Hold(void) {
	if (state == CLIMBING) {
		state = HOLDING;
		m_lock->Set(true);
		m_motor->Set(0);
	}
}

