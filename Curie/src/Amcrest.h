/*
 * Amcrest.h
 *
 *  Created on: Feb 21, 2018
 *      Author: Michael
 */

#ifndef _AMCREST_H_
#define _AMCREST_H_

#include <WPILib.h>
#include <Curie.h>

#define MAXURLSIZE	1024
#define DEFUSER 	"admin"
#define DEFPASSWD	"team3637"

class AmcrestIPCAM {
	public:
		AmcrestIPCAM(const char *ipaddr, int channel, int subtype);
		~AmcrestIPCAM();

		void setChannel(int chan);
		void setSub(int subtype);

	private:
		frc::CameraServer& m_cs = *CameraServer::GetInstance();
		int m_channel;
		int m_subtype;
};

#endif /* _AMCREST_H_ */
