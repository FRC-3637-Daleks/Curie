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

class Lifter {
	public:
		typedef enum operatingMode { ELEVATOR_MODE,
				CLIMBING_MODE, NUM_OPERATING_MODES } opMode_t;
		typedef enum talonMode { PERCENT_VBUS,
				POSITION, NUM_TALON_MODES } tMode_t;
		Lifter(int motorChannel, int slaveMotorChannel, int shifter);
		Lifter(WPI_TalonSRX* motor, WPI_TalonSRX* slave, Solenoid* shifter);
		Lifter(WPI_TalonSRX& motor, WPI_TalonSRX& slave, Solenoid& shifter);
		~Lifter();

		void setOperatingMode(opMode_t newMode);
		opMode_t getOperatingMode();
		void Set(double value);
		void setTalonMOde(tMode_t newMode);
		tMode_t getTalonMode();

	private:
		WPI_TalonSRX *m_master;
		WPI_TalonSRX *m_slave;
		Solenoid *m_shifter;
		opMode_t m_omode;
		tMode_t m_tmode;
		bool m_needFree;
		void initLifter();
};

#endif /* _LIFTER_H_ */
