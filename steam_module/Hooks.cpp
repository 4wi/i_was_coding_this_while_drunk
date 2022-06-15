#include "Hooks.hpp"
#include "Utils.hpp"

#include "MinHook/minhook.hpp"

#include <filesystem>
#include <sstream>

BOOL __stdcall Hooks::hkCreateProcessW( LPCWSTR lpApplicationName, LPWSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcAttr,
	LPSECURITY_ATTRIBUTES lpThreadAttr, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment,
	LPCWSTR lpCurrentDir, LPSTARTUPINFOW pStartupInfo, LPPROCESS_INFORMATION pProcessInfo )
{
	std::wstring wsApplicationName = std::wstring( lpCommandLine );

	std::string sApplicationName;
	g_Utils.WstringToString( wsApplicationName, sApplicationName );

	if ( !strstr( sApplicationName.c_str( ), "Counter-Strike Global Offensive\\csgo.exe" ) )
	{
		return o_CreateProcessW( lpApplicationName, lpCommandLine, lpProcAttr, lpThreadAttr, bInheritHandles,
			dwCreationFlags, lpEnvironment, lpCurrentDir, pStartupInfo, pProcessInfo );
	}

	BOOL bResult = o_CreateProcessW( lpApplicationName, lpCommandLine, lpProcAttr, lpThreadAttr, bInheritHandles,
		dwCreationFlags, lpEnvironment, lpCurrentDir, pStartupInfo, pProcessInfo );
	if ( !bResult )
		return bResult;

	HANDLE hProcess = pProcessInfo->hProcess;
	if( !hProcess )
	{
		MessageBoxA( 0, "Failed to init memory manager [0]", "Critical Error", MB_ICONERROR );
		return bResult;
	}

	std::vector< uint8_t > aBinary = { };
	g_Utils.OpenBinary( "C:\\Users\\lanes\\Documents\\projects\\csgo\\crecks\\monolith\\bin_cheat\\0x221B0000.bin", aBinary );

	void* pBase = VirtualAllocEx( hProcess, reinterpret_cast< void* >( 0x221B0000 ), aBinary.size( ), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE );
	if ( !pBase )
	{
		printf( "failed to alloc mem\n" );
		return bResult;
	}

	WriteProcessMemory( hProcess, pBase, aBinary.data( ), aBinary.size( ), 0 );

	printf( "mapped lol!\n" );

	return bResult;
}