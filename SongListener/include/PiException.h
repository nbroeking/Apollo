#ifndef __PiException__
#define __PiException__

#include <exception>
#include <string>

class PiException: public std::exception 
{
public:
	PiException();
        PiException(const std::string&);
        virtual ~PiException() throw();
        virtual const char* what() const throw();
private:
        std::string msg;
};


#endif
