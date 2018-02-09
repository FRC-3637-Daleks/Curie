/*
 * IMU.h
 *
 *  Created on: Feb 6, 2018
 *      Author: Michael
 */

#ifndef _IMU_H_
#define _IMU_H_

#include <Curie.h>
#include <WPILib.h>
#include <DalekTask.h>

#define SERIAL_BAUD_RATE 	115200

class IMU : public DalekTask {
  public:

	IMU(SerialPort::Port p);
	IMU(SerialPort* p);
	IMU(SerialPort &p);
	~IMU();

	void Run();

	double GetXAccel();
	double GetYAccel();
	double GetZAccel();
	double GetYaw();
	double GetPitch();
	double GetRoll();
	int GetReadCount();

  private:
	void initializePort();
	char m_buffer[1024];
	SerialPort* m_port;
	double m_ax, m_ay, m_az, m_yaw, m_pitch, m_roll;
	int m_count;
	bool m_needFree;
};

#endif /* _IMU_H_ */
