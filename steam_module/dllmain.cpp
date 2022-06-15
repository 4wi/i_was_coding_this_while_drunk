#include <Windows.h>
#include <iostream>
#include <fstream>

#include "Install.hpp"
#include "Utils.hpp"

using RtlAdjustPrivilege_t = NTSTATUS ( __stdcall* ) ( ULONG, BOOLEAN, BOOLEAN, PBOOLEAN );

BOOL __stdcall DllMain( HMODULE hModule, DWORD ulReason, LPVOID lpReserved )
{
	AllocConsole( );
	freopen_s( reinterpret_cast< FILE** >( stdin ), "CONIN$", "r", stdin );
	freopen_s( reinterpret_cast< FILE** >( stdout ), "CONOUT$", "w", stdout );
	SetConsoleTitleA( "=)" );

	if ( ulReason != DLL_PROCESS_ATTACH )
		return 0;

	HMODULE hNtDll = GetModuleHandleA( "ntdll.dll" );
	if ( !hNtDll )
		return 1;

	RtlAdjustPrivilege_t fnRtlAdjustPrivilege = reinterpret_cast< RtlAdjustPrivilege_t >( 
		GetProcAddress( hNtDll, "RtlAdjustPrivilege" ) );
	if ( !fnRtlAdjustPrivilege )
		return 1;
	
	BOOLEAN bEnabled;
	fnRtlAdjustPrivilege( 20, 1, 0, &bEnabled );

	g_pRegistry = new CRegistry( );
	HMODULE hAdvApi = LoadLibraryA( "ADVAPI32.dll" );
	if ( !hAdvApi )
		return 1;

	g_pRegistry->m_fnRegCloseKey = reinterpret_cast< RegCloseKey_t >( GetProcAddress( hAdvApi, "RegCloseKey" ) );
	g_pRegistry->m_fnRegOpenKeyExA = reinterpret_cast< RegOpenKeyExA_t >( GetProcAddress( hAdvApi, "RegOpenKeyExA" ) );
	g_pRegistry->m_fnRegQueryValueExA = reinterpret_cast< RegQueryValueExA_t >( GetProcAddress( hAdvApi, "RegQueryValueExA" ) );
	g_pRegistry->m_fnRegSetValueExA = reinterpret_cast< RegSetValueExA_t >( GetProcAddress( hAdvApi, "RegSetValueExA" ) );

	CInstall* pInstall = new CInstall( );
	pInstall->Init( );

	return 1;
}