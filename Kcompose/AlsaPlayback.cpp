/*
 * AlsaPlayback.cpp
 *
 *  Created on: 17 Mar 2021
 *      Author: adam
 */

#include "AlsaPlayback.h"
#include <iostream>

AlsaPlayback::AlsaPlayback(const char *device)
{
	snd_pcm_hw_params_t *hw_params;
	snd_pcm_sw_params_t *sw_params;
	unsigned             rate = kDefaultRate;

	// Open a PCM device.
	if (snd_pcm_open(&playback_handle, device, SND_PCM_STREAM_PLAYBACK, SND_PCM_NONBLOCK) < 0) {
		throw std::string("cannot open audio device");
	}

	// Set up stream parameters.
	snd_pcm_hw_params_alloca(&hw_params);
	snd_pcm_hw_params_any(playback_handle, hw_params);
	snd_pcm_hw_params_set_access(playback_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED);
	snd_pcm_hw_params_set_format(playback_handle, hw_params, SND_PCM_FORMAT_S16_LE);
	snd_pcm_hw_params_set_rate_near(playback_handle, hw_params, &rate, 0);
	snd_pcm_hw_params_set_channels(playback_handle, hw_params, 2);
	snd_pcm_hw_params_set_periods(playback_handle, hw_params, 2, 0);
	snd_pcm_hw_params_set_period_size(playback_handle, hw_params, BUFSIZE, 0);
	snd_pcm_hw_params(playback_handle, hw_params);
	snd_pcm_sw_params_alloca(&sw_params);
	snd_pcm_sw_params_current(playback_handle, sw_params);
	snd_pcm_sw_params_set_avail_min(playback_handle, sw_params, BUFSIZE);
	snd_pcm_sw_params(playback_handle, sw_params);

	// Set up audio buffer.
	buf = (short *)malloc(2 * sizeof(short) * BUFSIZE);
}

AlsaPlayback::~AlsaPlayback()
{
	snd_pcm_close(playback_handle);
	free(buf);
}

int AlsaPlayback::getNumDescriptors()
{
	return snd_pcm_poll_descriptors_count(playback_handle);
}

void AlsaPlayback::setupDescriptors(struct pollfd *pfds)
{
	snd_pcm_poll_descriptors(playback_handle, pfds, getNumDescriptors());
}

void AlsaPlayback::callback()
{
	snd_pcm_sframes_t nframes = BUFSIZE;
//    int l1, l2;
//    double dphi, dphi_mod, f1, f2, f3, freq_note, sound;

    memset(buf, 0, nframes * 4);

    for(int i = 0; i < BUFSIZE * 2; i++) {
    	buf[i] = (i & 63) * 500;
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
                buf[2 * l1] += sound;
                buf[2 * l1 + 1] += sound;
            }
        }
    }
#endif // 0

    if(nframes != snd_pcm_writei(playback_handle, buf, nframes)) {
    	std::cout << "PCM overrun!" << std::endl;
    	snd_pcm_prepare(playback_handle);
    }
}
