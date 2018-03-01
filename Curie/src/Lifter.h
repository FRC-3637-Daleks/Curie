#ifndef _LIFTER_H_
#define _LIFTER_H_

#include <Curie.h>
#include <WPILib.h>
#include <ctre/Phoenix.h>

#define LIFTER_DEFAULT_P 0.125
#define LIFTER_DEFAULT_I 0.0
#define LIFTER_DEFAULT_D 0.0
#define LIFTER_DEFAULT_F 0.125
#define LIFTER_POSITION_TOLERANCE 0.5

#define ELEVATOR_UP_SPEED  -0.2
#define ELEVATOR_DOWN_SPEED 0.2

#define DEPLOYHOOK_POSITION  200
#define CLIMB_DOWN_SPEED	 -0.2
#define CLIMB_UP_SPEED      0.2
#define DEFAULT_CLIMB_HEIGHT 15

class Lifter {
	public:
		typedef enum operatingMode { ELEVATOR_MODE,
				CLIMBING_MODE, NUM_OPERATING_MODES } opMode_t;
		typedef enum talonMode { PERCENT_VBUS,
				POSITION, NUM_TALON_MODES } tMode_t;
		enum climbState { PRECLIMB, BRACED, HOOKDEPLOYED, CLIMBING, HOLDING};

		Lifter(int motorChannel, int slaveMotorChannel,
				int shifterChannel, int braceChannel, int lockChannel,
				int lowerChannel, int upperChannel, int ultraChannel);
		Lifter(WPI_TalonSRX* motor, WPI_TalonSRX* slave,
				Solenoid* shifter, Solenoid* brace, Solenoid* lock,
				int lowerChannel, int upperChannel, int ultraChannel);
		Lifter(WPI_TalonSRX& motor, WPI_TalonSRX& slave,
				Solenoid& shifter, Solenoid& brace, Solenoid& lock,
				int lowerChannel, int upperChannel, int ultraChannel);
		~Lifter();

		void InvertMotor(bool invert);
		void Set(double value);
		void SetOperatingMode(opMode_t newMode);
		void SetTalonMode(tMode_t newMode);
		opMode_t GetOperatingMode();
		tMode_t GetTalonMode();
		void ManualUp();
		void ManualDown();
		bool MoveToBottom();
		void Stop();
		bool AtTop();
		bool AtBottom();
		void SetP(double p);
		void SetI(double i);
		void SetD(double d);
		void ZeroPosition();
		int GetPosition();
		double GetVelocity();
		void DeployBrace();
		void DeployHook();
		void InitiateClimb();
		void HoldPosition();
		float GetHeight();

	private:
		WPI_TalonSRX *m_master;
		WPI_TalonSRX *m_slave;
		Solenoid *m_shifter;
		Solenoid *m_brace;
		Solenoid *m_lock;
		DigitalInput *m_upper;
		DigitalInput *m_lower;
		AnalogInput  *m_ultra;
		opMode_t m_omode;
		tMode_t m_tmode;
		bool m_needFree;
		bool m_ready;
		climbState m_state;
		double m_climbHeight;
		void initLifter();
};

#endif /* _LIFTER_H_ */
