/*
 * Lifter.h
 *
 *  Created on: Feb 10, 2018
 *      Author: Michael
 */

#ifndef _LIFTER_H_
#define _LIFTER_H_

#include <Curie.h>
#include <WPILib.h>
#include <ctre/Phoenix.h>

#define LIFTER_DEFAULT_P 0.1
#define LIFTER_DEFAULT_I 0.0
#define LIFTER_DEFAULT_D 0.0
#define LIFTER_DEFAULT_F 0.0
#define LIFTER_POSITION_TOLERANCE 0.5

class Lifter {
	public:
		typedef enum operatingMode { ELEVATOR_MODE,
				CLIMBING_MODE, NUM_OPERATING_MODES } opMode_t;
		typedef enum talonMode { PERCENT_VBUS,
				POSITION, NUM_TALON_MODES } tMode_t;

		Lifter(int motorChannel, int slaveMotorChannel, int shifter, int pot);
		Lifter(WPI_TalonSRX* motor, WPI_TalonSRX* slave, Solenoid* shifter, int pot);
		Lifter(WPI_TalonSRX& motor, WPI_TalonSRX& slave, Solenoid& shifter, int pot);
		~Lifter();

		void invertMotor(bool invert);
		void setOperatingMode(opMode_t newMode);
		opMode_t getOperatingMode();
		void Set(double value);
		void setTalonMode(tMode_t newMode);
		tMode_t getTalonMode();
		void SetP(double p);
		void SetI(double i);
		void SetD(double d);

	private:
		WPI_TalonSRX *m_master;
		WPI_TalonSRX *m_slave;
		Solenoid *m_shifter;
		AnalogInput *m_pot;
		opMode_t m_omode;
		tMode_t m_tmode;
		bool m_needFree;
		void initLifter();
};

#endif /* _LIFTER_H_ */
