#include <iostream>
#include <string>

#include <WPILib.h>
#include <LiveWindow/LiveWindow.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>
#include <TimedRobot.h>
#include <AHRS.h>
#include <ctre/Phoenix.h>
#include <DalekDrive.h>
#include <Curie.h>
#include <Lifter.h>
#include <Intake.h>
#include <Amcrest.h>
#include <SimplePath.h>

using namespace std;
using namespace frc;

class Robot : public frc::TimedRobot
{
public:
	WPI_TalonSRX *leftMotor,  *rightMotor;
 	WPI_TalonSRX *leftSlave,  *rightSlave;
	WPI_TalonSRX *wristMotor, *rollerMotor;
	WPI_TalonSRX *liftMaster, *liftSlave;
	AnalogInput	 *ultraLeft,  *ultraRight;
	Solenoid     *shifter, *brace, *lock;
	AmcrestIPCAM *cam;
	Joystick     *leftJoystick, *rightJoystick;
	XboxController *xbox;
	DalekDrive *drive;
	Intake     *intake;
	Lifter     *lift;
	AHRS       *ahrs;
	SimplePath *autonPath;

	void
	RobotInit()
	{
		leftMotor     = new WPI_TalonSRX(LeftDriveMotor);
		leftSlave     = new WPI_TalonSRX(LeftSlaveMotor);
		rightMotor    = new WPI_TalonSRX(RightDriveMotor);
		rightSlave    = new WPI_TalonSRX(RightSlaveMotor);
		liftMaster    = new WPI_TalonSRX(LiftMasterMotor);
		liftSlave     = new WPI_TalonSRX(LiftSlaveMotor);
		wristMotor    = new WPI_TalonSRX(WristMotor);
		rollerMotor   = new WPI_TalonSRX(RollerMotor);

		leftJoystick  = new Joystick(LeftJoystick);
		rightJoystick = new Joystick(RightJoystick);
		xbox          = new XboxController(XboxControls);

		intake        = new Intake(WristMotor, RollerMotor, IntakeLowerLimit,
								   IntakeUpperLimit);
		ahrs          = new AHRS(SPI::Port::kMXP);
		ultraLeft     = new AnalogInput(UltrasonicLeft);
		ultraRight    = new AnalogInput(UltrasonicRight);
		cam           = new AmcrestIPCAM(IP_CAMERA, 0, 1);
		shifter       = new Solenoid(PCMID, Shifter);
		brace         = new Solenoid(PCMID, Brace);
		lock          = new Solenoid(PCMID, Lock);
		drive         = new DalekDrive(leftMotor, leftSlave, rightMotor, rightSlave);
		lift          = new Lifter(liftMaster, liftSlave, shifter, brace, lock,
								LiftLowerLimit, LiftUpperLimit, UltrasonicClimb);

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

		intake->Start();
	}

	void
	AutonomousInit() override
	{
		std::string loc = autoLocation.GetSelected();
		std::string tgt = autoTarget.GetSelected();

		gameData = frc::DriverStation::GetInstance().GetGameSpecificMessage();

		// get our starting position
		if(loc.compare(LEFT_POSITION) == 0)
			autoloc = Left;
		else if (loc.compare(CENTER_POSITION) == 0)
			autoloc = Center;
		else if (loc.compare(RIGHT_POSITION) == 0)
			autoloc = Right;
		else
			autoloc = Center;

		// compute autonomous objective, based on GameSpecificMessage
		if(tgt.compare(TARGET_SWITCH) == 0) {
			if(gameData[0] == 'L')
				autotgt = LeftSwitch;
			else
				autotgt = RightSwitch;
		}
		else if (tgt.compare(TARGET_SCALE) == 0) {
			if(gameData[1] == 'L')
				autotgt = LeftScale;
			else
				autotgt = RightScale;
		}
		else
			autotgt = BaseLine;
		autonPath = new SimplePath(autoloc, autotgt);
		autoCount = 0;
	}

	void
	AutonomousPeriodic()
	{
		autonPath->RunPath(drive, ahrs);
		frc::SmartDashboard::PutNumber("Drive Distance",
				drive->GetDistance());
		frc::SmartDashboard::PutNumber("autoCount",
				autoCount);
		autoCount++;
	}

	void
	TeleopInit()
	{
		if(!lift->AtBottom())
			lift->MoveToBottom();
	}

	void TeleopPeriodic()
	{
		bool useArcade;

		useArcade = (leftJoystick->GetZ() == -1.0);

		// Drive controls
		if (useArcade)
			drive->ArcadeDrive(leftJoystick);
		else
			drive->TankDrive(leftJoystick, rightJoystick);

		// Wrist Movement A/B button
		if(xbox->GetAButtonPressed()) {
			intake->Lower();
		} else if (xbox->GetBButtonPressed()) {
			intake->Raise();
		} else if ((xbox->GetAButtonReleased()) || (xbox->GetBButtonReleased())) {
			intake->StopWrist();
		}

		// Roller Movement X/Y button
		if(xbox->GetXButtonPressed()) {
			intake->Pull();
		} else if (xbox->GetYButtonPressed()) {
			intake->Push();
		} else if ((xbox->GetXButtonReleased()) || (xbox->GetYButtonReleased())) {
			intake->StopRoller();
		}

		//Climber Controls (Start:Climb, Back:Hold, LeftStick:Hook, RightStick:Wing)
		if(xbox->GetStickButtonPressed(frc::GenericHID::JoystickHand::kLeftHand)) {
			lift->DeployBrace();
		} else if (xbox->GetStartButtonPressed()) {
			lift->InitiateClimb();
		} else if (xbox->GetBackButtonPressed()) {
			lift->HoldPosition();
		}

		if (xbox->GetBumperPressed(frc::GenericHID::JoystickHand::kLeftHand)) {
			lift->SetOperatingMode(Lifter::ELEVATOR_MODE);
		} else if (xbox->GetBumperPressed(frc::GenericHID::JoystickHand::kRightHand)) {
			lift->SetOperatingMode(Lifter::CLIMBING_MODE);
		}

		// manual control of elevator & climber
		//Switched Right and left hand, this was only for testing
		if (xbox->GetTriggerAxis(frc::GenericHID::JoystickHand::kRightHand) > 0.05) {
			lift->ManualUp();
		} else if (xbox->GetTriggerAxis(frc::GenericHID::JoystickHand::kLeftHand) > 0.05) {
			lift->ManualDown();
		} else  {
			lift->Stop();
		}

		if(xbox->GetStickButtonPressed(frc::GenericHID::JoystickHand::kRightHand)) {
			lift->SetTalonMode(Lifter::POSITION);
			lift->Set(5500);
		}

		if(leftJoystick->GetTrigger())
			drive->SetPrecisionMode(true);
		if(rightJoystick->GetTrigger())
			drive->SetPrecisionMode(false);

		UpdateDashboard();
	}

	void TestPeriodic()
	{

	}

	void UpdateDashboard()
	{
		frc::SmartDashboard::PutNumber("Elevator Position",
				lift->GetPosition());
		frc::SmartDashboard::PutNumber("Elevator Velocity",
				lift->GetVelocity());

		frc::SmartDashboard::PutNumber("Left Drive Velocity",
				drive->GetVelocity(Motors::LeftDriveMotor));
		frc::SmartDashboard::PutNumber("Right Drive Velocity",
				drive->GetVelocity(Motors::RightDriveMotor));

		frc::SmartDashboard::PutNumber("Left Distance",
				drive->GetPosition(LeftDriveMotor));
		frc::SmartDashboard::PutNumber("Right Distance",
				drive->GetPosition(RightDriveMotor));

		frc::SmartDashboard::PutNumber("ElevUpSpeed",
				ELEVATOR_UP_SPEED);
		frc::SmartDashboard::PutNumber("ElevDownSpeed",
				ELEVATOR_DOWN_SPEED);

		frc::SmartDashboard::PutNumber("AtBottom",
				lift->AtBottom());
		frc::SmartDashboard::PutNumber("AtTop",
				lift->AtTop());

		frc::SmartDashboard::PutNumber("Distance",
				drive->GetDistance());

	}

private:
	frc::LiveWindow& m_lw = *LiveWindow::GetInstance();

	frc::SendableChooser<std::string> autoLocation;
	frc::SendableChooser<std::string> autoTarget;
	std::string gameData;
	StartPositions_t autoloc;
	TargetType_t autotgt;
	int autoCount;
};

START_ROBOT_CLASS(Robot)
