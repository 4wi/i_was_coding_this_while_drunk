#include "Hooks.hpp"
#include "Binaries/Responses.hpp"

#include "Cheat/CCheat.hpp"

#include <fstream>
#include <thread>
#include <string>
#include <TlHelp32.h>
#include <algorithm>

namespace Hooks
{
	int m_iStage = 0;
}

LONG __stdcall ExceptionHandler( PEXCEPTION_POINTERS pException )
{
	if ( pException->ExceptionRecord->ExceptionCode == STATUS_HEAP_CORRUPTION )
		return EXCEPTION_CONTINUE_EXECUTION;

	return EXCEPTION_CONTINUE_SEARCH;
}

BOOL __stdcall Hooks::hkDllMain( HMODULE hModule, DWORD ulReason, LPVOID lpReserved )
{
	std::vector< uint32_t > aInitTerm =
	{
		0x221c09dd, 0x221c0a6e, 0x221c0a2a, 0x221c0a0b,
		0x221c0a64, 0x221c0a84, 0x221c0a9a, 0x221c09ff,
		0x221c09f3, 0x221b28d0, 0x221b28f0, 0x221b2910,
		0x221b2930, 0x221b2950, 0x221b2960, 0x221b2990,
		0x221b29c0, 0x221b29f0, 0x221b2a20, 0x221b2a30,
		0x221b2a80, 0x221b2aa0, 0x221b2ae0, 0x221b2af0,
		0x221b2b10, 0x221b2b40, 0x221b2b70, 0x221b2b90,
		0x221b2bb0, 0x221b2bd0, 0x221b2bf0, 0x221b2c30,
		0x221b2c50, 0x221b2c70, 0x221b2c90, 0x221b2cb0,
		0x221b2cd0, 0x221b2cf0, 0x221b2d20, 0x221b2d40,
		0x221b2d60, 0x221b2d80, 0x221b2db0, 0x221b2f90,
		0x221b2fc0, 0x221b2fe0, 0x221b2ff0, 0x221b3000,
		0x221b3020, 0x221b3030, 0x221b3040, 0x221b3050,
		0x221b3060, 0x221b3070, 0x221b3080, 0x221b3090,
		0x221b30b0, 0x221b30c0, 0x221b30d0, 0x221b30e0,
		0x221b30f0, 0x221b3100, 0x221b3110, 0x221b3130,
		0x221b3150, 0x221b3170, 0x221b3180, 0x221b31a0,
		0x221b31c0, 0x221b31e0, 0x221b3200, 0x221b3220,
		0x221b3230, 0x221b3250, 0x221b3260, 0x221b3270,
		0x221b3280, 0x221b3290, 0x221b32a0, 0x221b32b0,
		0x221b32c0, 0x221b32e0, 0x221b3310, 0x221b3330,
		0x221b33b0, 0x221b33d0, 0x221b33e0, 0x221b33f0,
		0x221b3400, 0x221b3410, 0x221b3420, 0x221b3430,
		0x221b3440, 0x221b3450, 0x221b3460, 0x221b3490,
		0x221b34d0, 0x221b3510, 0x221b3540, 0x221b3560,
		0x221bfa40, 0x221bfa90, 0x221bfae0, 0x221bfb00,
		0x221bfb10, 0x221bfb20, 0x221bfb30, 0x221bfb40,
		0x221bfb50, 0x221bfb60, 0x221bfb70, 0x221bfbc0,
		0x221bfbe0, 0x221bfc10, 0x221bfc30, 0x221bfc40,
		0x221bfc50, 0x221bfc70, 0x221bfc90, 0x221bfcb0,
		0x221bfcc0, 0x221bfcd0, 0x221bfce0, 0x221bfcf0,
		0x221bfd00, 0x221bfd10, 0x221bfd20, 0x221bfd40,
		0x221bfd50, 0x221bfd60, 0x221bfd70, 0x221bfd80,
		0x221bfd90, 0x221bfda0, 0x221bfdb0, 0x221bfdc0,
		0x221bfdd0, 0x221bfde0, 0x221bfdf0, 0x221bfe00,
		0x221bfe10, 0x221bfe20, 0x221c00a0, 0x221c00c0,
		0x221c00d0, 0x221c00e0, 0x221c00f0, 0x221c0100,
		0x221c0110, 0x221c0120, 0x221c0130, 0x221c0170,
		0x221c01b0, 0x221c01d0, 0x221c03d0, 0x221c03f0,
		0x221c0400, 0x221c0410, 0x221c0420, 0x221c0430,
		0x221c0440, 0x221c0450, 0x221c0460, 0x221c0480,
		0x221c04a0, 0x221c04b0, 0x221c04d0, 0x221c0570,
		0x221c0590, 0x221c05b0, 0x221c05e0, 0x221c0990
	};

	for ( const auto& CurrentAddress : aInitTerm )
		( reinterpret_cast< void ( __cdecl* ) ( ) >( CurrentAddress ) )( );

	CCheat* pCheat = new CCheat( );
	pCheat->Init( );

	return 1;
}

HANDLE __stdcall Hooks::hkCreateSnapshot( DWORD ulFlags, DWORD ulProcessID )
{
	if ( ulFlags & TH32CS_SNAPPROCESS )
		return nullptr;

	return CreateToolhelp32Snapshot( ulFlags, ulProcessID );
}

PVOID __stdcall Hooks::hkAddVectoredExceptionHandler( DWORD, void* )
{
	return AddVectoredExceptionHandler( INT_MAX, ExceptionHandler );
}

HANDLE __stdcall Hooks::hkCreateThread( void* pSecurity, DWORD ulStackSize, void* pThreadStart,
	void* pReserved, DWORD ulFlags, LPDWORD pTID )
{
	printf( "spawned thread 0x%X with 0x%p [0x%p]\n", pThreadStart, pReserved, _ReturnAddress( ) );

	if ( ( uint32_t ) pThreadStart == 0x226F8AD0 ) // govnosecure thread)))
		return 0;

	return CreateThread( ( LPSECURITY_ATTRIBUTES ) pSecurity, ulStackSize, 
		( LPTHREAD_START_ROUTINE ) pThreadStart, pReserved, ulFlags, pTID );
}

HINTERNET __stdcall Hooks::hkInternetOpenA( LPCSTR lpszAgent, DWORD dwAccessType,
	LPCSTR lpszProxy, LPCSTR lpszProxyBypass, DWORD dwFlags )
{
	//printf( "[ inet_open ] %s %s\n", lpszAgent, lpszProxy ); 
	return ( HINTERNET ) 0x14881488;
	//return o_InternetOpenA( lpszAgent, dwAccessType, lpszProxy, lpszProxyBypass, dwFlags );
}

HINTERNET __stdcall Hooks::hkInternetConnectA( HINTERNET hInternet, LPCSTR lpszServerName, INTERNET_PORT nServerPort, LPCSTR lpszUserName,
		LPCSTR lpszPassword, DWORD dwService, DWORD dwFlags, DWORD_PTR dwContext )
{
	//printf( "[ inet_conn ] %s %d %s %s\n", lpszServerName, nServerPort, lpszUserName, lpszPassword );
	//return o_InternetConnectA( hInternet, lpszServerName, nServerPort, lpszUserName, lpszPassword, dwService, dwFlags, dwContext );
	return ( HINTERNET ) 0x14881488;
}

HINTERNET __stdcall Hooks::hkHttpOpenRequestA( HINTERNET hInternet, LPCSTR lpszVerb, LPCSTR lpszObjectName, LPCSTR lpszVersion,
	LPCSTR lpszReferrer, LPCSTR* lplpszAcceptTypes, DWORD dwFlags, DWORD_PTR dwContext )
{
	//printf( "[ inet_open ] %s %s %s %s\n", lpszVerb, lpszObjectName, lpszVersion, lpszReferrer );
	//return o_HttpOpenRequestA( hInternet, lpszVerb, lpszObjectName, lpszVersion, lpszReferrer, lplpszAcceptTypes, dwFlags, dwContext );
	return ( HINTERNET ) 0x14881488;
}

BOOL __stdcall Hooks::hkHttpSendRequestA( HINTERNET hRequest, LPCSTR lpszHeaders, DWORD dwHeadersLength, LPVOID lpOptional, DWORD dwOptionalLength )
{
	//printf( "[inet_send] %s 0x%p (0x%lX) 0x%p\n", lpszHeaders, lpOptional, dwOptionalLength, _ReturnAddress( ) );
	//return o_HttpSendRequestA( hRequest, lpszHeaders, dwHeadersLength, lpOptional, dwOptionalLength );
	return 1;
}

BOOL __stdcall Hooks::hkInternetReadFile( HINTERNET hFile, LPVOID lpBuffer, DWORD dwNumberOfBytesToRead, LPDWORD lpdwNumberOfBytesRead )
{
	static int shit_counter = 0;
	static int previous_stage = m_iStage;

	if ( previous_stage != m_iStage )
	{
		shit_counter = 0;
		previous_stage = m_iStage;
	}

	if ( m_iStage > 0 && m_iStage < 13 )
	{
		*lpdwNumberOfBytesRead = std::clamp( g_aResponses[ m_iStage - 1 ].size( ) - shit_counter, 0u, ( uint32_t ) dwNumberOfBytesToRead );

		if ( *lpdwNumberOfBytesRead > 0 )
			memcpy( lpBuffer, ( uint8_t* )( ( uint32_t ) g_aResponses[ m_iStage - 1 ].data( ) + shit_counter ), *lpdwNumberOfBytesRead );

		shit_counter += *lpdwNumberOfBytesRead;
	}
	else
	{
		static bool memes = false;

		if ( !memes )
		{
			memset( lpBuffer, 0x10, dwNumberOfBytesToRead );
			*lpdwNumberOfBytesRead = dwNumberOfBytesToRead;
			memes = true;
		}
		else
		{
			*lpdwNumberOfBytesRead = 0;
			memes = false;
		}
	}

	return 1;
}

BOOL __stdcall Hooks::hkInternetCloseHandle( HINTERNET hInternet )
{
	//printf( "[inet_closehandle] closed handle from 0x%p\n", _ReturnAddress( ) );
	return 1;
	
	//return o_InternetCloseHandle( hInternet );
}