/*
 * GuiMessage.h
 *
 *  Created on: 25 Mar 2021
 *      Author: adam
 */

#ifndef GUIMESSAGE_H_
#define GUIMESSAGE_H_

#include <pthread.h>
#include <queue>

class GuiMessage
{
public:
	enum Type { msgPreset };

	GuiMessage(Type type, unsigned param)
		: _type(type)
		, _param(param)
	{ }

	Type     getType()  const { return _type; }
	unsigned getParam() const { return _param; }

private:
	Type     _type;
	unsigned _param;
};

class GuiMessageQueue
{
public:
	GuiMessageQueue();

	void       push(GuiMessage msg);
	GuiMessage pop();

	unsigned size() const { return _queue.size(); }

private:
	pthread_mutex_t        _lock;
	std::queue<GuiMessage> _queue;
};

#endif // GUIMESSAGE_H_
