/*
 * Gui.h
 *
 *  Created on: 21 Mar 2021
 *      Author: adam
 */

#ifndef GUI_H_
#define GUI_H_

#include "PcMidiKeyboard.h"
#include <pthread.h>

class Gui {
public:
	Gui();

	void start();

	int getMidiKeyboardClientId();
	bool isClosed() const;

private:
	pthread_t       _thread;
	PcMidiKeyboard *_pcMidiKeyboard;
	bool            _done;

public:
	void process();
};

#endif /* GUI_H_ */
