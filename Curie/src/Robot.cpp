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
	Joystick *leftJoystick, *rightJoystick;
	XboxController *xbox;
	DalekDrive *d;

	void
	RobotInit()
	{
		//leftMotor     = new WPI_TalonSRX(3);
		//leftSlave     = new WPI_TalonSRX(4);
		//rightMotor    = new WPI_TalonSRX(7);
		//rightSlave    = new WPI_TalonSRX(8);
		leftMotor     = new WPI_TalonSRX(3);
		// leftSlave     = new WPI_TalonSRX(4);
		rightMotor    = new WPI_TalonSRX(1);
		// rightSlave    = new WPI_TalonSRX(8);

		wristMotor    = new WPI_TalonSRX(WRIST_MOTOR);
		rollerMotor   = new WPI_TalonSRX(ROLLER_MOTOR);

		leftJoystick  = new Joystick(LEFT_JOYSTICK);
		rightJoystick = new Joystick(RIGHT_JOYSTICK);
		xbox          = new XboxController(XBOX_CONTROLS);
		// d             = new DalekDrive(leftMotor, leftSlave, rightMotor, rightSlave);
		d             = new DalekDrive(leftMotor, rightMotor);
		d->SetInvertedMotor(LEFT_DRIVEMOTOR, true);
		d->SetInvertedMotor(RIGHT_DRIVEMOTOR, true);
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
			wristMotor->Set(0.2);
		} else if (xbox->GetBButtonPressed()) {
			wristMotor->Set(-0.2);
		} else if ((xbox->GetAButtonReleased()) || (xbox->GetBButtonReleased())) {
			wristMotor->Set(0.0);
		}

		// Roller Movement X/Y button
		if(xbox->GetXButtonPressed()) {
			rollerMotor->Set(0.75);
		} else if (xbox->GetYButtonPressed()) {
			rollerMotor->Set(-0.75);
		} else if ((xbox->GetXButtonReleased()) || (xbox->GetYButtonReleased())) {
			rollerMotor->Set(0.0);
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
