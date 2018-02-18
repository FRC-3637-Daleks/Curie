/*
 * Intake.h
 *
 *  Created on: Feb 6, 2018
 *      Author: GEM
 */

#ifndef SRC_INTAKE_H_
#define SRC_INTAKE_H_

#include <Curie.h>

#include <WPILib.h>
#include <ctre/Phoenix.h>

class Intake {
  public:
	typedef enum Motors MotorType_t;

	Intake(int wristMotorChannel, int rollerMotorChannel,
		   int intakeLowerLimit, int intakeUpperLimit, int intakeProximity);
	Intake(WPI_TalonSRX* wristMotor, WPI_TalonSRX* rollerMotor,
		   int intakeLowerLimit, int intakeUpperLimit, int intakeProximity);
	Intake(WPI_TalonSRX& wristMotor, WPI_TalonSRX& rollerMotor,
		   int intakeLowerLimit, int intakeUpperLimit, int intakeProximity);

	void Raise();
	void Lower();
	void Pull();
	void Push();
	void StopWrist();
	void StopRoller();
	bool Proximity();
	bool WristUpperLimit();
	bool WristLowerLimit();

	~Intake();

  private:
  	void InitIntake();
  	void IntakeOK();
  	WPI_TalonSRX *m_wristMotor;
  	WPI_TalonSRX *m_rollerMotor;
  	DigitalInput *m_lowerLimit;
  	DigitalInput *m_upperLimit;
	DigitalInput *m_proximity;
  	bool m_needFree;

};

#endif /* SRC_INTAKE_H_ */

