/*
 * Curie.h
 *
 *  Created on: Jan 17, 2018
 *      Author: Michael
 */

#ifndef _CURIE_H_
#define _CURIE_H_

// #define PRACTICE_BOT // uncomment if on simple drivebase
// #define USB_CAMERA   // uncomment if using usb camera

#define IP_CAMERA_ADDRESS "10.36.37.33"
#define PCMID 12

enum Motors {
	/* Motor Definitions
	 */
#ifdef PRACTICE_BOT
		LeftDriveMotor = 1,
		RightDriveMotor = 3,
		WristMotor = 5,
		RollerMotor = 6,
#else
		LeftDriveMotor = 4,
		LeftSlaveMotor = 5,
		RightDriveMotor = 7,
		RightSlaveMotor = 8,
		WristMotor = 2,
		RollerMotor = 1,
		LiftMasterMotor = 3,
		LiftSlaveMotor = 6,
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
		Wings,
		Brace,
		Lock,
};

enum DigitalIO {
		ElevatorUpperLimit = 0,
		ElevatorLowerLimit,
		IntakeLowerLimit,
		IntakeUpperLimit,
		IntakeProximity,
};

enum AnalogIO {
		UltrasonicLeft = 1,
		UltrasonicRight,
		UltrasonicClimb,
};

enum Constants {
		/* Which PID slot to pull gains from.  Starting 2018, you can choose
		 * from 0,1,2 or 3.  Only the first two (0,1) are visible in web-based configuration.
		 */
		DriveSlotIdx = 0,
		ElevatorSlotIdx,
		WristSlotIdx,

		/* Talon SRX/ Victor SPX will supported multiple (cascaded) PID loops.
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

#endif /* _CURIE_H_ */
