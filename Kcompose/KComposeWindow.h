/*
 * KComposeWindow.h
 *
 *  Created on: 21 Mar 2021
 *      Author: adam
 */

#ifndef KCOMPOSEWINDOW_H_
#define KCOMPOSEWINDOW_H_

#include "PcMidiKeyboard.h"
#include "MiniFmSynth.h"
#include "GuiMessage.h"
#include <gtkmm.h>

class KComposeWindow
    : public Gtk::Window
{
public:
	KComposeWindow(GuiMessageQueue &msgQueue, PcMidiKeyboard *kb);
	virtual ~KComposeWindow() { }

private:
  //Override default signal handler:
  bool on_key_press_event(GdkEventKey* event) override;
  bool on_key_release_event(GdkEventKey *key_event) override;
  void on_radio_button_clicked(int &presetid); //Gtk::RadioButton& button);

  uint8_t _keymap[128]; // remember which keys are pressed to foil the auto-repeat.
  Gtk::Grid _container;
  //Gtk::RadioButton m_first;
  //Gtk::RadioButton m_second;

  Gtk::RadioButton *_radioPreset[MiniFmSynth::kNumPresets];
  Gtk::RadioButton *_SelectedButton;

  PcMidiKeyboard   *_pcMidiKeyboard;
  GuiMessageQueue  &_msgQueue;
};

#endif /* KCOMPOSEWINDOW_H_ */
