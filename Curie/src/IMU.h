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
#include <AHRS.h>
#include "SmartDashboard/SendableBuilder.h"

class IMU : public DalekTask {
  public:

	IMU(SPI::Port p);
	IMU(AHRS* p);
	IMU(AHRS& p);
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
	void initializeIMU();
	AHRS *m_ahrs;
	double m_ax, m_ay, m_az, m_yaw, m_pitch, m_roll;
	int m_count;
	bool m_needFree;
};

#endif /* _IMU_H_ */
