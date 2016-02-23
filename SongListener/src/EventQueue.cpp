#include "EventQueue.h"
#include "ScopedLock.h"
#include "Log.h"

using namespace std;
BlockingQueue::BlockingQueue():
m(),
cond()
{
	ScopedLock s(m);
	q = new queue<Event>();
}
BlockingQueue::~BlockingQueue()
{
	ScopedLock s(m);
	delete q;
}
void BlockingQueue::push(Event e)
{
//	SystemLog("Trying to push an event");
	ScopedLock s(m);
//	SystemLog("We are not blocked");
	q->push(e);
	cond.notify_one();
//	SystemLog("We pushed the event");
	//Signal Free
}
Event BlockingQueue::pop()
{
	std::unique_lock<std::mutex> lock(m);
	
	while( q->empty() )
	{
		cond.wait(lock);
	}
	Event e = q->front();
	q->pop();
	return e; //Check that this is valid
}

