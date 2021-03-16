#ifndef ALSA_MIDI_IN_H_
#define ALSA_MIDI_IN_H_

#include <alsa/asoundlib.h>

class AlsaMidiIn
{
public:
	AlsaMidiIn();
	~AlsaMidiIn();

	class Sink
	{
	public:
		virtual ~Sink() { }
		virtual void midiEvent(snd_seq_event_t *ev) = 0;
	};

	enum MidiCapability {
		kMidiCapInput  = SND_SEQ_PORT_CAP_READ | SND_SEQ_PORT_CAP_SUBS_READ,
		kMidiCapOutput = SND_SEQ_PORT_CAP_WRITE | SND_SEQ_PORT_CAP_SUBS_WRITE,
	};
	int  findDevice(const char *deviceName, MidiCapability desiredCapability = kMidiCapInput);
	void connectFrom(int sourceDevice);
	void printDevices();

	int getNumDescriptors();
	void setupDescriptors(struct pollfd *pfds);

	void setSink(Sink *s) { _sink = s; }

	void callback();

private:
	snd_seq_t *_hSeq;
	Sink      *_sink;
};

#endif // ALSA_MIDI_IN_H_
