//By: Nithya.N

#include <Curie.h>
#include <WPILib.h>
#include <ctre/Phoenix.h>
#include <Lifter.h>
#include <Elevator.h>


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
	m_lifter->SetOperatingMode(Lifter::ELEVATOR_MODE);
	m_lifter->SetTalonMode(Lifter::PERCENT_VBUS);
}

void
Elevator::InvertElevatorMotor(bool isInverted)
{
	if(m_lifter->GetOperatingMode() == Lifter::ELEVATOR_MODE)
		m_lifter->InvertMotor(isInverted);
}


void
Elevator::MoveToPosition(double value)
{
	if(m_lifter->GetOperatingMode() == Lifter::ELEVATOR_MODE) {
		if(m_lifter->GetTalonMode() != Lifter::POSITION)
			m_lifter->SetTalonMode(Lifter::POSITION);
		m_lifter->Set(value);
	}
}

void
Elevator::MaintainElevatorPosition(double value)
{
	if(m_lifter->GetOperatingMode() == Lifter::ELEVATOR_MODE) {
		if(m_lifter->GetTalonMode() == Lifter::POSITION)
			m_lifter->Set(value);
	}
}

void
Elevator::ManualDown()
{
	if(m_lifter->GetOperatingMode() == Lifter::ELEVATOR_MODE)
		m_lifter->Set(AtBottom() ? ELEVATOR_DOWN_SPEED : 0.0);
}

void
Elevator::ManualUp()
{
	if(m_lifter->GetOperatingMode() == Lifter::ELEVATOR_MODE)
		m_lifter->Set(AtTop() ? ELEVATOR_UP_SPEED : 0.0);
}

void
Elevator::StopElevator(void)
{
	if(m_lifter->GetOperatingMode() == Lifter::ELEVATOR_MODE)
		m_lifter->Set(0.0);
}

bool
Elevator::AtTop()
{
	return (m_limitTop->Get() == 0);
}

bool
Elevator::AtBottom()
{
	return (m_limitBottom->Get() == 0);
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
