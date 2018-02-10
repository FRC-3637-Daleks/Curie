/*
 * Curie.h
 *
 *  Created on: Jan 17, 2018
 *      Author: Michael
 */

#ifndef _CURIE_H_
#define _CURIE_H_
#define PRACTICE_BOT

enum Motors {
	/* Motor Definitions
	 */
#ifdef PRACTICE_BOT
		LeftDriveMotor = 1,
		RightDriveMotor = 3,
		WristMotor = 5,
		RollerMotor = 6,
		NumMotors = 4,
#else
		LeftDriveMotor = 4,
		LeftSlaveMotor = 3,
		RightDriveMotor = 8,
		RightSlaveMotor = 7,
		WristMotor = 1,
		RollerMotor = 2,
		WinchMotor = 5,
		NumMotors = 8,
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
