#include "AlsaMidiIn.h"
#include <iostream>
#include <string>

#define MIDI_DEVICE "default"
#define MIDI_CLIENT_NAME "keytarSim"

AlsaMidiIn::AlsaMidiIn()
{
    if(snd_seq_open(&_hSeq, MIDI_DEVICE, SND_SEQ_OPEN_INPUT, 0) < 0) {
        throw std::string("Error opening ALSA sequencer.");
    }

    snd_seq_set_client_name(_hSeq, MIDI_CLIENT_NAME);
    if(snd_seq_create_simple_port(_hSeq, MIDI_CLIENT_NAME,
                                  SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE,
                                  SND_SEQ_PORT_TYPE_APPLICATION) < 0) {
    	throw std::string("Error creating sequencer port.");
    }
}

AlsaMidiIn::~AlsaMidiIn()
{
	snd_seq_close(_hSeq);
}

// Find a device whose name starts with the given string and has the given capabilities.
int AlsaMidiIn::findDevice(const char *name, MidiCapability desiredCapability)
{
	int result = -1;
	snd_seq_t *hSeq;

	if(snd_seq_open(&hSeq, MIDI_DEVICE, SND_SEQ_OPEN_INPUT, 0) < 0) {
	    throw std::string("Error opening ALSA sequencer.");
	}

	snd_seq_client_info_t *cinfo;
	snd_seq_client_info_alloca(&cinfo);

	snd_seq_port_info_t *pinfo;
	snd_seq_port_info_alloca(&pinfo);

	snd_seq_client_info_set_client(cinfo, -1);
	while(snd_seq_query_next_client(hSeq, cinfo) >= 0 && result < 0) {
		// Look through all the available MIDI ports for this client.
		snd_seq_port_info_set_client(pinfo, snd_seq_client_info_get_client(cinfo));
		snd_seq_port_info_set_port(pinfo, -1);

		// Look for a name match.
		if(0 == strncasecmp(snd_seq_client_info_get_name(cinfo), name, strlen(name))) {
			// Iterate through all the ports for this client.
			while(snd_seq_query_next_port(hSeq, pinfo) >= 0) {
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
	snd_seq_close(hSeq);
	return result;
}

void AlsaMidiIn::printDevices()
{
	snd_seq_t *hSeq;

	if(snd_seq_open(&hSeq, MIDI_DEVICE, SND_SEQ_OPEN_INPUT, 0) < 0) {
		throw std::string("Error opening ALSA sequencer.");
	}

	snd_seq_client_info_t *cinfo;
	snd_seq_port_info_t   *pinfo;

	snd_seq_client_info_alloca(&cinfo);
	snd_seq_port_info_alloca(&pinfo);
	snd_seq_client_info_set_client(cinfo, -1);

	while(snd_seq_query_next_client(hSeq, cinfo) >= 0) {
		snd_seq_port_info_set_client(pinfo, snd_seq_client_info_get_client(cinfo));
		snd_seq_port_info_set_port(pinfo, -1);
		std::cout << "client " << snd_seq_client_info_get_client(cinfo)
				  << ": '" << snd_seq_client_info_get_name(cinfo)
				  << "' [type=" << (snd_seq_client_info_get_type(cinfo) == SND_SEQ_USER_CLIENT ? "user" : "kernel")
				  << "]" << std::endl;
		while(snd_seq_query_next_port(hSeq, pinfo) >= 0) {
			std::cout << "  " << snd_seq_port_info_get_port(pinfo) << " " << snd_seq_port_info_get_name(pinfo) << std::endl;
		}
	}

	snd_seq_close(hSeq);
}

void AlsaMidiIn::connectFrom(int sourceDevice)
{
	int destDevice = snd_seq_client_id(_hSeq);
	std::cout << "Connecting " << sourceDevice << " to " << destDevice << std::endl;

	int rslt = snd_seq_connect_from(_hSeq, 0, sourceDevice, 0);
	if(0 != rslt) {
		throw std::string("Failed to connect");
	}
}

int AlsaMidiIn::getNumDescriptors()
{
	return snd_seq_poll_descriptors_count(_hSeq, POLLIN);
}

void AlsaMidiIn::setupDescriptors(struct pollfd *pfds)
{
	snd_seq_poll_descriptors(_hSeq, pfds, getNumDescriptors(), POLLIN);
}

// Called when there are events.
void AlsaMidiIn::callback()
{
    snd_seq_event_t *ev;

    do {
        snd_seq_event_input(_hSeq, &ev);
        if(_sink != 0) {
        	_sink->midiEvent(ev);
        }
        snd_seq_free_event(ev);
    } while (snd_seq_event_input_pending(_hSeq, 0) > 0);
}
