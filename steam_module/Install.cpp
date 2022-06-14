#include "Install.hpp"
#include "Hooks.hpp"

#include "MinHook/minhook.hpp"

#include <iostream>
#include <VirtualizerSDK.h>

void CInstall::Init( )
{
	VIRTUALIZER_LION_WHITE_START

	if ( MH_Initialize( ) != MH_OK )
	{
		printf( "failed to init hooking engine\n" );
		return;
	}

	if ( MH_CreateHookApi( L"kernel32.dll", "CreateProcessW", reinterpret_cast< void* >( Hooks::hkCreateProcessW ),
		reinterpret_cast< void** >( &Hooks::o_CreateProcessW ) ) != MH_OK )
	{
		printf( "failed to init hooking engine\n" );
		return;
	}
	
	if ( MH_EnableHook( MH_ALL_HOOKS ) != MH_OK )
	{
		printf( "failed to set hooking engine\n" );
		return;
	}

	printf( "init processed\n" );

	VIRTUALIZER_LION_WHITE_END
}