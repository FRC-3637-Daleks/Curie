#include <Curie.h>
#include <WPILib.h>
#include <ctre/Phoenix.h>

#include <Lifter.h>

Lifter::Lifter(int masterChannel, int slaveChannel,
		int shifterChannel, int braceChannel, int lockChannel,
		int lowerChannel, int upperChannel, int ultraChannel)
{
	m_master  = new WPI_TalonSRX(masterChannel);
	m_slave   = new WPI_TalonSRX(slaveChannel);
	m_shifter = new Solenoid(PCMID, shifterChannel);
	m_brace   = new Solenoid(PCMID, braceChannel);
	m_lock    = new Solenoid(PCMID, lockChannel);
	m_lower   = new DigitalInput(lowerChannel);
	m_upper   = new DigitalInput(upperChannel);
	m_ultra   = new AnalogInput(ultraChannel);

	m_omode   = ELEVATOR_MODE;
	m_tmode   = PERCENT_VBUS;
	m_ready   = false;  // track if ready to use position control
	m_state   = PRECLIMB;
	m_climbHeight = DEFAULT_CLIMB_HEIGHT;

	initLifter();
	m_needFree = true;
}

Lifter::Lifter(WPI_TalonSRX *master, WPI_TalonSRX *slave,
		Solenoid *shifter, Solenoid *brace, Solenoid *lock,
		int lowerChannel, int upperChannel, int ultraChannel)
{
	m_master  = master;
	m_slave   = slave;
	m_shifter = shifter;
	m_brace   = brace;
	m_lock    = lock;
	m_lower   = new DigitalInput(lowerChannel);
	m_upper   = new DigitalInput(upperChannel);
	m_ultra   = new AnalogInput(ultraChannel);

	m_omode   = ELEVATOR_MODE;
	m_tmode   = PERCENT_VBUS;
	m_ready   = false;
	m_state   = PRECLIMB;
	m_climbHeight = DEFAULT_CLIMB_HEIGHT;
	initLifter();
	m_needFree = false;
}

Lifter::Lifter(WPI_TalonSRX &master, WPI_TalonSRX &slave,
		Solenoid &shifter, Solenoid &brace, Solenoid &lock,
		int lowerChannel, int upperChannel, int ultraChannel)
{
	m_master  = &master;
	m_slave   = &slave;
	m_shifter = &shifter;
	m_brace   = &brace;
	m_lock    = &lock;
	m_lower   = new DigitalInput(lowerChannel);
	m_upper   = new DigitalInput(upperChannel);
	m_ultra   = new AnalogInput(ultraChannel);

	m_omode   = ELEVATOR_MODE;
	m_tmode   = PERCENT_VBUS;
	m_ready   = false;
	m_state   = PRECLIMB;
	m_climbHeight = DEFAULT_CLIMB_HEIGHT;
	initLifter();
	m_needFree = false;
}

Lifter::~Lifter()
{
	if(m_needFree) {
		delete m_master;
		delete m_slave;
		delete m_shifter;
		delete m_brace;
		delete m_lock;
	}
	delete m_upper;
	delete m_lower;
	delete m_ultra;
	m_needFree = false;
}

void
Lifter::initLifter()
{
	m_master->SetNeutralMode(NeutralMode::Brake);
	m_master->ConfigNominalOutputForward(0.0, CANTimeoutMs);
	m_master->ConfigNominalOutputReverse(-0.0, CANTimeoutMs);
	m_master->ConfigPeakOutputForward(1.0, CANTimeoutMs);
	m_master->ConfigPeakOutputReverse(-1.0, CANTimeoutMs);
	m_master->ConfigOpenloopRamp(RAMP_RATE, CANTimeoutMs);
	m_master->ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder,
				PIDLoopIdx, CANTimeoutMs);
	m_master->SetInverted(false);
	m_master->SetSensorPhase(false);
	m_master->SelectProfileSlot(PIDSlotIdx, PIDLoopIdx);
	m_master->Config_kF(PIDSlotIdx, LIFTER_DEFAULT_F, CANTimeoutMs);
	m_master->Config_kP(PIDSlotIdx, LIFTER_DEFAULT_P, CANTimeoutMs);
	m_master->Config_kI(PIDSlotIdx, LIFTER_DEFAULT_I, CANTimeoutMs);
	m_master->Config_kD(PIDSlotIdx, LIFTER_DEFAULT_D, CANTimeoutMs);
	m_slave->Set(ControlMode::Follower, m_master->GetDeviceID());
	m_slave->SetInverted(false);
	m_slave->SetSensorPhase(false);
	m_slave->ConfigOpenloopRamp(RAMP_RATE, CANTimeoutMs);
	m_shifter->Set(ELEVATOR_MODE);
	if(AtBottom())
		ZeroPosition();
	return;
}

void
Lifter::InvertMotor(bool invert)
{
	m_master->SetInverted(invert);
	m_slave->SetInverted(invert);
	return;
}

void
Lifter::ZeroPosition()
{
	//Zero encoder, and set ready to do position control
	//Look up SetSelectedSensorPosition (Talon XRS)
	m_master->SetSelectedSensorPosition(0, PIDLoopIdx, CANTimeoutMs);
	m_ready = true;
}

void
Lifter::ManualDown(void)
{
	double speed;

	if(GetTalonMode() != PERCENT_VBUS)
		return;

	if(GetOperatingMode() == ELEVATOR_MODE)
		speed = (AtBottom() ? 0.0 : ELEVATOR_DOWN_SPEED);
	else
		speed = (AtBottom() ? 0.0 : CLIMB_DOWN_SPEED);

	Set(speed);
	return;
}

void
Lifter::ManualUp(void)
{
	double speed;

	if(GetTalonMode() != PERCENT_VBUS)
		return;

	if(GetOperatingMode() == ELEVATOR_MODE)
		speed = (AtTop() ? 0.0 : ELEVATOR_UP_SPEED);
	else
		speed = (AtTop() ? 0.0 : CLIMB_UP_SPEED);

	Set(speed);
	return;
}

void
Lifter::Stop(void)
{
	if(GetTalonMode() != PERCENT_VBUS)
		SetTalonMode(PERCENT_VBUS);
	Set(0.0);
}

void
Lifter::SetOperatingMode(opMode_t newMode)
{
	// set the operating mode, and shift the lifter into the correct gear
	switch(newMode) {
	case ELEVATOR_MODE:
		m_omode = newMode;
		m_shifter->Set(false);
		break;
	case CLIMBING_MODE:
		m_omode = newMode;
		m_shifter->Set(true);
		break;
	default:
		break;
	}
}

Lifter::opMode_t
Lifter::GetOperatingMode()
{
	return m_omode;
}

void
Lifter::Set(double value)
{
	m_slave->Set(ControlMode::Follower, 3);
	switch(m_tmode) {
	case PERCENT_VBUS:
		m_master->Set(ControlMode::PercentOutput, value);
		break;
	case POSITION:
		m_master->Set(ControlMode::Position, value);
		break;
	default:
		break;
	}
	return;
}

void
Lifter::SetTalonMode(tMode_t newMode)
{
	m_tmode = newMode;
}

Lifter::tMode_t
Lifter::GetTalonMode()
{
	return m_tmode;
}

int
Lifter::GetPosition()
{
	return m_master->GetSensorCollection().GetQuadraturePosition();
}

double
Lifter::GetVelocity()
{
	return m_master->GetSensorCollection().GetQuadratureVelocity();
}

void
Lifter::SetP(double p)
{
	m_master->SelectProfileSlot(PIDSlotIdx, PIDLoopIdx);
	m_master->Config_kP(PIDSlotIdx, p, CANTimeoutMs);
}

void
Lifter::SetI(double i)
{
	m_master->SelectProfileSlot(PIDSlotIdx, PIDLoopIdx);
	m_master->Config_kI(PIDSlotIdx, i, CANTimeoutMs);
	return;
}

void
Lifter::SetD(double d)
{
	m_master->SelectProfileSlot(PIDSlotIdx, PIDLoopIdx);
	m_master->Config_kD(PIDSlotIdx, d, CANTimeoutMs);
}

bool
Lifter::AtBottom()
{
	if(m_lower->Get() == 0) {
		// ZeroPosition();
		return true;
	}
	return false;
}

bool
Lifter::AtTop()
{
	return (m_upper->Get() == 0);
}

bool
Lifter::MoveToBottom()
{
	if(AtBottom())
		return true;
	ManualDown();
	return false;
}

void
Lifter::DeployHook(void)
{
	if(m_state != BRACED)
		return;
	SetOperatingMode(ELEVATOR_MODE);
	ManualUp();
	if(AtTop())
		m_state = HOOKDEPLOYED;
}

void
Lifter::DeployBrace(void)
{
	if(m_state != PRECLIMB)
		return;
	if(GetOperatingMode() == CLIMBING_MODE) {
		m_brace->Set(true);
		m_state = BRACED;
	}
}

float
Lifter::GetHeight()
{
	return (m_ultra->GetVoltage()*INCHES_PER_VOLTS);
}

void
Lifter::InitiateClimb(void)
{
	if(m_state != HOOKDEPLOYED)
		return;
	m_state = CLIMBING;
	SetOperatingMode(CLIMBING_MODE);
	SetTalonMode(PERCENT_VBUS);

	if(GetHeight() > m_climbHeight) {
		HoldPosition();
		return;
	}

	Set(CLIMB_UP_SPEED);
	return;
}

void
Lifter::HoldPosition(void)
{
	//if(m_state == CLIMBING) {
		m_state = HOLDING;
		m_lock->Set(true);
		Set(0.0);
	//}
}
