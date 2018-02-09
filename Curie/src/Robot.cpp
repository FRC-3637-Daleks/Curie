/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include <iostream>
#include <string>

#include <WPILib.h>
#include <LiveWindow/LiveWindow.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>
#include <TimedRobot.h>
#include <ctre/Phoenix.h>
#include <DalekDrive.h>
#include <Curie.h>

using namespace std;
using namespace frc;

class Robot : public frc::TimedRobot
{
public:
	WPI_TalonSRX *leftMotor, *rightMotor;
 	WPI_TalonSRX *leftSlave, *rightSlave;
	WPI_TalonSRX *wristMotor, *rollerMotor;
	WPI_TalonSRX *winchMotor;
	Joystick *leftJoystick, *rightJoystick;
	XboxController *xbox;
	DigitalInput *intakeUpperLimit, *intakeLowerLimit, *intakeProximity;
	SerialPort *p;
	DalekDrive *d;
	Intake *i;

	void
	RobotInit()
	{
#ifdef PRACTICE_BOT
		leftMotor     = new WPI_TalonSRX(LeftDriveMotor);
		rightMotor    = new WPI_TalonSRX(RightDriveMotor);
#else
		leftMotor     = new WPI_TalonSRX(LeftDriveMotor);
		leftSlave     = new WPI_TalonSRX(LeftSlaveMotor);
		rightMotor    = new WPI_TalonSRX(RightDriveMotor);
		rightSlave    = new WPI_TalonSRX(RightSlaveMotor);
#endif
		wristMotor    = new WPI_TalonSRX(WristMotor);
		rollerMotor   = new WPI_TalonSRX(RollerMotor);

		leftJoystick  = new Joystick(LeftJoystick);
		rightJoystick = new Joystick(RightJoystick);
		xbox          = new XboxController(XboxControls);

		p = new SerialPort(115200, SerialPort::kUSB, 8,
	             SerialPort::kParity_None, SerialPort::kStopBits_One);
		     
		intakeUpperLimit = new DigitalInput(IntakeUpperLimit);
		intakeLowerLimit = new DigitalInput(IntakeLowerLimit);
		intakeProximity  = new DigitalInput(IntakeProximity);
#ifdef PRACTICE_BOT
		d             = new DalekDrive(leftMotor, rightMotor);
		d->SetInvertedMotor(LeftDriveMotor, true);
		d->SetInvertedMotor(RightDriveMotor, true);
#else
		d             = new DalekDrive(leftMotor, leftSlave, rightMotor, rightSlave);
		d->SetInvertedMotor(LeftDriveMotor, false);
		d->SetInvertedMotor(RightDriveMotor, false);
#endif
		autoLocation.AddDefault("Left", LEFT_POSITION);
		autoLocation.AddObject("Center", CENTER_POSITION);
		autoLocation.AddObject("Right", RIGHT_POSITION);
		frc::SmartDashboard::PutData("Autonomous Starting Location",
				&autoLocation);

		autoTarget.AddDefault("Switch", TARGET_SWITCH);
		autoTarget.AddObject("Scale", TARGET_SCALE);
		autoTarget.AddObject("AutoLine", TARGET_AUTOLINE);
		frc::SmartDashboard::PutData("Autonomous Target",
				&autoTarget);
	}


	void
	AutonomousInit() override
	{
		std::string loc = autoLocation.GetSelected();
		std::string tgt = autoTarget.GetSelected();

		gameData = frc::DriverStation::GetInstance().GetGameSpecificMessage();

		// get our starting position
		if(loc.compare(LEFT_POSITION) == 0)
			autoloc = 1;
		else if (loc.compare(CENTER_POSITION) == 0)
			autoloc = 2;
		else if (loc.compare(RIGHT_POSITION) == 0)
			autoloc = 3;
		else
			autoloc = 2;

		// compute autonomous objective, based on GameSpecificMessage
		if(tgt.compare(TARGET_SWITCH) == 0) {
			if(gameData[0] == 'L')
				autotgt = 1;
			else
				autotgt = 2;
		}
		else if (tgt.compare(TARGET_SCALE) == 0) {
			if(gameData[1] == 'L')
				autotgt = 3;
			else
				autotgt = 4;
		}
		else
			autotgt = 0;
	}

	void
	AutonomousPeriodic()
	{
		d->SetLeftRightMotorOutputs(0.0, 0.0);
	}

	void
	TeleopInit()
	{

	}

	void TeleopPeriodic()
	{
		bool useArcade;

		useArcade = (leftJoystick->GetZ() == -1.0);

		// Drive controls
		if (useArcade)
			d->ArcadeDrive(leftJoystick);
		else
			d->TankDrive(leftJoystick, rightJoystick);

		// Wrist Movement A/B button
		if(xbox->GetAButtonPressed()) {
			i->Raise();
		} else if (xbox->GetBButtonPressed()) {
			i->Lower();
		} else if ((xbox->GetAButtonReleased()) || (xbox->GetBButtonReleased())) {
			i->StopWrist();
		} else if ((i->WristUpperLimit()) || (i->WristLowerLimit())) {
			i->StopWrist();
		}

		// Roller Movement X/Y button
		if(xbox->GetXButtonPressed()) {
			i->Pull();
		} else if (xbox->GetYButtonPressed()) {
			i->Push();
		} else if ((xbox->GetXButtonReleased()) || (xbox->GetYButtonReleased())) {
			i->StopRoller();
		} else if (i->Proximity()) {
			i->StopRoller();
		}
	}

	void TestPeriodic()
	{

	}

private:
	frc::LiveWindow& m_lw = *LiveWindow::GetInstance();
	frc::SendableChooser<std::string> autoLocation;
	frc::SendableChooser<std::string> autoTarget;
	std::string gameData;
	int autoloc;
	int autotgt;
};

START_ROBOT_CLASS(Robot)
