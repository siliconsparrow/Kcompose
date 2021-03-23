/*
 * PcMidiKeyboard.cpp
 *
 *  Created on: 22 Mar 2021
 *      Author: adam
 */

#include "PcMidiKeyboard.h"
#include <string>
#include <gdk/gdk.h>

const char *midiClientName = "PCKeyboard";

enum MIDINote
{
	C0,  Cs0,  D0,  Ds0,  E0,  F0,  Fs0,  G0,  Gs0,  A0,  As0,  B0,
	C1,  Cs1,  D1,  Ds1,  E1,  F1,  Fs1,  G1,  Gs1,  A1,  As1,  B1,
	C2,  Cs2,  D2,  Ds2,  E2,  F2,  Fs2,  G2,  Gs2,  A2,  As2,  B2,
	C3,  Cs3,  D3,  Ds3,  E3,  F3,  Fs3,  G3,  Gs3,  A3,  As3,  B3,
	C4,  Cs4,  D4,  Ds4,  E4,  F4,  Fs4,  G4,  Gs4,  A4,  As4,  B4,
	C5,  Cs5,  D5,  Ds5,  E5,  F5,  Fs5,  G5,  Gs5,  A5,  As5,  B5,
	C6,  Cs6,  D6,  Ds6,  E6,  F6,  Fs6,  G6,  Gs6,  A6,  As6,  B6,
	C7,  Cs7,  D7,  Ds7,  E7,  F7,  Fs7,  G7,  Gs7,  A7,  As7,  B7,
	C8,  Cs8,  D8,  Ds8,  E8,  F8,  Fs8,  G8,  Gs8,  A8,  As8,  B8,
	C9,  Cs9,  D9,  Ds9,  E9,  F9,  Fs9,  G9,  Gs9,  A9,  As9,  B9,
	C10, Cs10, D10, Ds10, E10, F10, Fs10, G10, Gs10, A10, As10, B10,
};

const struct { unsigned keycode; unsigned noteNum; } PC_MIDI_KEYBOARD_MAP[] = {
	{ GDK_KEY_z, C3 },
	{ GDK_KEY_s, Cs3 },
	{ GDK_KEY_x, D3 },
	{ GDK_KEY_d, Ds3 },
	{ GDK_KEY_c, E3 },
	{ GDK_KEY_v, F3 },
	{ GDK_KEY_g, Fs3 },
	{ GDK_KEY_b, G3 },
	{ GDK_KEY_h, Gs3 },
	{ GDK_KEY_n, A3 },
	{ GDK_KEY_j, As3 },
	{ GDK_KEY_m, B3 },
	{ GDK_KEY_comma, C4 },

	{ GDK_KEY_q, C4 },
	{ GDK_KEY_2, Cs4 },
	{ GDK_KEY_w, D4 },
	{ GDK_KEY_3, Ds4 },
	{ GDK_KEY_e, E4 },
	{ GDK_KEY_r, F4 },
	{ GDK_KEY_5, Fs4 },
	{ GDK_KEY_t, G4 },
	{ GDK_KEY_6, Gs4 },
	{ GDK_KEY_y, A4 },
	{ GDK_KEY_7, As4 },
	{ GDK_KEY_u, B4 },
	{ GDK_KEY_i, C5 },
	{ GDK_KEY_9, Cs5 },
	{ GDK_KEY_o, D5 },
	{ GDK_KEY_0, Ds5 },
	{ GDK_KEY_p, E5 },

	{ 0, 0 }
};

PcMidiKeyboard::PcMidiKeyboard()
{
	if(snd_seq_open(&_hSeq, "default", SND_SEQ_OPEN_OUTPUT, 0) < 0) {
		throw std::string("Error opening ALSA sequencer.");
	}

	snd_seq_set_client_name(_hSeq, midiClientName);

    if((_portId = snd_seq_create_simple_port(_hSeq, midiClientName,
	            SND_SEQ_PORT_CAP_READ|SND_SEQ_PORT_CAP_SUBS_READ,
	            SND_SEQ_PORT_TYPE_APPLICATION)) < 0) {
	    throw std::string("Error creating sequencer port.\n");
	  }

//	if((_portId = snd_seq_create_simple_port(_hSeq, midiClientName,
//	            SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE,
//	            SND_SEQ_PORT_TYPE_APPLICATION)) < 0) {
//	    throw std::string("Error creating sequencer port.\n");
//	}
}

int PcMidiKeyboard::getClientId()
{
	return snd_seq_client_id(_hSeq);
}

// Transmit MIDI message to ALSA. eventType would usually be SND_SEQ_EVENT_NOTEON or SND_SEQ_EVENT_NOTEOFF.
void PcMidiKeyboard::sendMidiNoteEvent(snd_seq_event_type_t eventType, unsigned noteNum)
{
    snd_seq_event_t ev;
    snd_seq_ev_clear(&ev);
    snd_seq_ev_set_source(&ev, _portId);
	snd_seq_ev_set_subs(&ev);
	snd_seq_ev_set_direct(&ev);

	ev.type = eventType;
	ev.data.note.note = noteNum;
	ev.data.note.channel = kMidiChannel;
	ev.data.note.velocity = kDefaultVelocity;

    snd_seq_event_output(_hSeq, &ev);
    snd_seq_drain_output(_hSeq);
}

unsigned PcMidiKeyboard::mapKeyToNote(int keycode)
{
	for(int i = 0; PC_MIDI_KEYBOARD_MAP[i].keycode != 0; i++) {
		if(PC_MIDI_KEYBOARD_MAP[i].keycode == keycode) {
			return PC_MIDI_KEYBOARD_MAP[i].noteNum;
		}
	}

	return 0;
}

bool PcMidiKeyboard::keypress(int keycode)
{
	unsigned noteNum = mapKeyToNote(keycode);
	if(noteNum != 0) {
		sendMidiNoteEvent(SND_SEQ_EVENT_NOTEON, noteNum);
		return true;
	}

	return false; // We don't want this keystroke.
}

bool PcMidiKeyboard::keyrelease(int keycode)
{
	unsigned noteNum = mapKeyToNote(keycode);
	if(noteNum != 0) {
		sendMidiNoteEvent(SND_SEQ_EVENT_NOTEOFF, noteNum);
		return true;
	}

	return false; // We don't want this keystroke.
}

