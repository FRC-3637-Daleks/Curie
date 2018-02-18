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

	Elevator(int elevMotor, int elevSlaveMotor, int shifter,
			int limitTop, int limitBottom);
	Elevator(Lifter *lift, int limitTop, int limitBottom);
	Elevator(Lifter &lift, int limitTop, int limitBottom);
	~Elevator();

	void MoveElevator(double value);
	void MaintainElevatorPosistion(double value);
	void InvertElevatorMotor(bool isInverted);
	void Up();
	void Down();
	void SetP(double p);
	void SetI(double i);
	void SetD(double d);
	void StopElevator();

private:
	void InitElevator();
	double normalizeValue(double v);
	Lifter *m_lifter;
	DigitalInput *m_limitTop;
	DigitalInput *m_limitBottom;
	bool m_needFree;
};

#endif /* SRC_ELEVATOR_H_ */









