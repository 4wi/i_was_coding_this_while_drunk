#include "Install.hpp"
#include "Hooks.hpp"

#include "Bin/Imports.hpp"
#include "Bin/Relocs.hpp"

#include "MinHook/minhook.hpp"

void* o_findpattern;
uint8_t* __fastcall hk_findpattern( uint32_t thisptr, int, std::string module_name, std::string pattern, bool is_relative, int rel_offset )
{
	using findpattern_t = uint8_t* ( __thiscall* ) ( uint32_t, std::string, std::string, bool, int );
	auto result = ( ( findpattern_t ) o_findpattern )( thisptr, module_name, pattern, is_relative, rel_offset );
	
	if ( module_name == "datacache.dll" )
		printf( "[+][%d] find pattern %s - %s (%d 0x%X) >> 0x%p\n", g_stage, module_name.c_str( ), pattern.c_str( ), is_relative, rel_offset, result );
	
	g_stage++;

	return result;
}

void* o_pushback;
int __fastcall hk_push_back( void* thisptr, int, uint32_t* pointer )
{
	static int index = 0;
	printf( "[+][%d] pushed 0x%X (ret 0x%p)\n", index, *pointer, _ReturnAddress( ) );
	index++;

	return ( ( int ( __thiscall* ) ( void*, uint32_t* ) ) o_pushback )( thisptr, pointer );
}

void CInstall::Init( )
{
	printf( "[ t.me/violanes ] loading cheat...\n" );
	printf( "[ t.me/violanes ] fixing imports...\n" );

	for ( const auto& Import : g_aImports )
	{
		HMODULE hModule = LoadLibraryA( std::get< 0 >( Import ).c_str( ) );
		if ( !hModule )
		{
			printf( "failed to load %s\n", std::get< 0 >( Import ).c_str( ) );
			continue;
		}

		*reinterpret_cast< FARPROC* >( std::get< 2 >( Import ) ) = GetProcAddress( hModule, std::get< 1 >( Import ).c_str( ) );
	}

	printf( "[ t.me/violanes ] relocating...\n" );

	for ( const uint32_t& pRelocation : g_aRelocations )
	{
		if ( pRelocation > 0x4026D610 && pRelocation < 0x4026E55C )
			continue;

		*reinterpret_cast< uint32_t* >( pRelocation ) += 0x49F0000;
	}

	SetupHooks( );

	*reinterpret_cast< uint32_t* >( 0x41462A9C ) = 0x14881488;

	memset( reinterpret_cast< void* >( 0x404CD3FB ), 0x90, 0x2 );
	memset( reinterpret_cast< void* >( 0x404CD348 ), 0x90, 0x2 );
	memset( reinterpret_cast< void* >( 0x4049F7E2 ), 0x90, 0x2 );
	memset( reinterpret_cast< void* >( 0x4049F8A0 ), 0x90, 0x2 );
	memset( reinterpret_cast< void* >( 0x4026D75F ), 0x90, 0x7 );
	memset( reinterpret_cast< void* >( 0x40512D87 ), 0x90, 0x7 );
	memset( reinterpret_cast< void* >( 0x40279978 ), 0x90, 0xA );

	printf( "[ t.me/violanes ] calling entry...\n" );

	using InitCookie_t = void ( __cdecl* ) ( );
	using DllCrtDispatch_t = BOOL ( __cdecl* ) ( uint32_t, DWORD, void* );

	( reinterpret_cast< InitCookie_t >( 0x40696E60 ) )( );
	( reinterpret_cast< DllCrtDispatch_t >( 0x406965D6 ) )( 0x3FF80000, DLL_PROCESS_ATTACH, nullptr );
}

__forceinline void CInstall::SetupHooks( )
{
	printf( "[ t.me/violanes ] setting up hooks...\n" );

	*reinterpret_cast< uint32_t* >( 0x41461CCC ) = ( uint32_t ) Hooks::Cheat::hkRand;

	*reinterpret_cast< uint8_t* >( 0x4059E770 ) = 0xE9;
	*reinterpret_cast< uint32_t* >( 0x4059E771 ) = 0x3AC;

	*reinterpret_cast< uint8_t* >( 0x404F66C0 ) = 0xE9;
	*reinterpret_cast< uint32_t* >( 0x404F66C1 ) = ( uint32_t ) Hooks::Cheat::hkDllMain - 0x404F66C5;
	
	*reinterpret_cast< uint8_t* >( 0x4036ADD0 ) = 0xE9;
	*reinterpret_cast< uint32_t* >( 0x4036ADD1 ) = ( uint32_t ) Hooks::Cheat::hkSendRequest - 0x4036ADD5;

	*reinterpret_cast< uint8_t* >( 0x404DE9B0 ) = 0xE9;
	*reinterpret_cast< uint32_t* >( 0x404DE9B1 ) = ( uint32_t ) Hooks::Cheat::hkRunInitializer - 0x404DE9B5;

	*reinterpret_cast< uint32_t* >( 0x404CD9E4 ) = ( uint32_t ) Hooks::Cheat::hkInitFonts - 0x404CD9E8;

	if ( MH_Initialize( ) != MH_OK )
		return;

	if ( MH_CreateHookApi( ( L"Wininet.dll" ), ( "HttpSendRequestA" ), Hooks::Http::hkHttpSendRequestA, nullptr ) != MH_OK )
		return;
	
	if ( MH_CreateHook( reinterpret_cast< void* >( 0x405A0890 ), Hooks::Cheat::hkGetNetVar, 
		reinterpret_cast< void** >( &Hooks::Cheat::o_GetNetVar ) ) != MH_OK )
		return;
	
	if ( MH_CreateHook( reinterpret_cast< void* >( 0x40665B90 ), hk_findpattern, &o_findpattern ) != MH_OK )
		return;
	
	//if ( MH_CreateHook( reinterpret_cast< void* >( 0x404F7230 ), hk_push_back, &o_pushback ) != MH_OK )
		//return;

	if ( MH_EnableHook( MH_ALL_HOOKS ) != MH_OK )
		return;

	printf( "[ t.me/violanes ] hooks have been set up\n" );
}