/*
 * PcKeyboardMidiIn.h
 *
 *  Created on: 18 Mar 2021
 *      Author: adam
 */

#ifndef PCKEYBOARDMIDIIN_H_
#define PCKEYBOARDMIDIIN_H_

#include "MidiIn.h"

class PcKeyboardMidiIn
    : public MidiIn
{
public:
	PcKeyboardMidiIn();
	virtual ~PcKeyboardMidiIn();

	virtual int getNumDescriptors();
	virtual void setupDescriptors(struct pollfd *pfds);
	virtual void callback();

private:
	int _fd;
};

#endif /* PCKEYBOARDMIDIIN_H_ */
