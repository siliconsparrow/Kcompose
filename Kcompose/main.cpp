
// A good place to get example code and such is from the ALSA GitHub repo. Look at
// all the alsa-utils for examples on how to use the library.
// https://github.com/alsa-project

// I'm starting with miniFMsynth by Matthias Nagorni and adapting the basic structure to my own audio filter system

#include "AlsaMidiIn.h"
#include "AlsaPlayback.h"
#include "MiniFmSynth.h"
#include <alsa/asoundlib.h>
#include <string>
#include <iostream>

int main (int argc, char *argv[])
{
	try
	{
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

		// Create a synth instance and connect it to the input and output.
		MiniFmSynth synth;
		pcmOut.setSource(&synth);
		midiIn.setSink(&synth);

		// Get file descriptors for polling the PCM and MIDI devices.
		int seq_nfds = midiIn.getNumDescriptors();
		int pcm_nfds = pcmOut.getNumDescriptors();
		struct pollfd *pfds = (struct pollfd *)alloca(sizeof(struct pollfd) * (seq_nfds + pcm_nfds));
		midiIn.setupDescriptors(&pfds[0]);
		pcmOut.setupDescriptors(&pfds[seq_nfds]);

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
