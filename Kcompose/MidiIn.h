// Base class for MIDI input devices.
// Could be a keyboard, virtual device or a MIDI file player.

#ifndef MIDI_IN_H_
#define MIDI_IN_H_

#include <alsa/asoundlib.h>

class MidiIn
{
public:
	class Sink
	{
	public:
		virtual ~Sink() { }
		virtual void midiEvent(snd_seq_event_t *ev) = 0;
	};

	MidiIn() : _sink(0) { }
	virtual ~MidiIn() { }

	virtual int getNumDescriptors() = 0;
	virtual void setupDescriptors(struct pollfd *pfds) = 0;
	virtual void callback() = 0;

	void setSink(Sink *s) { _sink = s; }

protected:
	Sink      *_sink;
};

#endif // MIDI_IN_H_
