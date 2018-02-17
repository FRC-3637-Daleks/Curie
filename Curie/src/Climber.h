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
#include <Lifter.h>

class Climber {
	public:
		enum climbState { PRECLIMB, HOOKDEPLOYED, CLIMBING, HOLDING, NUM_CLIMB_STATES };

		Climber(Lifter *lift, int brace, int lock, int wings,
				int ultra);
		Climber(Lifter *lift, Solenoid *brace, Solenoid* lock, Solenoid* wings,
				AnalogInput* ultra);
		Climber(Lifter& lift, Solenoid& brace, Solenoid& lock, Solenoid& wings,
				AnalogInput& ultra);
		~Climber();

		void DeployHook();
		void DeployBrace();
		void DeployWings();
		void DoClimb();
		void Hold();
		void setClimbLimit(int value);

	private:
		void InitClimber();
		Lifter *m_lifter;
		Solenoid *m_brace, *m_lock, *m_wings;
		AnalogInput *m_ultra;
		int m_climbLimit;
		climbState state;
		bool m_needFree;
};

#endif /* SRC_CLIMBER_H_ */
