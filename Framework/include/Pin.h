#ifndef __nicpin__
#define __nicpin__

#include <string>
#include <fstream>

class Pin
{
public:
	Pin(std::string x = "17"); // create a GPIO object that controls GPIOx, where x is passed to this constructor
	int on(); // Set GPIO Value (putput pins)
	int off(); // Get GPIO Value (input/ output pins)
	int getval_gpio(std::string& val);
	std::string getVal(); // return the GPIO number associated with the instance of an object
	~Pin();
private:
	std::string gpionum; // GPIO number associated with the instance of an object
	int export_gpio(); // exports GPIO
	int unexport_gpio(); // unexport GPIO
	int setdir_gpio(std::string dir); // Set GPIO Direction
};
#endif
