#ifndef ALSA_MIDI_IN_H_
#define ALSA_MIDI_IN_H_


#include "MidiIn.h"

class AlsaMidiIn
    : public MidiIn
{
public:
	AlsaMidiIn();
	virtual ~AlsaMidiIn();

	enum MidiCapability {
		kMidiCapInput  = SND_SEQ_PORT_CAP_READ | SND_SEQ_PORT_CAP_SUBS_READ,
		kMidiCapOutput = SND_SEQ_PORT_CAP_WRITE | SND_SEQ_PORT_CAP_SUBS_WRITE,
	};

	static int  findDevice(const char *deviceName, MidiCapability desiredCapability = kMidiCapInput);
	static void printDevices();

	void connectFrom(int sourceDevice);

	virtual int getNumDescriptors();
	virtual void setupDescriptors(struct pollfd *pfds);
	virtual void callback();

private:
	snd_seq_t *_hSeq;
};

#endif // ALSA_MIDI_IN_H_
