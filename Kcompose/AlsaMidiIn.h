#ifndef ALSA_MIDI_IN_H_
#define ALSA_MIDI_IN_H_

#include <alsa/asoundlib.h>

class AlsaMidiIn
{
public:
	AlsaMidiIn();
	~AlsaMidiIn();

	enum MidiCapability {
		kMidiCapInput  = SND_SEQ_PORT_CAP_READ | SND_SEQ_PORT_CAP_SUBS_READ,
		kMidiCapOutput = SND_SEQ_PORT_CAP_WRITE | SND_SEQ_PORT_CAP_SUBS_WRITE,
	};
	int  findDevice(const char *deviceName, MidiCapability desiredCapability = kMidiCapInput);
	void connectFrom(int sourceDevice);
	void printDevices();

	int getNumDescriptors();
	void setupDescriptors(struct pollfd *pfds);

	void callback();

private:
	snd_seq_t *seq_handle;

};

#endif // ALSA_MIDI_IN_H_
