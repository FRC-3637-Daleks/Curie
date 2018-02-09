/*
 * IMU.cpp
 *
 *  Created on: Feb 6, 2018
 *      Author: Michael
 */
#include <Curie.h>
#include <WPILib.h>
#include <IMU.h>

IMU::IMU(SerialPort::Port p)
{
	m_port = new SerialPort(SERIAL_BAUD_RATE, p, 8, SerialPort::kParity_None,
			SerialPort::kStopBits_One);
	m_needFree = true;
	initializePort();
	return;
}

IMU::IMU(SerialPort* p)
{
	m_port = p;
	m_needFree = false;
	initializePort();
}

IMU::IMU(SerialPort &p)
{
	m_port = &p;
	m_needFree = false;
	initializePort();
}

IMU::~IMU()
{
	if(m_needFree)
		delete m_port;
	return;
}

void
IMU::Run()
{
	int rcnt;
	if((rcnt = m_port->Read(m_buffer, 1024)) > 0) {
		// if there was data available, parse the string
		// and store the results in the local variables
		m_count += rcnt;
	}
}

void
IMU::initializePort()
{
	m_port->SetFlowControl(SerialPort::FlowControl::kFlowControl_XonXoff);
	m_port->EnableTermination();
	m_port->SetTimeout(10);
	m_port->SetReadBufferSize(1024);
	m_port->SetWriteBufferSize(1024);
	m_ax = m_ay = m_az = 0.0;
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
