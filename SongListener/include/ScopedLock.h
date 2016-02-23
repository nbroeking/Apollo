/*
* File: ScopedLock
* Written By: Nicolas Broeking
* Written On: July 25, 2014
*/

#ifndef __ScopedLock__
#define __ScopedLock__

#include <mutex>
class ScopedLock
{
public:
	ScopedLock( std::mutex& m);
	~ScopedLock();
private: 
	std::mutex* mut;
};

#endif //__ScopedLock__
