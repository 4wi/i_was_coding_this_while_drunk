#include <iostream>
#include <vector>
#include <cstdint>
#include <Windows.h>

class CUtils
{

public:

	void OpenBinary( std::string m_sSource, std::vector< std::uint8_t > &m_aData );
	uint8_t* FindPattern( std::string sModule, std::string sPattern );

};

extern CUtils g_Utils;