/*
 * KComposeWindow.h
 *
 *  Created on: 21 Mar 2021
 *      Author: adam
 */

#ifndef KCOMPOSEWINDOW_H_
#define KCOMPOSEWINDOW_H_

#include "PcMidiKeyboard.h"
#include <gtkmm.h>

class KComposeWindow
    : public Gtk::Window
{
public:
	KComposeWindow(PcMidiKeyboard *kb);
	virtual ~KComposeWindow();

private:
  //Override default signal handler:
  bool on_key_press_event(GdkEventKey* event) override;
  bool on_key_release_event(GdkEventKey *key_event) override;


  uint8_t keymap[128]; // remember which keys are pressed to foil the auto-repeat.
  Gtk::Grid m_container;
  Gtk::RadioButton m_first;
  Gtk::RadioButton m_second;

  PcMidiKeyboard *_pcMidiKeyboard;
};

#endif /* KCOMPOSEWINDOW_H_ */
