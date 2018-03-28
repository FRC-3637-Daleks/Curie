/*
 * DalekDrive.cpp
 *
 *  Created on: Jan 30, 2017
 *      Author: Michael
 */

#include <Curie.h>

#include <WPILib.h>
#include <ctre/Phoenix.h>
#include <AHRS.h>
#include <DalekDrive.h>

DalekDrive::DalekDrive(int leftMotorChannel, int rightMotorChannel, AHRS *a)
{
	m_leftMotor       = new WPI_TalonSRX(leftMotorChannel);
	m_rightMotor      = new WPI_TalonSRX(rightMotorChannel);
	m_leftSlaveMotor  = NULL;
	m_rightSlaveMotor = NULL;
	m_drive           = new DifferentialDrive(*m_leftMotor, *m_rightMotor);
	m_mode            = Normal;
	m_ahrs            = a;
    m_turnController  = new PIDController(kP, kI, kD, kF, m_ahrs, this);
	InitDalekDrive();

	m_needFree = true;
}

DalekDrive::DalekDrive(int leftMotorChannel, int leftSlaveMotorChannel,
			int rightMotorChannel, int rightSlaveMotorChannel, AHRS *a)
{
	m_leftMotor       = new WPI_TalonSRX(leftMotorChannel);
	m_rightMotor      = new WPI_TalonSRX(rightMotorChannel);
	m_leftSlaveMotor  = new WPI_TalonSRX(leftSlaveMotorChannel);
	m_rightSlaveMotor = new WPI_TalonSRX(rightSlaveMotorChannel);
	m_drive           = new DifferentialDrive(*m_leftMotor, *m_rightMotor);
	m_mode            = Normal;
	m_ahrs            = a;
	m_turnController  = new PIDController(kP, kI, kD, kF, m_ahrs, this);
	InitDalekDrive();

	m_needFree = true;
}

DalekDrive::DalekDrive(WPI_TalonSRX* leftMotor, WPI_TalonSRX* rightMotor,
		AHRS *a)
{
	m_leftMotor       = leftMotor;
	m_rightMotor      = rightMotor;
	m_leftSlaveMotor  = NULL;
	m_rightSlaveMotor = NULL;
	m_drive           = new DifferentialDrive(*m_leftMotor, *m_rightMotor);
	m_mode            = Normal;
	m_ahrs            = a;
	m_turnController  = new PIDController(kP, kI, kD, kF, m_ahrs, this);
	InitDalekDrive();

	m_needFree = false;
}

DalekDrive::DalekDrive(WPI_TalonSRX& leftMotor, WPI_TalonSRX& rightMotor,
		AHRS *a)
{
	m_leftMotor       = &leftMotor;
	m_rightMotor      = &rightMotor;
	m_leftSlaveMotor  = NULL;
	m_rightSlaveMotor = NULL;
	m_drive           = new DifferentialDrive(*m_leftMotor, *m_rightMotor);
	m_mode            = Normal;
	m_ahrs            = a;
	m_turnController  = new PIDController(kP, kI, kD, kF, m_ahrs, this);
	InitDalekDrive();

	m_needFree = false;
}

DalekDrive::DalekDrive(WPI_TalonSRX* leftMotor, WPI_TalonSRX* leftSlaveMotor,
		 WPI_TalonSRX* rightMotor, WPI_TalonSRX* rightSlaveMotor, AHRS *a)
{
	m_leftMotor       = leftMotor;
	m_leftSlaveMotor  = leftSlaveMotor;
	m_rightMotor      = rightMotor;
	m_rightSlaveMotor = rightSlaveMotor;
	m_drive           = new DifferentialDrive(*m_leftMotor, *m_rightMotor);
	m_mode            = Normal;
	m_ahrs            = a;
	m_turnController  = new PIDController(kP, kI, kD, kF, m_ahrs, this);
	InitDalekDrive();

	m_needFree = false;
}

DalekDrive::DalekDrive(WPI_TalonSRX& leftMotor, WPI_TalonSRX& leftSlaveMotor,
		 WPI_TalonSRX& rightMotor, WPI_TalonSRX& rightSlaveMotor, AHRS *a)
{
	m_leftMotor       = &leftMotor;
	m_leftSlaveMotor  = &leftSlaveMotor;
	m_rightMotor      = &rightMotor;
	m_rightSlaveMotor = &rightSlaveMotor;
	m_drive           = new DifferentialDrive(*m_leftMotor, *m_rightMotor);
	m_mode            = Normal;
	m_ahrs            = a;
	m_turnController  = new PIDController(kP, kI, kD, kF, m_ahrs, this);
	InitDalekDrive();

	m_needFree = false;
}

DalekDrive::~DalekDrive()
{
	delete m_drive;
	if(m_needFree) {
		delete m_leftMotor;
		delete m_rightMotor;
		if(m_leftSlaveMotor)
			delete m_leftSlaveMotor;
		if(m_rightSlaveMotor)
			delete m_rightSlaveMotor;
	}
	m_turnController->Disable();
	delete m_turnController;

	m_needFree = false;
	return;
}

void
DalekDrive::PIDWrite(double output)
{
	angleAdj = output;
	return;
}

void
DalekDrive::DisableTurnControl()
{
	m_turnController->Disable();
}

void
DalekDrive::Drive(double outputMagnitude, double curve)
{
	// Differential drive only supports PercentOutput control mode
	if(inMode != ctre::phoenix::motorcontrol::ControlMode::PercentOutput)
		return;

	if(m_drive)
		m_drive->CurvatureDrive(outputMagnitude, curve, false);
	return;
}

void
DalekDrive::SetPrecisionMode(bool val)
{
	m_mode = (val) ? Precision : Normal;
	return;
}

float
DalekDrive::Normalize(float val)
{
	if(m_mode == Normal)
		return val;

	return val * PRECISION_ADJUSTMENT;
}

void
DalekDrive::TankDrive(GenericHID* leftStick, GenericHID* rightStick,
             bool squaredInputs)
{
	// Differential drive only supports PercentOutput control mode
	if(inMode != ctre::phoenix::motorcontrol::ControlMode::PercentOutput)
		return;

	if(m_drive)
		m_drive->TankDrive(Normalize(leftStick->GetY()),
				Normalize(rightStick->GetY()), squaredInputs);
}

void
DalekDrive::TankDrive(GenericHID& leftStick, GenericHID& rightStick,
             bool squaredInputs)
{
	// Differential drive only supports PercentOutput control mode
	if(inMode != ctre::phoenix::motorcontrol::ControlMode::PercentOutput)
		return;

	if(m_drive)
		m_drive->TankDrive(Normalize(leftStick.GetY()),
				Normalize(rightStick.GetY()), squaredInputs);
}

void
DalekDrive::TankDrive(double leftValue, double rightValue,
             bool squaredInputs)
{
	// Differential drive only supports PercentOutput control mode
	if(inMode != ctre::phoenix::motorcontrol::ControlMode::PercentOutput)
		return;

	if(m_drive)
		m_drive->TankDrive(Normalize(leftValue), Normalize(rightValue), squaredInputs);
}

void
DalekDrive::ArcadeDrive(GenericHID* stick, bool squaredInputs)
{
	// Differential drive only supports PercentOutput control mode
	if(inMode != ctre::phoenix::motorcontrol::ControlMode::PercentOutput)
		return;

	if(m_drive)
		m_drive->ArcadeDrive(Normalize(stick->GetY()), Normalize(stick->GetX()), squaredInputs);
}

void
DalekDrive::ArcadeDrive(GenericHID& stick, bool squaredInputs)
{
	// Differential drive only supports PercentOutput control mode
	if(inMode != ctre::phoenix::motorcontrol::ControlMode::PercentOutput)
		return;

	if(m_drive)
		m_drive->ArcadeDrive(Normalize(stick.GetY()), Normalize(stick.GetX()), squaredInputs);
}

void
DalekDrive::ArcadeDrive(double moveValue, double rotateValue,
               bool squaredInputs)
{
	// Differential drive only supports PercentOutput control mode
	if(inMode != ctre::phoenix::motorcontrol::ControlMode::PercentOutput)
		return;

	if(m_drive)
		m_drive->ArcadeDrive(Normalize(moveValue), Normalize(rotateValue), squaredInputs);
}

void
DalekDrive::SetLeftRightMotorOutputs(double leftValue, double rightValue)
{
	// Set the motor's value using mode
	m_leftMotor->Set(inMode,  leftValue);
	m_rightMotor->Set(inMode, rightValue);
	return;
}

void
DalekDrive::SetInvertedMotor(MotorType_t motor, bool isInverted)
{
	switch(motor) {
	case LeftDriveMotor:
		if(m_leftMotor) {
			m_leftMotor->SetInverted(isInverted);
			if(m_leftSlaveMotor)
				m_leftSlaveMotor->SetInverted(isInverted);
		}
		break;

	case RightDriveMotor:
		if(m_rightMotor) {
			m_rightMotor->SetInverted(isInverted);
			if(m_rightSlaveMotor)
					m_rightSlaveMotor->SetInverted(isInverted);
		}
		break;

	default:
		break;
	}
}

void
DalekDrive::SetMaxOutput(double maxOutput)
{
	if(m_drive)
		m_drive->SetMaxOutput(maxOutput);
}

void
DalekDrive::SetControlMode(ctre::phoenix::motorcontrol::ControlMode mode)
{
	switch(mode) {
	case ctre::phoenix::motorcontrol::ControlMode::PercentOutput:
		inMode = mode;
		break;
	case ctre::phoenix::motorcontrol::ControlMode::Position:
		inMode = mode;
		break;
	default:
		inMode = ctre::phoenix::motorcontrol::ControlMode::PercentOutput;
		break;
	}
}

int
DalekDrive::GetPosition(MotorType_t motor)
{
	if(motor == LeftDriveMotor)
		return m_leftMotor->GetSensorCollection().GetQuadraturePosition();
	return m_rightMotor->GetSensorCollection().GetQuadraturePosition();
}

double
DalekDrive::GetDistance()
{
	double left_ticks  = m_leftMotor->GetSensorCollection().GetQuadraturePosition();
	double right_ticks = m_rightMotor->GetSensorCollection().GetQuadraturePosition();
	double ave_ticks = (fabs(left_ticks) + fabs(right_ticks))/2.0;

	return ave_ticks / ENCODER_TICKS_PER_INCH;

}

int
DalekDrive::GetVelocity(MotorType_t motor)
{
	if(motor == LeftDriveMotor)
		return m_leftMotor->GetSensorCollection().GetQuadratureVelocity();
	return m_rightMotor->GetSensorCollection().GetQuadratureVelocity();
}

void
DalekDrive::DriveStraight(double speed, double target)
{
	if(!m_turnController->IsEnabled()) {
		// Acquire current yaw angle, using this as the target angle.
		m_turnController->SetSetpoint(m_ahrs->GetYaw());
		angleAdj = 0.0;
		m_turnController->Enable();
	}
	// slow down when we get near to our destination
	if(fabs(target - GetDistance()) < 36)
		speed /= 2;
	double leftStickValue  = speed + angleAdj;
	double rightStickValue = speed - angleAdj;
	TankDrive(-1 * leftStickValue,  -1 * rightStickValue, false);
	return;
}

bool
DalekDrive::isTurnCCW(float current, float target)
{
	const float diff = current - target;
	if(diff > 0.0f) {
		if (diff > 180.0f)
			return false;
		return true;
	}
	if(diff > -180.0f)
		return false;
	return true;
	// return ((diff > 0.0f) ? (diff > 180.0f) : (diff > -180.0f));
}

float
DalekDrive::headingDiff(float current, float target)
{
	const float diff = fmod(current - target + 3600, 360);
	return diff <= 180 ? diff : 360 - diff;
}

void
DalekDrive::TurnToHeading(double speed, double heading)
{
	float current = m_ahrs->GetFusedHeading();
	float hdiff   = headingDiff(current, heading);

	// not sure how to get the turnController to help
	if(m_turnController->IsEnabled())
		m_turnController->Disable();

	if(hdiff < 10.0f)
		speed /= 2;

	if(isTurnCCW(current, (float)heading))
		TankDrive(speed, -1 * speed, false);
	else
		TankDrive(-1 * speed, speed, false);
	return;
}

void
DalekDrive::InitDalekDrive(void)
{
	// Configure the Talon's as needed
	m_leftMotor->SetNeutralMode(NeutralMode::Brake);
	m_rightMotor->SetNeutralMode(NeutralMode::Brake);
	m_leftMotor->ConfigNominalOutputForward(0.0, CANTimeoutMs);
	m_leftMotor->ConfigNominalOutputReverse(-0.0, CANTimeoutMs);
	m_rightMotor->ConfigNominalOutputForward(0.0, CANTimeoutMs);
	m_rightMotor->ConfigNominalOutputReverse(-0.0, CANTimeoutMs);
	m_leftMotor->ConfigPeakOutputForward(1.0, CANTimeoutMs);
	m_leftMotor->ConfigPeakOutputReverse(-1.0, CANTimeoutMs);
	m_rightMotor->ConfigPeakOutputForward(1.0, CANTimeoutMs);
	m_rightMotor->ConfigPeakOutputReverse(-1.0, CANTimeoutMs);
	m_leftMotor->ConfigOpenloopRamp(RAMP_RATE, CANTimeoutMs); // TBD: how many MS ???
	m_rightMotor->ConfigOpenloopRamp(RAMP_RATE, CANTimeoutMs);
	m_leftMotor->ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder, PIDLoopIdx, CANTimeoutMs);
	m_rightMotor->ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder, PIDLoopIdx, CANTimeoutMs);
	m_leftMotor->SetSensorPhase(true);
	m_rightMotor->SetSensorPhase(false);
	m_leftMotor->SetSelectedSensorPosition(PIDSlotIdx, PIDLoopIdx, CANTimeoutMs);
	m_rightMotor->SetSelectedSensorPosition(PIDSlotIdx, PIDLoopIdx, CANTimeoutMs);
	m_leftMotor->SetInverted(false);
	m_rightMotor->SetInverted(false);
	m_leftMotor->GetSensorCollection().SetQuadraturePosition(0,0);
	m_rightMotor->GetSensorCollection().SetQuadraturePosition(0,0);

	if(m_leftSlaveMotor) {
		m_leftSlaveMotor->Set(ControlMode::Follower, m_leftMotor->GetDeviceID());
		m_leftSlaveMotor->SetInverted(false);
		m_leftSlaveMotor->ConfigOpenloopRamp(RAMP_RATE, CANTimeoutMs);
	}
	if(m_rightSlaveMotor) {
		m_rightSlaveMotor->Set(ControlMode::Follower, m_rightMotor->GetDeviceID());
		m_rightSlaveMotor->SetInverted(false);
		m_rightSlaveMotor->ConfigOpenloopRamp(RAMP_RATE, CANTimeoutMs);
	}
	inMode = ControlMode::PercentOutput;

    angleAdj = 0.0;
    m_turnController->SetInputRange(-180.0f,  180.0f);
    m_turnController->SetOutputRange(-1.0, 1.0);
    m_turnController->SetPercentTolerance(kToleranceDegrees/360.0);
    m_turnController->SetContinuous(true);
    m_turnController->Disable();
}

bool
DalekDrive::printFaults()
{
	Faults f;
	StickyFaults sf;
	bool faultFound = false;

	m_leftMotor->GetFaults(f);
	if(f.HasAnyFault()) {
		faultFound = true;
		frc::SmartDashboard::PutNumber("Left Talon reported faults", f.ToBitfield());
		if(m_leftSlaveMotor) {
			Faults slave;
			m_leftSlaveMotor->GetFaults(slave);
			frc::SmartDashboard::PutNumber("Left slave status",
					slave.ToBitfield());
		}
	}
	m_leftMotor->GetStickyFaults(sf);
	if(sf.HasAnyFault()) {
		frc::SmartDashboard::PutNumber("Left Talon reported sticky faults",
				sf.ToBitfield());
		m_leftMotor->ClearStickyFaults(0);
	}
	m_rightMotor->GetFaults(f);
	if(f.HasAnyFault()) {
		faultFound = true;
		frc::SmartDashboard::PutNumber("Right Talon reported faults", f.ToBitfield());
		if(m_rightSlaveMotor) {
			Faults slave;
			m_rightSlaveMotor->GetFaults(slave);
			frc::SmartDashboard::PutNumber("Right slave status",
				slave.ToBitfield());
		}
	}
	m_rightMotor->GetStickyFaults(sf);
	if(sf.HasAnyFault()) {
		frc::SmartDashboard::PutNumber("Right Talon reported sticky faults",
				sf.ToBitfield());
		m_leftMotor->ClearStickyFaults(0);
	}

	return faultFound;
}

bool
DalekDrive::DriveOk()
{
	// update dashboard of current draw for motors
	frc::SmartDashboard::PutNumber("Left Motor current",
			m_leftMotor->GetOutputCurrent());
	if(m_leftSlaveMotor)
		frc::SmartDashboard::PutNumber("Left Slave Motor current",
			m_leftSlaveMotor->GetOutputCurrent());
	frc::SmartDashboard::PutNumber("Right Motor current",
			m_rightMotor->GetOutputCurrent());
	if(m_rightSlaveMotor)
		frc::SmartDashboard::PutNumber("Right Slave Motor current",
			m_rightSlaveMotor->GetOutputCurrent());

	// check for motor faults
	if (printFaults())
		return false;

	return true;
}
