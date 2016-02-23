/*
*File: Blocking Queue
*Written By: Nicolas Broeking
*Written On: July 25, 2014
* ThreadSafe
*/

#ifndef __BlockingQueue__
#define __BlockingQueue__
#include "Event.h"
#include <mutex>
#include <queue>
#include <condition_variable>

class BlockingQueue
{
public:
	BlockingQueue();
	~BlockingQueue();
	Event pop();
	void push(Event);
protected:
	std::mutex m;
	std::queue<Event>* q;	
	std::condition_variable cond;
private:
};

#endif //End the declaration
