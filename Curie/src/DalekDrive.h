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
/*
 * Utility class for handling Robot drive based on a definition of the motor
 * configuration.
 */
class DalekDrive {
  public:
	typedef enum Motors MotorType_t;

	DalekDrive(int leftMotorChannel, int rightMotorChannel);
	DalekDrive(int leftMotorChannel, int leftSlaveMotorChannel,
             int rightMotorChannel, int rightSlaveMotorChannel);
	DalekDrive(WPI_TalonSRX* leftMotor, WPI_TalonSRX* rightMotor);
	DalekDrive(WPI_TalonSRX* leftMotor, WPI_TalonSRX* leftSlaveMotor,
			WPI_TalonSRX* rightMotor, WPI_TalonSRX* rightSlaveMotor);
	DalekDrive(WPI_TalonSRX& leftMotor, WPI_TalonSRX& rightMotor);
	DalekDrive(WPI_TalonSRX& leftMotor, WPI_TalonSRX& leftSlaveMotor,
			WPI_TalonSRX& rightMotor, WPI_TalonSRX& rightSlaveMotor);
	~DalekDrive();

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
	bool DriveOk();

 private:
	void InitDalekDrive();
	bool printFaults();
	WPI_TalonSRX *m_leftMotor;
	WPI_TalonSRX *m_rightMotor;
	WPI_TalonSRX *m_leftSlaveMotor;
	WPI_TalonSRX *m_rightSlaveMotor;
	frc::DifferentialDrive *m_drive;
	bool m_needFree;
};
#endif /* _DALEKDRIVE_H_ */
