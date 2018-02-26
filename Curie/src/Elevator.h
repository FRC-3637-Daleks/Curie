//By: Nithya.N

#ifndef SRC_ELEVATOR_H_
#define SRC_ELEVATOR_H_

#include <Curie.h>

#include <WPILib.h>
#include <ctre/Phoenix.h>
#include <Lifter.h>


class Elevator {
public:
	enum elevPos {SWITCH, SCALE, BASE_POS, MAX_HIGH, MAX_LOW};

	Elevator(Lifter *lift, int limitTop, int limitBottom);
	Elevator(Lifter &lift, int limitTop, int limitBottom);
	~Elevator();

	void MoveToPosition(double value);
	void MaintainElevatorPosition(double value);
	void InvertElevatorMotor(bool isInverted);
	void ManualDown();
	void ManualUp();
	bool AtTop();
	bool AtBottom();
	void SetP(double p);
	void SetI(double i);
	void SetD(double d);
	void StopElevator();

private:
	void InitElevator();
	Lifter *m_lifter;
	DigitalInput *m_limitTop;
	DigitalInput *m_limitBottom;
	bool m_needFree;
};

#endif /* SRC_ELEVATOR_H_ */









