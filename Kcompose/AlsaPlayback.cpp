/*
 * AlsaPlayback.cpp
 *
 *  Created on: 17 Mar 2021
 *      Author: adam
 */

#include "AlsaPlayback.h"
#include <iostream>

AlsaPlayback::AlsaPlayback(const char *device)
	: _source(0)
{
	snd_pcm_hw_params_t *hw_params;
	snd_pcm_sw_params_t *sw_params;
	unsigned             rate = kDefaultRate;

	// Open a PCM device.
	if (snd_pcm_open(&_hPlayback, device, SND_PCM_STREAM_PLAYBACK, 0/*SND_PCM_NONBLOCK*/) < 0) {
		throw std::string("cannot open audio device");
	}

	// Set up stream parameters.
	snd_pcm_hw_params_alloca(&hw_params);
	snd_pcm_hw_params_any(_hPlayback, hw_params);
	snd_pcm_hw_params_set_access(_hPlayback, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED);
	snd_pcm_hw_params_set_format(_hPlayback, hw_params, SND_PCM_FORMAT_S16_LE);
	snd_pcm_hw_params_set_rate_near(_hPlayback, hw_params, &rate, 0);
	snd_pcm_hw_params_set_channels(_hPlayback, hw_params, 2);
	snd_pcm_hw_params_set_periods(_hPlayback, hw_params, 2, 0);
	snd_pcm_hw_params_set_period_size(_hPlayback, hw_params, BUFSIZE, 0);
	snd_pcm_hw_params(_hPlayback, hw_params);
	snd_pcm_sw_params_alloca(&sw_params);
	snd_pcm_sw_params_current(_hPlayback, sw_params);
	snd_pcm_sw_params_set_avail_min(_hPlayback, sw_params, BUFSIZE);
	snd_pcm_sw_params(_hPlayback, sw_params);

	// Set up audio buffer.
	_buf = (short *)malloc(2 * sizeof(short) * BUFSIZE);
}

AlsaPlayback::~AlsaPlayback()
{
	snd_pcm_close(_hPlayback);
	free(_buf);
}

int AlsaPlayback::getNumDescriptors()
{
	return snd_pcm_poll_descriptors_count(_hPlayback);
}

void AlsaPlayback::setupDescriptors(struct pollfd *pfds)
{
	snd_pcm_poll_descriptors(_hPlayback, pfds, getNumDescriptors());
}

void AlsaPlayback::callback()
{
	snd_pcm_sframes_t nframes = BUFSIZE;

	if(_source != 0) {
		_source->getFrame(_buf, nframes);
	} else {
		memset(_buf, 0, nframes * 4);
	}

    if(nframes != snd_pcm_writei(_hPlayback, _buf, nframes)) {
    	std::cout << "PCM overrun!" << std::endl;
    	snd_pcm_prepare(_hPlayback);
    }
}
