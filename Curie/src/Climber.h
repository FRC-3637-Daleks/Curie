/*
 * Climber.h
 *
 *  Created on: Jan 29, 2018
 *      Author: Kent
 */

#ifndef SRC_CLIMBER_H_
#define SRC_CLIMBER_H_

#include <Curie.h>
#include <WPILib.h>
#include <ctre/Phoenix.h>


class Climber {
	public:
		enum climbState { PRECLIMB, HOOKDEPLOYED, CLIMBING, HOLDING, NUM_CLIMB_STATES };

		Climber(int motorChannel, int shifter, int lock, int wings, int encoderPortA, int encoderPortB);
		Climber(WPI_TalonSRX* motor, Solenoid* shifter, Solenoid* lock, Solenoid* wings, Encoder* encoder);
		Climber(WPI_TalonSRX& motor, Solenoid& shifter, Solenoid& lock, Solenoid& wings, Encoder& encoder);
		~Climber();


		void DeployHook();
		void DeployWings();
		void DoClimb();
		void Hold();



	private:
		void InitClimber();
		WPI_TalonSRX *m_motor;
		Solenoid *m_shifter, *m_lock, *m_wings;
		Encoder *m_encoder;
		climbState state;



		bool m_needFree;
};



#endif /* SRC_CLIMBER_H_ */
