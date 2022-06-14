#include <iostream>
#include <Windows.h>

class CUtils
{

public:

	uint8_t* FindPattern( std::string sModule, std::string sPattern );

};

extern CUtils g_Utils;