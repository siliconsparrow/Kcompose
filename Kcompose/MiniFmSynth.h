/*
 * MiniFmSynth.h
 *
 *  Created on: 17 Mar 2021
 *      Author: adam
 */

#ifndef MINIFMSYNTH_H_
#define MINIFMSYNTH_H_

#include "AlsaMidiIn.h"
#include "AlsaPlayback.h"

class MiniFmSynth
	: public MidiIn::Sink
	, public AlsaPlayback::Source
{
public:
	MiniFmSynth();
	virtual ~MiniFmSynth();

	enum { kNumPresets = 3 };

	class Preset
	{
	public:
		Preset(const char *name, double modulation, int harmonic, int subharmonic, int transpose,
			   double attack, double decay, double sustain, double release);

		const char *_name;
		double _modulation;
		double _attack;
		double _decay;
		double _sustain;
		double _release;
		int    _harmonic;
		int    _subharmonic;
		int    _transpose;
	};

	virtual void midiEvent(snd_seq_event_t *ev);
	virtual void getFrame(short *buffer, snd_pcm_sframes_t nFrames);

private:
	enum { POLY = 10 };

	Preset _preset;
	double phi[POLY];
	double phi_mod[POLY];
	double pitch;
	double velocity[POLY];
	double env_time[POLY];
	double env_level[POLY];
	int note[POLY];
	int gate[POLY];
	int note_active[POLY];

	double envelope(int *note_active, int gate, double *env_level, double t, double attack, double decay, double sustain, double release);
};

extern MiniFmSynth::Preset g_Presets[];

#endif /* MINIFMSYNTH_H_ */
