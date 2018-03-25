/*
 * DalekDrive.h
 *
 *  Created on: Jan 30, 2017
 *      Author: Michael
 */
#ifndef _DALEKDRIVE_H_
#define _DALEKDRIVE_H_

#include <Curie.h>

#include <WPILib.h>
#include <ctre/Phoenix.h>
#include <AHRS.h>

/*
 * Utility class for handling Robot drive based on a definition of the motor
 * configuration.
 */
#define PRECISION_ADJUSTMENT 0.5

class DalekDrive : public PIDOutput {
  public:
	typedef enum Motors MotorType_t;
	typedef enum driveMode { Normal, Precision } driveMode_t;

	DalekDrive(int leftMotorChannel, int rightMotorChannel, AHRS *ahrs);
	DalekDrive(int leftMotorChannel, int leftSlaveMotorChannel,
             int rightMotorChannel, int rightSlaveMotorChannel, AHRS *ahrs);
	DalekDrive(WPI_TalonSRX* leftMotor, WPI_TalonSRX* rightMotor, AHRS *ahrs);
	DalekDrive(WPI_TalonSRX* leftMotor, WPI_TalonSRX* leftSlaveMotor,
			WPI_TalonSRX* rightMotor, WPI_TalonSRX* rightSlaveMotor, AHRS *ahrs);
	DalekDrive(WPI_TalonSRX& leftMotor, WPI_TalonSRX& rightMotor, AHRS *ahrs);
	DalekDrive(WPI_TalonSRX& leftMotor, WPI_TalonSRX& leftSlaveMotor,
			WPI_TalonSRX& rightMotor, WPI_TalonSRX& rightSlaveMotor, AHRS *ahrs);
	virtual ~DalekDrive();

	void Drive(double outputMagnitude, double curve);
	void TankDrive(frc::GenericHID* leftStick, frc::GenericHID* rightStick,
                 bool squaredInputs = true);
	void TankDrive(frc::GenericHID& leftStick, frc::GenericHID& rightStick,
                 bool squaredInputs = true);
	void TankDrive(double leftValue, double rightValue,
                 bool squaredInputs = true);
	void ArcadeDrive(frc::GenericHID* stick, bool squaredInputs = true);
	void ArcadeDrive(frc::GenericHID& stick, bool squaredInputs = true);
	void ArcadeDrive(double moveValue, double rotateValue,
                   bool squaredInputs = true);
	void SetLeftRightMotorOutputs(double leftOutput, double rightOutput);
	void SetInvertedMotor(MotorType_t motor, bool isInverted);
	void SetMaxOutput(double maxOutput);
	void SetControlMode(ctre::phoenix::motorcontrol::ControlMode mode);
	void DriveStraight(double speed);
	void TurnToHeading(double speed, double heading);
	int  GetPosition(MotorType_t motor);
	double GetDistance();
	int  GetVelocity(MotorType_t motor);
	void SetPrecisionMode(bool v);
	bool DriveOk();
	bool isTurnCCW(float target, float current);
	float headingDiff(float target, float current);
	void DisableTurnControl();
	virtual void PIDWrite(double output);

 private:
    const double kP = 0.03;
    const double kI = 0.00;
    const double kD = 0.00;
    const double kF = 0.00;
    const double kToleranceDegrees = 2.0;

	void InitDalekDrive();
	bool printFaults();
	float Normalize(float v);
	WPI_TalonSRX *m_leftMotor;
	WPI_TalonSRX *m_rightMotor;
	WPI_TalonSRX *m_leftSlaveMotor;
	WPI_TalonSRX *m_rightSlaveMotor;
	AHRS *m_ahrs;
	frc::DifferentialDrive *m_drive;
	PIDController *m_turnController;
	ctre::phoenix::motorcontrol::ControlMode inMode;
	driveMode_t m_mode;
	double angleAdj;

	bool m_needFree;
};
#endif /* _DALEKDRIVE_H_ */
