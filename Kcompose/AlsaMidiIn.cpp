#include "AlsaMidiIn.h"
#include <iostream>
#include <string>

#define MIDI_DEVICE "default"
#define MIDI_CLIENT_NAME "keytarSim"

AlsaMidiIn::AlsaMidiIn()
{
    if(snd_seq_open(&seq_handle, MIDI_DEVICE, SND_SEQ_OPEN_INPUT, 0) < 0) {
        throw std::string("Error opening ALSA sequencer.");
    }

    snd_seq_set_client_name(seq_handle, MIDI_CLIENT_NAME);
    if(snd_seq_create_simple_port(seq_handle, MIDI_CLIENT_NAME,
                                  SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE,
                                  SND_SEQ_PORT_TYPE_APPLICATION) < 0) {
    	throw std::string("Error creating sequencer port.");
    }
}

AlsaMidiIn::~AlsaMidiIn()
{
	snd_seq_close(seq_handle);
}

// Find a device whose name starts with the given string and has the given capabilities.
int AlsaMidiIn::findDevice(const char *name, MidiCapability desiredCapability)
{
	int result = -1;

	snd_seq_client_info_t *cinfo;
	snd_seq_client_info_alloca(&cinfo);

	snd_seq_port_info_t *pinfo;
	snd_seq_port_info_alloca(&pinfo);

	snd_seq_client_info_set_client(cinfo, -1);
	while(snd_seq_query_next_client(seq_handle, cinfo) >= 0 && result < 0) {
		// Look through all the available MIDI ports for this client.
		snd_seq_port_info_set_client(pinfo, snd_seq_client_info_get_client(cinfo));
		snd_seq_port_info_set_port(pinfo, -1);

		// Look for a name match.
		if(0 == strncasecmp(snd_seq_client_info_get_name(cinfo), name, strlen(name))) {
			// Iterate through all the ports for this client.
			while(snd_seq_query_next_port(seq_handle, pinfo) >= 0) {
				// Check the port supports MIDI input.
				if((unsigned)desiredCapability == (snd_seq_port_info_get_capability(pinfo) & (unsigned)desiredCapability)) {
					// Found a match!
					result = snd_seq_client_info_get_client(cinfo);
					break;
				}
			}
		}
	}

	// Seems to crash if I call these free functions.
	//snd_seq_port_info_free(pinfo);
	//snd_seq_client_info_free(cinfo);
	return result;
}

void AlsaMidiIn::printDevices()
{
	snd_seq_client_info_t *cinfo;
	snd_seq_port_info_t   *pinfo;

	snd_seq_client_info_alloca(&cinfo);
	snd_seq_port_info_alloca(&pinfo);
	snd_seq_client_info_set_client(cinfo, -1);

	while(snd_seq_query_next_client(seq_handle, cinfo) >= 0) {
		snd_seq_port_info_set_client(pinfo, snd_seq_client_info_get_client(cinfo));
		snd_seq_port_info_set_port(pinfo, -1);
		std::cout << "client " << snd_seq_client_info_get_client(cinfo)
				  << ": '" << snd_seq_client_info_get_name(cinfo)
				  << "' [type=" << (snd_seq_client_info_get_type(cinfo) == SND_SEQ_USER_CLIENT ? "user" : "kernel")
				  << "]" << std::endl;
		while(snd_seq_query_next_port(seq_handle, pinfo) >= 0) {
			std::cout << "  " << snd_seq_port_info_get_port(pinfo) << " " << snd_seq_port_info_get_name(pinfo) << std::endl;
		}
	}
}

void AlsaMidiIn::connectFrom(int sourceDevice)
{
	int destDevice = snd_seq_client_id(seq_handle);
	std::cout << "Connecting " << sourceDevice << " to " << destDevice << std::endl;

	if(0 != snd_seq_connect_from(seq_handle, 0, sourceDevice, 0)) {
		throw std::string("Failed to connect");
	}
}

int AlsaMidiIn::getNumDescriptors()
{
	return snd_seq_poll_descriptors_count(seq_handle, POLLIN);
}

void AlsaMidiIn::setupDescriptors(struct pollfd *pfds)
{
	snd_seq_poll_descriptors(seq_handle, pfds, getNumDescriptors(), POLLIN);
}

// Called when there are events.
void AlsaMidiIn::callback()
{
    snd_seq_event_t *ev;
    //int              l1;

    do {
        snd_seq_event_input(seq_handle, &ev);
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
        snd_seq_free_event(ev);
    } while (snd_seq_event_input_pending(seq_handle, 0) > 0);
}
