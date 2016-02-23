#include "Event.h"
#include "ScopedLock.h"

Event::Event():
m(),
songs()
{
	ScopedLock s(m);
	type = NOOP;
	fd = 0;
}
Event::Event(const Event& event):
m(),
songs()
{
	//Each event must have its own mutex
	this->type = event.type;
	this->fd = event.fd;
	this->songs = event.songs;
}
Event::~Event()
{
}
Event& Event::operator= (const Event& event )
{
	//We dont copy mutexes
	//Hopefully thread safe but probably not
	this->type = event.type;
	this->fd = event.fd;
	this->songs = event.songs;
	return *this;
}
Type Event::getType()
{
	//ScopedLock
	ScopedLock s(m);
	return type;
}
void Event::setType(Type t)
{
	//Scoped Lock
	ScopedLock s(m);
	type = t;
}
int Event::getFD()
{
	ScopedLock s(m);
	return fd;
}
void Event::setFD(int x)
{
	ScopedLock s(m);
	fd = x;
}
std::vector<std::string> Event::getSongs()
{
	ScopedLock s(m);
	return songs;
}
void Event::setSongs(std::vector<std::string> t)
{
	ScopedLock s(m);
	songs = t;
}
