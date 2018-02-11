//By: Nithya.N

#ifndef SRC_ELEVATOR_H_
#define SRC_ELEVATOR_H_

#include <Curie.h>

#include <WPILib.h>
#include <ctre/Phoenix.h>
#include <PIDController.h>

class Elevator {
public:

	enum elevPos {SWITCH,  SCALE, BASE_POS, MAX_HIGH, MAX_LOW};
	enum motorMode {PERCENTVOLTAGE, POSITION};

	Elevator(int elevMotor, int elevSlaveMotor, int potenimeter, double limitValue, double pidController);
	Elevator(WPI_TalonSRX* elevMotor, WPI_TalonSRX* elevSlaveMotorm, int potenimeter, double limitValue, double pidController);
	Elevator(WPI_TalonSRX& elevMoto, WPI_TalonSRX& elevSlaveMotor, int potenimeter, double limitValue, double pidController);
	~Elevator();

	void InvertElevatorMotor(bool isInverted);

	void MoveElevator(ControlMode mode, double value);

	void MaintainElevatorPosistion(ControlMode mode, double value);
	//PID LOOP

	void SetP (double p);
	//Proportional constant

	void SetI (double i);
	//Integral constant

	void SetD (double d);
	//Derivative constant

	void StopElevator();

	int GetLimit();

	 private:

		void InitElevator();

		WPI_TalonSRX *m_elevMotor, *m_elevSlaveMotor;
		Potentiometer *m_potenimeter;
		DigitalInput *m_limitValue;
		PIDController *m_pidController;

		bool isinverted;
		bool m_needFree;
	};

	#endif /* SRC_ELEVATOR_H_ */









