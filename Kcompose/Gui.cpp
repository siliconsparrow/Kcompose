/*
 * Gui.cpp
 *
 *  Created on: 21 Mar 2021
 *      Author: adam
 */

#include "Gui.h"
#include "KComposeWindow.h"
#include <gtkmm/application.h>

void *gGuiStart(void *arg)
{
	((Gui *)arg)->process();
	return 0;
}

Gui::Gui()
	: _pcMidiKeyboard(0)
	, _done(false)
{
	pthread_create(&_thread, 0, &gGuiStart, this);
}

bool Gui::isClosed() const
{
	// TODO: Can I query the state of the thread instead of this?
	return _done;
}

int Gui::getMidiKeyboardClientId()
{
	if(0 == _pcMidiKeyboard) {
		_pcMidiKeyboard = new PcMidiKeyboard();
	}

	return _pcMidiKeyboard->getClientId();
}

void Gui::process()
{
	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create("KCompose");

    KComposeWindow window(_pcMidiKeyboard);

    // Shows the window and returns when it is closed.
    app->run(window);

    _done = true;

    pthread_exit(0);
}

