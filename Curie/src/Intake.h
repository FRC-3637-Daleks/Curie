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
#include <DalekTask.h>

class Intake: public DalekTask {
  public:
	typedef enum intakeState { Stopped, MovingUp, AtTop, MovingDown, AtBottom } iState_t;

	Intake(int wristMotorChannel, int rollerMotorChannel,
		   int intakeLowerLimit, int intakeUpperLimit);
	Intake(WPI_TalonSRX* wristMotor, WPI_TalonSRX* rollerMotor,
		   int intakeLowerLimit, int intakeUpperLimit);
	Intake(WPI_TalonSRX& wristMotor, WPI_TalonSRX& rollerMotor,
		   int intakeLowerLimit, int intakeUpperLimit);

	void Run();

	void Raise();
	void Lower();
	void Pull();
	void Push();
	void StopWrist();
	void StopRoller();
	bool WristUpperLimit();
	bool WristLowerLimit();
	double WristPosition();
	void ResetPosition();
	~Intake();

  private:
  	void InitIntake();
  	void IntakeOK();
  	WPI_TalonSRX *m_wristMotor;
  	WPI_TalonSRX *m_rollerMotor;
  	DigitalInput *m_lowerLimit;
  	DigitalInput *m_upperLimit;
	iState_t m_state;
  	bool m_needFree;

};

#endif /* SRC_INTAKE_H_ */
