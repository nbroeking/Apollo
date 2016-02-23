#include "ScopedLock.h"

ScopedLock::ScopedLock(std::mutex& m)
{
	m.lock();
	mut = &m;
}
ScopedLock::~ScopedLock()
{
	mut->unlock();
}
