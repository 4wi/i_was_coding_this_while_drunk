#include <iostream>
#include <vector>
#include <Windows.h>

using RegOpenKeyExA_t = LSTATUS ( __stdcall* ) ( HKEY, LPCSTR, DWORD, REGSAM, PHKEY );
using RegQueryValueExA_t = LSTATUS ( __stdcall* ) ( HKEY, LPCSTR, LPDWORD, LPDWORD, LPBYTE, LPDWORD );
using RegCloseKey_t = LSTATUS ( __stdcall* ) ( HKEY );
using RegSetValueExA_t = LSTATUS ( __stdcall* ) ( HKEY, LPCSTR, DWORD, DWORD, BYTE*, DWORD );

class CUtils
{

public:

	void WstringToString( const std::wstring& sSource, std::string& sDest );
	void OpenBinary( std::string m_sSource, std::vector< std::uint8_t > &m_aData );

};

extern CUtils g_Utils;

struct CRegistry
{
	RegOpenKeyExA_t m_fnRegOpenKeyExA = nullptr;
	RegQueryValueExA_t m_fnRegQueryValueExA = nullptr;
	RegCloseKey_t m_fnRegCloseKey = nullptr;
	RegSetValueExA_t m_fnRegSetValueExA = nullptr;
};

extern CRegistry* g_pRegistry;