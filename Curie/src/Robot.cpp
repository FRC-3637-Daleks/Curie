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
#include <Lifter.h>
#include <Intake.h>
#include <Climber.h>
#include <Elevator.h>

using namespace std;
using namespace frc;

class Robot : public frc::TimedRobot
{
public:
	WPI_TalonSRX *leftMotor, *rightMotor;
 	WPI_TalonSRX *leftSlave, *rightSlave;
	WPI_TalonSRX *wristMotor, *rollerMotor;
	WPI_TalonSRX *liftMaster, *liftSlave;
	Solenoid     *shifter;
	Joystick     *leftJoystick, *rightJoystick;
	XboxController *xbox;
	SerialPort *p;
	DalekDrive *d;
	Intake     *i;
	Climber    *c;
	Elevator   *e;
	Lifter     *l;

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
		liftMaster    = new WPI_TalonSRX(LiftMasterMotor);
		liftSlave     = new WPI_TalonSRX(LiftSlaveMotor);
#endif
		wristMotor    = new WPI_TalonSRX(WristMotor);
		rollerMotor   = new WPI_TalonSRX(RollerMotor);

		leftJoystick  = new Joystick(LeftJoystick);
		rightJoystick = new Joystick(RightJoystick);
		xbox          = new XboxController(XboxControls);
		shifter       = new Solenoid(Shifter);
		p             = new SerialPort(115200, SerialPort::kUSB, 8,
	             	 	 	 SerialPort::kParity_None, SerialPort::kStopBits_One);
		i             = new Intake(WristMotor, RollerMotor, IntakeLowerLimit,
								   IntakeUpperLimit, IntakeProximity);
#ifdef PRACTICE_BOT
		d             = new DalekDrive(leftMotor, rightMotor);
#else
		d             = new DalekDrive(leftMotor, leftSlave, rightMotor, rightSlave);
		l             = new Lifter(liftMaster, liftSlave, shifter);
		c             = new Climber(l, Brace, Lock, Wing,
									ClimbEncoderA, ClimbEncoderB);
		e             = new Elevator(l, ElevatorLowerLimit, ElevatorUpperLimit);
#endif
		d->SetInvertedMotor(LeftDriveMotor, false);
		d->SetInvertedMotor(RightDriveMotor, false);

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
			i->Lower();
		} else if (xbox->GetBButtonPressed()) {
			i->Raise();
		} else if ((xbox->GetAButtonReleased()) || (xbox->GetBButtonReleased())) {
			i->StopWrist();
		}

		// Roller Movement X/Y button
		if(xbox->GetXButtonPressed()) {
			i->Pull();
		} else if (xbox->GetYButtonPressed()) {
			i->Push();
		} else if ((xbox->GetXButtonReleased()) || (xbox->GetYButtonReleased())) {
			i->StopRoller();
		}

		//Climber Controls (Start:Climb, Back:Hold, LeftStick:Hook, RightStick:Wing)
		if(xbox->GetStickButtonPressed(frc::GenericHID::JoystickHand::kLeftHand)) {
			c->DeployHook();
		} else if (xbox->GetStickButtonPressed(frc::GenericHID::JoystickHand::kRightHand)) {
			c->DeployWings();
		} else if (xbox->GetStartButtonPressed()) {
			c->DoClimb();
		} else if (xbox->GetBackButtonPressed()) {
			c->Hold();
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
