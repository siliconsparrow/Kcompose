/*
 * PcMidiKeyboard.h
 *
 *  Created on: 22 Mar 2021
 *      Author: adam
 */

#ifndef PCMIDIKEYBOARD_H_
#define PCMIDIKEYBOARD_H_

#include <alsa/asoundlib.h>

class PcMidiKeyboard
{
public:
	enum {
		kMidiChannel = 0,
		kDefaultVelocity = 100,
	};

	PcMidiKeyboard();

	int getClientId();

	bool keypress(int keycode);
	bool keyrelease(int keycode);

private:
	snd_seq_t *_hSeq;
	int        _portId;

	static unsigned mapKeyToNote(int keycode);

	void     sendMidiNoteEvent(snd_seq_event_type_t eventType, unsigned noteNum);
};

#endif /* PCMIDIKEYBOARD_H_ */
