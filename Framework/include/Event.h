/*
* File: Event.h
* Written By: Nicolas Broeking
* Written On: July 25, 2014
*
* This is the list of events that need to be processed.
*
*/

#ifndef __NicEvent__
#define __NicEvent__

enum Type { NOOP, QUIT, STOP, PLAY, CLEANUP, NEWSOCKET, PAUSE, SYNC, SEND,START, NUM};

#include <mutex>
#include <string>
#include <vector>
class Event
{
public:
	Event();
	~Event();
	Event(const Event&);
	Event & operator= (const Event& event);
	Type getType();
	void setType(Type);
	int getFD();
	void setFD(int);
	std::vector<std::string> getSongs();
	void setSongs(std::vector<std::string>);

protected:
private:
	std::mutex m;
	Type type;

	int fd;
	std::vector<std::string> songs;
	
};
#endif //End Declaration
