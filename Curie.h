/*
 * Curie.h
 *
 *  Created on: Jan 17, 2018
 *      Author: Michael
 */

#ifndef _CURIE_H_
#define _CURIE_H_
// #define PRACTICE_BOT
// #define USB_CAMERA

#define PCMID 12
#define IP_CAMERA "10.36.37.34"
#define ENCODER_TICKS_PER_INCH 215
#define INCHES_PER_VOLTS 40.33

enum Motors {
	/* Motor Definitions
	 */
#ifdef PRACTICE_BOT
	LeftDriveMotor  = 1,
	RightDriveMotor = 3,
	WristMotor      = 5,
	RollerMotor     = 6,
#else
	LeftDriveMotor  = 4,
	LeftSlaveMotor  = 5,
	RightDriveMotor = 7,
	RightSlaveMotor = 8,
	WristMotor      = 1,
	RollerMotor     = 2,
	LiftMasterMotor = 3,
	LiftSlaveMotor  = 6,
#endif
};

enum Joysticks {
	/* User input devices
	 */
	LeftJoystick = 0,
	RightJoystick,
	XboxControls,
	NumJoysticks,
};

enum Solenoids {
	Shifter = 0,
	Brace   = 1,
	Lock    = 2,
};

enum DigitalIO {
	LiftLowerLimit = 1,
	LiftUpperLimit = 0,
	IntakeLowerLimit   = 2,
	IntakeUpperLimit   = 3,
};

enum AnalogIO {
	UltrasonicLeft  = 2,
	UltrasonicRight = 3,
	UltrasonicClimb = 0,
	MagnetoPot      = 1,
};

enum CANConstants {
	/* Which PID slot to pull gains from.  Starting 2018, you can choose
	 * from 0,1,2 or 3.  Only the first two (0,1) are visible in web-based configuration.
	 */
	PIDSlotIdx = 0,

	/* Talon SRX/Victor SPX will supported multiple (cascaded) PID loops.
	 * For now we just want the primary one.
	 */
	PIDLoopIdx = 0,

	/*
	 * set to zero to skip waiting for confirmation, set to nonzero to wait
	 * and report to DS if action fails.
	 */
	CANTimeoutMs = 10,
};

#define LEFT_POSITION	"1"
#define CENTER_POSITION	"2"
#define RIGHT_POSITION	"3"

#define TARGET_SWITCH    "1"
#define TARGET_SCALE     "2"
#define TARGET_AUTOLINE  "3"

#define ENCODER_TICKS_PER_REV 4096
#define WHEEL_DIAMETER_INCHES 6.0
#define AT_WHEEL_RATIO (26.0/22.0)

#define RPM_THRESHOLD 300
#define RAMP_RATE 0.2

//Depth front to back of robot base (in inches)
#define ROBOT_BASE_DEPTH 36.0


#endif /* _CURIE_H_ */