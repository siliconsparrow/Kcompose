/*
 * PcKeyboardMidiIn.cpp
 *
 *  Created on: 18 Mar 2021
 *      Author: adam
 */

#include "PcKeyboardMidiIn.h"
#include <linux/input.h>
//#include <stdio.h>
#include <string>
#include <iostream>

PcKeyboardMidiIn::PcKeyboardMidiIn()
{
	const char *dev = "/dev/input/by-path/platform-i8042-serio-0-event-kbd";

	_fd = open(dev, O_RDONLY);
	if(_fd == -1) {
		throw std::string("Cannot open keyboard device");
	}
}

PcKeyboardMidiIn::~PcKeyboardMidiIn()
{
	close(_fd);
}

int PcKeyboardMidiIn::getNumDescriptors()
{
	return 1;
}

void PcKeyboardMidiIn::setupDescriptors(struct pollfd *pfds)
{
	pfds->fd      = _fd;
	pfds->events  = POLLIN;
	pfds->revents = 0;
}

void PcKeyboardMidiIn::callback()
{
	static const char *const evval[3] = {
	    "RELEASED",
	    "PRESSED ",
	    "REPEATED"
	};

	enum { kMaxKeyboardEvents = 32 };

	struct input_event eventBuf[kMaxKeyboardEvents];

	ssize_t s = read(_fd, &eventBuf[0], sizeof(eventBuf));
	if(s == -1) {
		throw std::string("KB read failed");
	}

	int nEvents = s / sizeof(struct input_event);
	for(int i = 0; i < nEvents; i++) {
		struct input_event *ev = &eventBuf[i];

		if(ev->type == EV_KEY && ev->value >= 0 && ev->value <= 2) {
		    std::cout << evval[ev->value] << " " << (int)ev->code << std::endl;
		}
	}
}


#ifdef EXAMPLE

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include <string.h>


int main(void)
{
    struct input_event ev;
    ssize_t n;

    while (1) {
        n = read(fd, &ev, sizeof ev);
        if (n == (ssize_t)-1) {
            if (errno == EINTR)
                continue;
            else
                break;
        } else
        if (n != sizeof ev) {
            errno = EIO;
            break;
        }
The above snippet breaks out from the loop if any error occurs, or if the userspace receives only a partial event structure (which should not happen, but might in some future/buggy kernels). You might wish to use a more robust read loop; I personally would be satisfied by replacing the last break with continue, so that partial event structures are ignored.

You can then examine the ev event structure to see what occurred, and finish the program:

        if (ev.type == EV_KEY && ev.value >= 0 && ev.value <= 2)
            printf("%s 0x%04x (%d)\n", evval[ev.value], (int)ev.code, (int)ev.code);

    }
    fflush(stdout);
    fprintf(stderr, "%s.\n", strerror(errno));
    return EXIT_FAILURE;
}
For a keypress,

ev.time: time of the event (struct timeval type)

ev.type: EV_KEY

ev.code: KEY_*, key identifier; see complete list in /usr/include/linux/input.h

ev.value: 0 if key release, 1 if key press, 2 if autorepeat keypress

More information at https://www.kernel.org/doc/Documentation/input/input.txt

#endif
