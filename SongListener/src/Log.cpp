#include "Log.h"

using namespace std;

void SystemLog(std::string log)
{
	cout << "Pi Log: " << log << "\n";
}
void SystemError(std::string log)
{
	cerr << "Pi Log Error: " << log << endl;
}
