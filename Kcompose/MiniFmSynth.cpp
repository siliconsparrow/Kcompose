/*
 * MiniFmSynth.cpp
 *
 *  Created on: 17 Mar 2021
 *      Author: adam
 */

#include "MiniFmSynth.h"
//#include <stdio.h>
//#include <stdlib.h>
//#include <math.h>

#define GAIN 5000.0

// Example presets:
MiniFmSynth::Preset g_presetHarpsichord(7.8, 3, 5, 24, 0.01, 0.8, 0.0, 0.1);
MiniFmSynth::Preset g_presetBell(3.5, 7, 9, 0, 0.01, 0.2, 0.3, 1.5);
MiniFmSynth::Preset g_presetOboe(0.7, 1, 3, 24, 0.05, 0.3, 0.8, 0.2);

MiniFmSynth::Preset::Preset(double modulation, int harmonic, int subharmonic, int transpose,
			                double attack, double decay, double sustain, double release)
	: _modulation(modulation)
	, _harmonic(harmonic)
	, _subharmonic(subharmonic)
	, _transpose(transpose)
	, _attack(attack)
	, _decay(decay)
	, _sustain(sustain)
	, _release(release)
{
}

MiniFmSynth::MiniFmSynth()
	: _preset(7.8, 3, 5, 24, 0.01, 0.8, 0.0, 0.1)
{
#if 0
	pitch = 0;
	for(int l1 = 0; l1 < POLY; note_active[l1++] = 0);
#endif // 0
}

MiniFmSynth::~MiniFmSynth()
{
}

// Called when a MIDI event arrives.
void MiniFmSynth::midiEvent(snd_seq_event_t *ev)
{
    //int              l1;

    switch (ev->type) {
#if 0
        case SND_SEQ_EVENT_PITCHBEND:
            pitch = (double)ev->data.control.value / 8192.0;
            break;
        case SND_SEQ_EVENT_CONTROLLER:
            if (ev->data.control.param == 1) {
                modulation = (double)ev->data.control.value / 10.0;
            }
            break;
#endif // 0
        case SND_SEQ_EVENT_NOTEON:
        	printf("NOTE %d (%d)\n", (int)ev->data.note.note, (int)ev->data.note.velocity);
#if 0
            for (l1 = 0; l1 < POLY; l1++) {
                if (!note_active[l1]) {
                    note[l1] = ev->data.note.note;
                    velocity[l1] = ev->data.note.velocity / 127.0;
                    env_time[l1] = 0;
                    gate[l1] = 1;
                    note_active[l1] = 1;
                    break;
                }
            }
#endif // 0
            break;

#if 0
        case SND_SEQ_EVENT_NOTEOFF:
            for (l1 = 0; l1 < POLY; l1++) {
                if (gate[l1] && note_active[l1] && (note[l1] == ev->data.note.note)) {
                    env_time[l1] = 0;
                    gate[l1] = 0;
                }
            }
            break;
#endif // 0
    }
}

// Called when the audio output wants more data.
void MiniFmSynth::getFrame(short *buffer, snd_pcm_sframes_t nFrames)
{
	//    int l1, l2;
	//    double dphi, dphi_mod, f1, f2, f3, freq_note, sound;

	for(int i = 0; i < nFrames * 2; i++) {
		buffer[i] = (i & 63) * 500;
	}

	#if 0
	    for (l2 = 0; l2 < POLY; l2++) {
	        if (note_active[l2]) {
	            f1 = 8.176 * exp((double)(transpose+note[l2]-2)*log(2.0)/12.0);
	            f2 = 8.176 * exp((double)(transpose+note[l2])*log(2.0)/12.0);
	            f3 = 8.176 * exp((double)(transpose+note[l2]+2)*log(2.0)/12.0);
	            freq_note = (pitch > 0) ? f2 + (f3-f2)*pitch : f2 + (f2-f1)*pitch;
	            dphi = M_PI * freq_note / 22050.0;
	            dphi_mod = dphi * (double)harmonic / (double)subharmonic;
	            for (l1 = 0; l1 < nframes; l1++) {
	                phi[l2] += dphi;
	                phi_mod[l2] += dphi_mod;
	                if (phi[l2] > 2.0 * M_PI) phi[l2] -= 2.0 * M_PI;
	                if (phi_mod[l2] > 2.0 * M_PI) phi_mod[l2] -= 2.0 * M_PI;
	                sound = GAIN * envelope(&note_active[l2], gate[l2], &env_level[l2], env_time[l2], attack, decay, sustain, release)
	                             * velocity[l2] * sin(phi[l2] + modulation * sin(phi_mod[l2]));
	                env_time[l2] += 1.0 / 44100.0;
	                _buf[2 * l1] += sound;
	                _buf[2 * l1 + 1] += sound;
	            }
	        }
	    }
	#endif // 0
}

#if 0
double envelope(int *note_active, int gate, double *env_level, double t, double attack, double decay, double sustain, double release) {

    if (gate)  {
        if (t > attack + decay) return(*env_level = sustain);
        if (t > attack) return(*env_level = 1.0 - (1.0 - sustain) * (t - attack) / decay);
        return(*env_level = t / attack);
    } else {
        if (t > release) {
            if (note_active) *note_active = 0;
            return(*env_level = 0);
        }
        return(*env_level * (1.0 - t / release));
    }
}
#endif // 0


#if 0
/* Read events from writeable port and route them to readable port 0  */
/* if NOTEON / OFF event with note < split_point. NOTEON / OFF events */
/* with note >= split_point are routed to readable port 1. All other  */
/* events are routed to both readable ports.                          */
void midi_route(snd_seq_t *seq_handle, int out_ports[], int split_point) {

  snd_seq_event_t *ev;

  do {
    snd_seq_event_input(seq_handle, &ev);
    snd_seq_ev_set_subs(ev);
    snd_seq_ev_set_direct(ev);
    if ((ev->type == SND_SEQ_EVENT_NOTEON)||(ev->type == SND_SEQ_EVENT_NOTEOFF)) {
      if (ev->data.note.note < split_point) {
        snd_seq_ev_set_source(ev, out_ports[0]);
      } else {
        snd_seq_ev_set_source(ev, out_ports[1]);
      }
      snd_seq_event_output_direct(seq_handle, ev);
    } else {
      snd_seq_ev_set_source(ev, out_ports[0]);
      snd_seq_event_output_direct(seq_handle, ev);
      snd_seq_ev_set_source(ev, out_ports[1]);
      snd_seq_event_output_direct(seq_handle, ev);
    }
    snd_seq_free_event(ev);
  } while (snd_seq_event_input_pending(seq_handle, 0) > 0);
}
#endif // 0
