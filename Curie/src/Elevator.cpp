//By: Nithya.N

#include <Curie.h>
#include <WPILib.h>
#include <ctre/Phoenix.h>
#include <Elevator.h>

Elevator::Elevator(int elevMotorChannel, int elevSlaveMotorChannel, int potenimeter, double limitValue, double pidController)
{
	m_elevMotor     = new WPI_TalonSRX*(elevMotorChannel);
	m_elevSlaveMotor = new WPI_TalonSRX*(elevSlaveMotorChannel);
	m_potenimeter   = new Potentiometer*(potenimeter);
	m_limitValue    = new DigitalInput (limitValue);
	m_pidController = new PIDController(pidController);

	InitElevator();
	m_needFree = true;
}

Elevator::Elevator(WPI_TalonSRX* elevMotor, WPI_TalonSRX* elevSlaveMotor, int potenimeter, double limitValue, double pidController)
{

	m_elevMotor     = elevMotor;
	m_elevSlaveMotor = elevSlaveMotor;
	m_potenimeter   = potenimeter;
	m_limitValue    = limitValue;
	m_pidController = pidController;

	InitElevator();
	m_needFree = true;
}

Elevator::Elevator(WPI_TalonSRX& elevMotor, WPI_TalonSRX& elevSlaveMotor, int potenimeter, double limitValue, double pidController)
{
	m_elevMotor     = &elevMotor;
	m_elevSlaveMotor = &elevSlaveMotor;
	m_potenimeter   = &potenimeter;
	m_limitValue    = &limitValue;
	m_pidController = &pidController;

	InitElevator();
	m_needFree = true;
}

Elevator::~Elevator()
{
	if (m_needFree) {
		delete m_elevMotor;
		delete m_elevSlaveMotor;
		delete m_potenimeter;
		delete m_limitValue;
		delete m_pidController;
	}
	m_needFree = false;
	return;
}

void
Elevator::InitElevator(void) {

	    m_elevMotor->SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
		m_elevSlaveMotor->SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
		m_elevMotor->ConfigNominalOutputForward(0.0, 0);
		m_elevMotor->ConfigNominalOutputReverse(-0.0, 0);
		m_elevSlaveMotor->ConfigNominalOutputForward(0.0, 0);
		m_elevSlaveMotor->ConfigNominalOutputReverse(-0.0, 0);
		m_elevMotor->ConfigPeakOutputForward(1.0, 0);
		m_elevMotor->ConfigPeakOutputReverse(-1.0, 0);
		m_elevSlaveMotor->ConfigPeakOutputForward(1.0, 0);
		m_elevSlaveMotor->ConfigPeakOutputReverse(-1.0, 0);
		m_elevMotor->ConfigOpenloopRamp(RAMP_RATE, 0);
		m_elevSlaveMotor->ConfigOpenloopRamp(RAMP_RATE, 0);
		//m_elevMotor->ConfigSelectedFeedbackSensor(ctre::phoenix::motorcontrol::FeedbackDevice::QuadEncoder,0,0);
		//m_elevSlaveMotor->ConfigSelectedFeedbackSensor(ctre::phoenix::motorcontrol::FeedbackDevice::QuadEncoder,0,0);
		m_elevMotor->SetSensorPhase(true);
		m_elevSlaveMotor->SetSensorPhase(false);
		m_elevMotor->SetSelectedSensorPosition(0, 0, 0);
		m_elevSlaveMotor->SetSelectedSensorPosition(0, 0, 0);
		m_elevMotor->SetInverted(false);

		if(m_elevSlaveMotor) {
			m_elevSlaveMotor->Set(ctre::phoenix::motorcontrol::ControlMode::Follower, m_elevMotor->GetDeviceID());
			m_elevSlaveMotor->SetInverted(false);
			m_elevSlaveMotor->ConfigOpenloopRamp(RAMP_RATE, 0);
	}
}

void
Elevator::InvertElevatorMotor(bool isInverted)
{
	m_elevMotor->SetInverted(isInverted);
	m_elevSlaveMotor->SetInverted(isInverted);
	isInverted = false;
}

void
Elevator::MoveElevator(ControlMode mode, double value)
{
	m_elevMotor->Set(mode, value);
}

void
Elevator::MaintainElevatorPosistion(ControlMode mode, double value)
{
	m_elevMotor->Set(mode, value);
}

void
Elevator::StopElevator(void)
{
	//Working on elevator control loop
	//Working on PID control loop
}

void
Elevator::SetP(double p)
{
	m_pidController->SetP(p);
}

void
Elevator::SetI(double i)
{
	m_pidController->SetI(i);
}

void
Elevator::SetD(double d)
{
	m_pidController->SetD(d);
}



