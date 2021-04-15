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
{
	// Run the Gui in its own thread so it doesn't interfere with the audio processing.
	pthread_create(&_thread, 0, &gGuiStart, this);
}

bool Gui::isClosed() const
{
	// Has the Gui thread finished? It will finish when the window is closed.
	return 0 == pthread_tryjoin_np(_thread, 0);
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

    KComposeWindow window(_msgQueue, _pcMidiKeyboard);

    // Shows the window and returns when it is closed.
    app->run(window);

    pthread_exit(0);
}

