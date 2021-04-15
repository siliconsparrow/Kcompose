/*
 * Gui.h
 *
 *  Created on: 21 Mar 2021
 *      Author: adam
 */

#ifndef GUI_H_
#define GUI_H_

#include "PcMidiKeyboard.h"
#include "GuiMessage.h"
#include <pthread.h>

class Gui {
public:
	Gui();

	void start();

	int getMidiKeyboardClientId();
	bool isClosed() const;

	bool       hasMessage() const { return _msgQueue.size() > 0; }
	GuiMessage getMessage()       { return _msgQueue.pop(); }

private:
	pthread_t        _thread;
	GuiMessageQueue  _msgQueue;
	PcMidiKeyboard  *_pcMidiKeyboard;

public:
	void process();
};

#endif /* GUI_H_ */
