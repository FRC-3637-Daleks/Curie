/*
 * Amcrest.cpp
 *
 *  Created on: Feb 21, 2018
 *      Author: Michael
 */
#include <WPILib.h>
#include <Curie.h>
#include <Amcrest.h>

AmcrestIPCAM::AmcrestIPCAM(const char *ipaddr, int channel, int subtype)
{
	char url[MAXURLSIZE];

	sprintf(url, "http:://%s:%s@%s/cgi-bin/mjpg/video.cgi?channel=%d&subtype=%d",
			DEFUSER, DEFPASSWD, ipaddr, channel, subtype);
	cs::HttpCamera cam("AMcrestCAM", url);

	m_cs.AddCamera(cam);
	m_cs.StartAutomaticCapture(cam);
	m_channel = channel;
	m_subtype = subtype;
}

AmcrestIPCAM::~AmcrestIPCAM()
{
	// TBD: how to stop/remove instances from CameraServer
	// for now just leak memory like a sieve
}

void
AmcrestIPCAM::setChannel(int chan)
{
	m_channel = chan;
}

void
AmcrestIPCAM::setSub(int sub)
{
	m_subtype = sub;
}
