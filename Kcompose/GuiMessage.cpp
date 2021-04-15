/*
 * GuiMessage.cpp
 *
 *  Created on: 25 Mar 2021
 *      Author: adam
 */

#include "GuiMessage.h"

GuiMessageQueue::GuiMessageQueue()
{
	pthread_mutex_init(&_lock, 0);
}

void GuiMessageQueue::push(GuiMessage msg)
{
	pthread_mutex_lock(&_lock);

	_queue.push(msg);

	pthread_mutex_unlock(&_lock);
}

GuiMessage GuiMessageQueue::pop()
{
	pthread_mutex_lock(&_lock);

	GuiMessage result = _queue.front();
	_queue.pop();

	pthread_mutex_unlock(&_lock);

	return result;
}
