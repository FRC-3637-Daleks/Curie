/*
 * IMU.cpp
 *
 *  Created on: Feb 6, 2018
 *      Author: Michael
 */
#include <Curie.h>
#include <WPILib.h>
#include <IMU.h>

IMU::IMU(SPI::Port p)
{
	m_ahrs = new AHRS(SPI::Port::kMXP);
	m_needFree = true;
	initializeIMU();
}

IMU::IMU(AHRS* p)
{
	m_ahrs = p;
	m_needFree = false;
	initializeIMU();
}

IMU::IMU(AHRS &p)
{
	m_ahrs = &p;
	m_needFree = false;
	initializeIMU();
}

IMU::~IMU()
{
	if(m_needFree)
		delete m_ahrs;
	return;
}

void
IMU::Run()
{
	m_yaw   = m_ahrs->GetYaw();
	m_pitch = m_ahrs->GetPitch();
	m_roll  = m_ahrs->GetRoll();
	m_ax    = m_ahrs->GetRawAccelX();
	m_ay    = m_ahrs->GetRawAccelY();
	m_az    = m_ahrs->GetRawAccelZ();
	m_count++;

	frc::SmartDashboard::PutNumber("IMU Heading",
			m_ahrs->GetFusedHeading());
}

void
IMU::initializeIMU()
{
	m_ax  = m_ay    = m_az   = 0.0;
	m_yaw = m_pitch = m_roll = 0.0;

	m_count = 0;
}

double
IMU::GetXAccel()
{
	return m_ax;
}

double
IMU::GetYAccel()
{
	return m_ay;
}

double
IMU::GetZAccel()
{
	return m_az;
}

double
IMU::GetYaw()
{
	return m_yaw;
}

double
IMU::GetPitch()
{
	return m_pitch;
}

double
IMU::GetRoll()
{
	return m_roll;
}

int
IMU::GetReadCount()
{
	return m_count;
}
