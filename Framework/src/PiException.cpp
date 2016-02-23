#include "PiException.h"

PiException::PiException():
exception(), 
msg("SDLERROR")
//msg(SDL_GetError()) 
{}

PiException::PiException(const std::string& m):
exception(), 
msg(m) 
{}

PiException::~PiException() throw() {}

const char* PiException::what() const throw() 
{
    return msg.c_str();
}
