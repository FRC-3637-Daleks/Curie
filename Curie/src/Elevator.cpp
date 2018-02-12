//By: Nithya.N

#include <Curie.h>
#include <WPILib.h>
#include <ctre/Phoenix.h>
#include <Lifter.h>
#include <Elevator.h>

Elevator::Elevator(int elevMotorChannel, int elevSlaveMotorChannel, int shifter,
		int limitTop, int limitBottom)
{
	m_lifter      = new Lifter(elevMotorChannel, elevSlaveMotorChannel, shifter);
	m_limitTop    = new DigitalInput(limitTop);
	m_limitBottom = new DigitalInput(limitBottom);

	InitElevator();
	m_needFree = true;
}

Elevator::Elevator(Lifter* lift, int limitTop, int limitBottom)
{
	m_lifter      = lift;
	m_limitTop    = new DigitalInput(limitTop);
	m_limitBottom = new DigitalInput(limitBottom);

	InitElevator();
	m_needFree = true;
}

Elevator::Elevator(Lifter& lift, int limitTop, int limitBottom)
{
	m_lifter      = &lift;
	m_limitTop    = new DigitalInput(limitTop);
	m_limitBottom = new DigitalInput(limitBottom);

	InitElevator();
	m_needFree = true;
}

Elevator::~Elevator()
{
	if (m_needFree)
		delete m_lifter;
	delete m_limitTop;
	delete m_limitBottom;

	m_needFree = false;
	return;
}

void
Elevator::InitElevator(void)
{
	m_lifter->setOperatingMode(Lifter::ELEVATOR_MODE);
	m_lifter->setTalonMode(Lifter::PERCENT_VBUS);
}

void
Elevator::InvertElevatorMotor(bool isInverted)
{
	if(m_lifter->getOperatingMode() == Lifter::ELEVATOR_MODE)
		m_lifter->invertMotor(isInverted);
}

double
Elevator::normalizeValue(double v)
{
	// if in %vbus mode, use limit switches.  If in position mode ???
	if(m_lifter->getTalonMode() == Lifter::PERCENT_VBUS) {
		if (v > 0.0) {
			if (m_limitTop->Get() == 1)
				v = 0.0;
		}
		else if (v < 0.0) {
			if (m_limitBottom->Get() == 1)
				v = 0.0;
		}
		return v;
	}
	return v;
}

void
Elevator::MoveElevator(double value)
{
	double nv = normalizeValue(value);

	if(m_lifter->getOperatingMode() == Lifter::ELEVATOR_MODE)
		m_lifter->Set(nv);
}

void
Elevator::MaintainElevatorPosistion(double value)
{
	if(m_lifter->getOperatingMode() == Lifter::ELEVATOR_MODE) {
		m_lifter->setTalonMode(Lifter::POSITION);
		m_lifter->Set(value);
	}
}

void
Elevator::StopElevator(void)
{
	if(m_lifter->getOperatingMode() == Lifter::ELEVATOR_MODE) {
		m_lifter->setTalonMode(Lifter::POSITION);
		m_lifter->Set(0.0);
	}
}

void
Elevator::SetP(double p)
{
	m_lifter->SetP(p);
}

void
Elevator::SetI(double i)
{
	m_lifter->SetI(i);
}

void
Elevator::SetD(double d)
{
	m_lifter->SetD(d);
}
