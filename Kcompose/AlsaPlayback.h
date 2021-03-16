/*
 * AlsaPlayback.h
 *
 *  Created on: 17 Mar 2021
 *      Author: adam
 */

#ifndef ALSAPLAYBACK_H_
#define ALSAPLAYBACK_H_

#include <alsa/asoundlib.h>

class AlsaPlayback
{
public:
	AlsaPlayback(const char *device);
	~AlsaPlayback();

	int getNumDescriptors();
	void setupDescriptors(struct pollfd *pfds);

	void callback();

private:
	enum { BUFSIZE = 512, kDefaultRate = 44100 };

	short     *buf;
	snd_pcm_t *playback_handle;
};

#endif /* ALSAPLAYBACK_H_ */
