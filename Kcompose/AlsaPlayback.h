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

	class Source
	{
	public:
		virtual ~Source() { }
		virtual void getFrame(short *buffer, snd_pcm_sframes_t nFrames) = 0;
	};

	void setSource(Source *s) { _source = s; }

	void callback();

private:
	enum { BUFSIZE = 512, kDefaultRate = 44100 };

	short     *_buf;
	snd_pcm_t *_hPlayback;
	Source    *_source;
};

#endif /* ALSAPLAYBACK_H_ */
