
// A good place to get example code and such is from the ALSA GitHub repo. Look at
// all the alsa-utils for examples on how to use the library.
// https://github.com/alsa-project

// I'm starting with miniFMsynth by Matthias Nagorni and adapting the basic structure to my own audio filter system

#include "AlsaMidiIn.h"
#include "AlsaPlayback.h"
//#include <stdio.h>
//#include <stdlib.h>
#include <alsa/asoundlib.h>
//#include <math.h>
#include <string>
#include <iostream>


#if 0
#define POLY 10
#define GAIN 5000.0
#endif // 0


#if 0
double phi[POLY], phi_mod[POLY], pitch, modulation, velocity[POLY], attack, decay, sustain, release, env_time[POLY], env_level[POLY];
int harmonic, subharmonic, transpose, note[POLY], gate[POLY], note_active[POLY];
#endif // 0



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



int main (int argc, char *argv[])
{
	try
	{
	#if 0
		int l1;

		if (argc < 10) {
			fprintf(stderr, "miniFMsynth <device> <FM> <harmonic> <subharmonic> <transpose> <a> <d> <s> <r>\n");
			exit(1);
		}
		modulation = atof(argv[2]);
		harmonic = atoi(argv[3]);
		subharmonic = atoi(argv[4]);
		transpose = atoi(argv[5]);
		attack = atof(argv[6]);
		decay = atof(argv[7]);
		sustain = atof(argv[8]);
		release = atof(argv[9]);
		pitch = 0;
	#endif // 0

		// Open a playback channel.
		AlsaPlayback pcmOut("default");

		// Open a MIDI device.
		AlsaMidiIn midiIn;

		// TEST - print available MIDI devices.
		//midiIn.printDevices();

		// Connect MIDI to the Vortex keyboard.
		int srcDevice = midiIn.findDevice("Vortex");
		if(srcDevice < 0) {
			// TODO: Make a keyboard MIDI source object for when the hardware keyboard is not available.
			printf("Failed to connect\n");
			return 1;
		}
		midiIn.connectFrom(srcDevice);

		// Get file descriptors for polling the PCM and MIDI devices.
		int seq_nfds = midiIn.getNumDescriptors();
		int pcm_nfds = pcmOut.getNumDescriptors();
		struct pollfd *pfds = (struct pollfd *)alloca(sizeof(struct pollfd) * (seq_nfds + pcm_nfds));
		midiIn.setupDescriptors(&pfds[0]);
		pcmOut.setupDescriptors(&pfds[seq_nfds]);

	#if 0
		for (l1 = 0; l1 < POLY; note_active[l1++] = 0);
	#endif // 0

		while (1) {
			// Wait for any events.
			if (poll (pfds, seq_nfds + pcm_nfds, 1000) > 0) {

				// Process MIDI events.
				for (int l1 = 0; l1 < seq_nfds; l1++) {
				   if (pfds[l1].revents > 0) {
					   midiIn.callback();
				   }
				}

				// Process PCM events.
				for(int l1 = seq_nfds; l1 < seq_nfds + pcm_nfds; l1++) {
					if (pfds[l1].revents > 0) {
						pcmOut.callback();
					}
				}
			}
		}
	}
	catch(std::string &ex)
	{
		std::cout << ex << std::endl;
		return 1;
	}
}
