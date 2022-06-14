#include <Windows.h>
#include <thread>
#include <fstream>

#include "Install.hpp"

BOOL __stdcall DllMain( HMODULE hModule, DWORD ulReason, LPVOID lpReserved )
{
	if ( ulReason != DLL_PROCESS_ATTACH )
		return 0;

	AllocConsole( );
	freopen_s( reinterpret_cast< FILE** >( stdin ), "CONIN$", "r", stdin );
	freopen_s( reinterpret_cast< FILE** >( stdout ), "CONOUT$", "w", stdout );
	SetConsoleTitleA( "=)" );

	std::thread( 
		[ ] ( ) -> void
		{
			CInstall* pInstall = new CInstall( );
			pInstall->Init( );
		} ).detach( );

	return 1;
}