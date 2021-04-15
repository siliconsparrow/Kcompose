/*
 * KComposeWindow.cpp
 *
 *  Created on: 21 Mar 2021
 *      Author: adam
 */

#include "KComposeWindow.h"

KComposeWindow::KComposeWindow(GuiMessageQueue &msgQueue, PcMidiKeyboard *kb)
	: _pcMidiKeyboard(kb)
	, _SelectedButton(nullptr)
	, _msgQueue(msgQueue)
{
  set_title("K-Compose");
  set_border_width(10);
  add(_container);

  memset(_keymap, 0, sizeof(_keymap));


  for(int i = 0; i < MiniFmSynth::kNumPresets; i++) {
	  Gtk::RadioButton *button = new Gtk::RadioButton();
	  _radioPreset[i] = button;
	  button->set_label(g_Presets[i]._name);
	  if(i == 0) {
		  button->set_active();
	  } else {
		  button->join_group(*_radioPreset[0]);
	  }

	  _container.add(*button);

	  //button->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &KComposeWindow::on_radio_button_clicked), sigc::ref(*button)));
	  button->signal_pressed().connect(sigc::bind(sigc::mem_fun(*this, &KComposeWindow::on_radio_button_clicked), i));
  }


//  // Radio buttons:
//  m_first.set_label("First");
//  m_second.set_label("Second");
//
//  m_second.join_group(m_first);
//  m_first.set_active();
//
//  // Main Container:
//  m_container.add(m_first);
//  m_container.add(m_second);

  // Events.
//  m_first.signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &KComposeWindow::on_radio_button_clicked), sigc::ref(m_first)));


  // We override the default event signal handler.
  add_events(Gdk::KEY_PRESS_MASK);

  show_all_children();
}

bool KComposeWindow::on_key_press_event(GdkEventKey* key_event)
{
	unsigned keyVal = key_event->keyval;
	if(_pcMidiKeyboard != 0 && keyVal < 128) {
		if(_keymap[keyVal] == 0) {
			if(_pcMidiKeyboard->keypress(keyVal)) {
				_keymap[keyVal] = 1;
				return true;
			}
		}
	}
/*
  //GDK_MOD1_MASK -> the 'alt' key(mask)
  //GDK_KEY_1 -> the '1' key
  //GDK_KEY_2 -> the '2' key

  //select the first radio button, when we press alt + 1
  if((key_event->keyval == GDK_KEY_1) &&
    (key_event->state &(GDK_SHIFT_MASK | GDK_CONTROL_MASK | GDK_MOD1_MASK)) == GDK_MOD1_MASK)
  {
    m_first.set_active();
    //returning true, cancels the propagation of the event
    return true;
  }
  else if((key_event->keyval == GDK_KEY_2) &&
    (key_event->state & (GDK_SHIFT_MASK | GDK_CONTROL_MASK | GDK_MOD1_MASK)) == GDK_MOD1_MASK)
  {
    //and the second radio button, when we press alt + 2
    m_second.set_active();
    return true;
  }
  else if(key_event->keyval == GDK_KEY_Escape)
  {
    //close the window, when the 'esc' key is pressed
    hide();
    return true;
  }
  */

  //if the event has not been handled, call the base class
  return Gtk::Window::on_key_press_event(key_event);
}

bool KComposeWindow::on_key_release_event(GdkEventKey *key_event)
{
	unsigned keyVal = key_event->keyval;

	if(_pcMidiKeyboard != 0 && keyVal < 128) {
		if(_pcMidiKeyboard->keyrelease(keyVal)) {
			_keymap[keyVal] = 0;
			return true;
		}
	}

    //if the event has not been handled, call the base class
    return Gtk::Window::on_key_release_event(key_event);
}

void KComposeWindow::on_radio_button_clicked(int &presetId) // Gtk::RadioButton& button)
{
	_msgQueue.push(GuiMessage(GuiMessage::msgPreset, presetId));
}
