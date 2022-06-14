#include "CCheat.hpp"

#include "../Hooks.hpp"

#include "../Helpers/Exports.hpp"
#include "../Helpers/Utils.hpp"

#include <iostream>
#include <vector>
#include <Windows.h>
#include <TlHelp32.h>

typedef void* ( *CreateInterfaceFn ) ( const char* pName, int* pReturnCode );

class InterfaceReg
{
private:
	using InstantiateInterfaceFn = void* (*)();
public:
	InstantiateInterfaceFn m_CreateFn;
	const char* m_pName;
	InterfaceReg* m_pNext;
};

template< typename T >
T* GetInterface( HMODULE hModule, const char* szInterfaceVersion, bool bExact = true )
{
	T* iface = nullptr;
	InterfaceReg* register_list;
	int part_match_len = strlen(szInterfaceVersion);

	DWORD interface_fn = reinterpret_cast< DWORD >( GetProcAddress( hModule, ( "CreateInterface" ) ) );

	if (!interface_fn) {
		return nullptr;
	}

	unsigned int jump_start = (unsigned int)(interface_fn)+4;
	unsigned int jump_target = jump_start + *(unsigned int*)(jump_start + 1) + 5;

	register_list = **reinterpret_cast<InterfaceReg***>(jump_target + 6);

	for (InterfaceReg* cur = register_list; cur; cur = cur->m_pNext) {
		if (bExact == true) {
			if (strcmp(cur->m_pName, szInterfaceVersion) == 0)
				iface = reinterpret_cast<T*>(cur->m_CreateFn());
		}
		else {
			if (!strncmp(cur->m_pName, szInterfaceVersion, part_match_len) && std::atoi(cur->m_pName + part_match_len) > 0)
				iface = reinterpret_cast<T*>(cur->m_CreateFn());
		}
	}

	if ( !iface )
		printf( "failed to get interface %s\n", szInterfaceVersion );

	return iface;
}

int __cdecl get_interface( const char* module_name, const char* interface_name )
{
	auto result = GetInterface< void >( GetModuleHandleA( module_name ), interface_name, false );
	if ( !result )
	{
		result = GetInterface< void >( GetModuleHandleA( module_name ), interface_name, true );
		if ( !result )
			printf( "failed to obtain!\n" );
	}
	
	printf( "[+][%d] asked for interface %s - %s >> 0x%X\n", g_stage, module_name, interface_name, result );

	g_stage++;

	return ( int ) result;
}

void CCheat::Init( )
{
	using InitFunction_t = void ( __cdecl* ) ( );

	printf( "[ t.me/violanes ] initing...\n" );

	*reinterpret_cast< uint8_t* >( 0x4049FFFA ) = 0xCC; // send_packet createmvoe

	std::string sUsername = "violanes";
	memcpy( reinterpret_cast< void* >( 0x407E2E58 ), sUsername.c_str( ), sUsername.length( ) );

	*reinterpret_cast< void** >( 0x4080899C ) = malloc( 0x2C );

	*reinterpret_cast< uint32_t* >( 0x404CCADC ) = 0;
	*reinterpret_cast< uint32_t* >( 0x404CCAE3 ) = 0x3B4;
	*reinterpret_cast< uint32_t* >( 0x4049EE85 ) = 0x84;
	*reinterpret_cast< uint32_t* >( 0x404CCAF7 ) = 0;
	*reinterpret_cast< uint32_t* >( 0x404CCB90 ) = 0;
	*reinterpret_cast< uint32_t* >( 0x4049EEC3 ) = 0;
	*reinterpret_cast< uint32_t* >( 0x4049EEF4 ) = 0;
	*reinterpret_cast< uint32_t* >( 0x4049EF6C ) = 0;
	*reinterpret_cast< uint8_t* >( 0x4049F00A ) = 0;
	*reinterpret_cast< uint8_t* >( 0x4049f020 ) = 0;
	*reinterpret_cast< uint8_t* >( 0x4049f02f ) = 0;
	*reinterpret_cast< uint8_t* >( 0x4049F079 ) = 0;
	*reinterpret_cast< uint8_t* >( 0x4049F088 ) = 0;
	*reinterpret_cast< uint8_t* >( 0x4049F097 ) = 0;
	*reinterpret_cast< uint8_t* >( 0x4049F0A6 ) = 0;
	*reinterpret_cast< uint8_t* >( 0x4049F0B9 ) = 0;
	*reinterpret_cast< uint8_t* >( 0x4049f0d2 ) = 0;
	*reinterpret_cast< uint8_t* >( 0x4049f0e1 ) = 0;
	*reinterpret_cast< uint8_t* >( 0x4049f190 ) = 0;
	*reinterpret_cast< uint8_t* >( 0x4049f19f ) = 0;
	*reinterpret_cast< uint8_t* >( 0x4049f1ae ) = 0;
	*reinterpret_cast< uint8_t* >( 0x4049F1EB ) = 0;
	*reinterpret_cast< uint8_t* >( 0x4049F344 ) = 0;
	*reinterpret_cast< uint8_t* >( 0x4049f34e ) = 0;
	*reinterpret_cast< uint8_t* >( 0x4049F3BA ) = 0;
	*reinterpret_cast< uint8_t* >( 0x4049F3D6 ) = 0;
	*reinterpret_cast< uint8_t* >( 0x404CCB9E ) = 0;
	*reinterpret_cast< uint8_t* >( 0x4049EE95 ) = 0;
	*reinterpret_cast< uint8_t* >( 0x4049EF16 ) = 0;
	*reinterpret_cast< uint8_t* >( 0x4049eeb0 ) = 0;
	*reinterpret_cast< uint8_t* >( 0x4049eedf ) = 0;
	*reinterpret_cast< uint8_t* >( 0x4049ef2c ) = 0;
	*reinterpret_cast< uint8_t* >( 0x4049EF58 ) = 0;
	*reinterpret_cast< uint8_t* >( 0x4049f270 ) = 0;
	*reinterpret_cast< uint8_t* >( 0x4049F2CA ) = 0;
	*reinterpret_cast< uint8_t* >( 0x4049f360 ) = 0;
	*reinterpret_cast< uint16_t* >( 0x4049EF31 ) = 0;
	*reinterpret_cast< uint16_t* >( 0x4049F1F5 ) = 0;
	*reinterpret_cast< uint16_t* >( 0x4049F35b ) = 0;

	InitPointers( );
	SetupInterfaces( );

	( reinterpret_cast< InitFunction_t >( 0x404FEBE0 ) )( );

	SetupClassIDs( );
	SetupWeapons( );
	SetupAddresses( );

	( reinterpret_cast< InitFunction_t >( 0x404FE840 ) )( );

	// call 0x404FE7A0 ; setup grenades

	( reinterpret_cast< InitFunction_t >( 0x404FE700 ) )( );

	( reinterpret_cast< void ( __thiscall* ) ( uint32_t ) >( 0x404141E0 ) )( 0x4142C650 );
	
	SetupHooks( );

	printf( "[ t.me/violanes ] done!\n" );
}

__forceinline void CCheat::SetupInterfaces( )
{
	std::vector< uint32_t > nullptr_calls =
	{
		0x4054925E, 0x40549871, 0x4054A0C0, 0x4054B28B,
		0x4054BAE0, 0x4054C335, 0x4054C94E, 0x4054D1A3,
		0x4054ED3F, 0x4054F7D0, 0x40550028, 0x40550880,
		0x40550E99, 0x405514B5, 0x405578F0, 0x40559B4B,
		0x4055A3A3, 0x4055ABFB, 0x4055B217, 0x405423FC,
		0x405429D0, 0x40543D99, 0x405443AC, 0x40544BFB,
		0x4054520E, 0x40545A5D, 0x405462AC, 0x4054759A,
		0x40547DE9, 0x40548638, 0x40548C4B, 
	};

	for ( const auto& address : nullptr_calls )
		*reinterpret_cast< uint32_t* >( address + 0x1 ) = ( uint32_t ) get_interface - ( address + 0x5 );

	using shit_t = void ( __cdecl* ) ( );
	//( reinterpret_cast< shit_t >( 0x404FECE0 ) )( );

	uint8_t** pInterfaces = reinterpret_cast< uint8_t** >( malloc( 0xC8 ) );
	if ( !pInterfaces )
		return;

	pInterfaces[ 0 ] = GetInterface< uint8_t >( GetModuleHandleA( "client.dll" ), "VClient0", false );
	pInterfaces[ 1 ] = **( uint8_t*** )( **( uint32_t** ) pInterfaces[ 0 ] + 0x1F );
	pInterfaces[ 2 ] = **( uint8_t*** )( g_aInterfaces[ 0 ] );
	pInterfaces[ 4 ] = **( uint8_t*** )( *( uint32_t* )( *( uint32_t* ) pInterfaces[ 0 ] + 0x28 ) + 0x5 );
	pInterfaces[ 3 ] = GetInterface< uint8_t >( GetModuleHandleA( "server.dll" ), "ServerGameDLL0", false );
	pInterfaces[ 5 ] = GetInterface< uint8_t >( GetModuleHandleA( "vgui2.dll" ), "VGUI_Panel", false );
	pInterfaces[ 6 ] = GetInterface< uint8_t >( GetModuleHandleA( "client.dll" ), "VClientEntityList", false );
	pInterfaces[ 7 ] = GetInterface< uint8_t >( GetModuleHandleA( "engine.dll" ), "VEngineClient", false );
	pInterfaces[ 8 ] = GetInterface< uint8_t >( GetModuleHandleA( "engine.dll" ), "VEngineServer023" );
	pInterfaces[ 9 ] = GetInterface< uint8_t >( GetModuleHandleA( "inputsystem.dll" ), "InputSystemVersion", false );
	pInterfaces[ 10 ] = *( uint8_t** ) g_aInterfaces[ 1 ];
	pInterfaces[ 11 ] = GetInterface< uint8_t >( GetModuleHandleA( "vguimatsurface.dll" ), "VGUI_Surface", false );
	pInterfaces[ 12 ] = GetInterface< uint8_t >( GetModuleHandleA( "engine.dll" ), "EngineTraceClient", false );
	pInterfaces[ 13 ] = GetInterface< uint8_t >( GetModuleHandleA( "engine.dll" ), "EngineTraceServer", false );
	pInterfaces[ 14 ] = GetInterface< uint8_t >( GetModuleHandleA( "vstdlib.dll" ), "VEngineCvar", false );
	pInterfaces[ 15 ] = GetInterface< uint8_t >( GetModuleHandleA( "engine.dll" ), "VDebugOverlay", false );
	pInterfaces[ 17 ] = GetInterface< uint8_t >( GetModuleHandleA( "localize.dll" ), "Localize_001" );
	pInterfaces[ 16 ] = GetInterface< uint8_t >( GetModuleHandleA( "engine.dll" ), "VModelInfoClient", false );
	pInterfaces[ 18 ] = *( uint8_t** ) GetProcAddress( GetModuleHandleA( "tier0.dll" ), "g_pMemAlloc" );
	pInterfaces[ 19 ] = **( uint8_t*** ) g_aInterfaces[ 2 ];
	pInterfaces[ 20 ] = ( reinterpret_cast< uint8_t* ( __thiscall* ) ( void* ) >( 0x40269A70 ) )( malloc( 0x6C ) );
	pInterfaces[ 21 ] = GetInterface< uint8_t >( GetModuleHandleA( "engine.dll" ), "VEngineModel", false );
	pInterfaces[ 22 ] = GetInterface< uint8_t >( GetModuleHandleA( "materialsystem.dll" ), "VMaterialSystem", false );
	pInterfaces[ 23 ] = GetInterface< uint8_t >( GetModuleHandleA( "vphysics.dll" ), "VPhysicsSurfaceProps", false );
	pInterfaces[ 24 ] = GetInterface< uint8_t >( GetModuleHandleA( "client.dll" ), "GameMovement", false );
	pInterfaces[ 25 ] = GetInterface< uint8_t >( GetModuleHandleA( "client.dll" ), "VClientPrediction", false );
	pInterfaces[ 26 ] = **( uint8_t*** ) g_aInterfaces[ 3 ];
	pInterfaces[ 27 ] = **( uint8_t*** ) g_aInterfaces[ 4 ];
	pInterfaces[ 28 ] = GetInterface< uint8_t >( GetModuleHandleA( "engine.dll" ), "GAMEEVENTSMANAGER002" );
	pInterfaces[ 29 ] = GetInterface< uint8_t >( GetModuleHandleA( "filesystem_stdio.dll" ), "VBaseFileSystem011" );
	pInterfaces[ 33 ] = GetInterface< uint8_t >( GetModuleHandleA( "filesystem_stdio.dll" ), "VFileSystem017" );
	pInterfaces[ 31 ] = GetInterface< uint8_t >( GetModuleHandleA( "engine.dll" ), "VEngineClientStringTable001" );
	pInterfaces[ 32 ] = GetInterface< uint8_t >( GetModuleHandleA( "engine.dll" ), "VEngineEffects", false );
	pInterfaces[ 30 ] = GetInterface< uint8_t >( GetModuleHandleA( "client.dll" ), "GameUI011" );
	pInterfaces[ 34 ] = *( uint8_t** ) g_aInterfaces[ 5 ];
	pInterfaces[ 38 ] = *( uint8_t** ) g_aInterfaces[ 6 ];
	pInterfaces[ 39 ] = GetInterface< uint8_t >( GetModuleHandleA( "engine.dll" ), "VEngineRenderView", false );
	pInterfaces[ 40 ] = *( uint8_t** ) g_aInterfaces[ 7 ];
	pInterfaces[ 41 ] = *( uint8_t** ) g_aInterfaces[ 8 ];
	pInterfaces[ 42 ] = GetInterface< uint8_t >( GetModuleHandleA( "datacache.dll" ), "MDLCache", false );
	pInterfaces[ 43 ] = GetInterface< uint8_t >( GetModuleHandleA( "engine.dll" ), "IEngineSoundClient003" );
	pInterfaces[ 44 ] = GetInterface< uint8_t >( GetModuleHandleA( "panorama.dll" ), "PanoramaUIEngine", false );
	pInterfaces[ 45 ] = GetInterface< uint8_t >( GetModuleHandleA( "client.dll" ), "IEffects", false );
	pInterfaces[ 46 ] = *( uint8_t** ) g_aInterfaces[ 9 ];

	HMODULE hSteamAPI = GetModuleHandleA( "steam_api.dll" );
	if ( !hSteamAPI )
	{
		free( pInterfaces );
		return;
	}

	auto fnGetHSteamUser = GetProcAddress( hSteamAPI, "SteamAPI_GetHSteamUser" );
	auto fnGetPipe = GetProcAddress( hSteamAPI, "SteamAPI_GetHSteamPipe" );
	auto fnSteamClient = GetProcAddress( hSteamAPI, "SteamClient" );
	
	uint32_t** pSteamUser = ( reinterpret_cast< uint32_t** ( __cdecl* ) ( void ) >( fnGetHSteamUser ) )( );
	uint32_t** pSteamPipe = ( reinterpret_cast< uint32_t** ( __cdecl* ) ( void ) >( fnGetPipe ) )( );
	uint32_t** pSteamClient = ( reinterpret_cast< uint32_t** ( __cdecl* ) ( void ) >( fnSteamClient ) )( );

	pInterfaces[ 35 ] = ( reinterpret_cast< uint8_t* ( __thiscall* ) ( uint32_t**, uint32_t**, uint32_t**, const char* ) >( 
		pSteamClient[ 0 ][ 8 ] ) )( pSteamClient, pSteamUser, pSteamPipe, "SteamFriends015" );
	pInterfaces[ 36 ] = ( reinterpret_cast< uint8_t* ( __thiscall* ) ( uint32_t**, uint32_t**, const char* ) >( 
		pSteamClient[ 0 ][ 9 ] ) )( pSteamClient, pSteamPipe, "SteamUtils008" );
	pInterfaces[ 37 ] = ( reinterpret_cast< uint8_t* ( __thiscall* ) ( uint32_t**, uint32_t**, uint32_t**, const char* ) >( 
		pSteamClient[ 0 ][ 16 ] ) )( pSteamClient, pSteamUser, pSteamPipe, "SteamNetworking005" );

	*reinterpret_cast< FARPROC* >( 0x407E920C ) = GetProcAddress( GetModuleHandleA( "vstdlib.dll" ), "RandomSeed" );
	*reinterpret_cast< FARPROC* >( 0x407E9200 ) = GetProcAddress( GetModuleHandleA( "vstdlib.dll" ), "RandomFloat" );
	*reinterpret_cast< FARPROC* >( 0x407E9210 ) = GetProcAddress( GetModuleHandleA( "vstdlib.dll" ), "RandomFloatExp" );
	*reinterpret_cast< FARPROC* >( 0x407E9204 ) = GetProcAddress( GetModuleHandleA( "vstdlib.dll" ), "RandomInt" );
	*reinterpret_cast< FARPROC* >( 0x407E9208 ) = GetProcAddress( GetModuleHandleA( "vstdlib.dll" ), "RandomGaussianFloat" );

	std::map< size_t, size_t > aIndexRedirections =
	{
		std::make_pair( 0, 0 ),
		std::make_pair( 1, 1 ),
		// failed 0x15B836D7 2!
		std::make_pair( 3, 3 ),
		std::make_pair( 4, 4 ),
		std::make_pair( 5, 5 ),
		std::make_pair( 6, 6 ),
		std::make_pair( 31, 7 ),
		std::make_pair( 7, 8 ),
		std::make_pair( 8, 9 ),
		std::make_pair( 9, 10 ),
		std::make_pair( 10, 11 ),
		std::make_pair( 11, 12 ),
		std::make_pair( 12, 13 ),
		std::make_pair( 13, 14 ),
		std::make_pair( 14, 15 ),
		std::make_pair( 15, 16 ),
		std::make_pair( 17, 17 ),
		std::make_pair( 16, 18 ),
		std::make_pair( 18, 19 ),
		std::make_pair( 19, 20 ),
		std::make_pair( 20, 21 ),
		std::make_pair( 21, 22 ),
		std::make_pair( 22, 23 ),
		std::make_pair( 23, 24 ),
		std::make_pair( 24, 25 ),
		std::make_pair( 25, 26 ),
		std::make_pair( 26, 27 ),
		std::make_pair( 28, 29 ),
		std::make_pair( 38, 30 ),
		std::make_pair( 39, 31 ),
		std::make_pair( 40, 32 ),
		std::make_pair( 29, 33 ),
		std::make_pair( 33, 34 ),
		std::make_pair( 41, 35 ),
		std::make_pair( 32, 36 ),
		std::make_pair( 35, 37 ),
		std::make_pair( 36, 38 ),
		std::make_pair( 42, 39 ),
		std::make_pair( 34, 40 ),
		std::make_pair( 37, 41 ),
		std::make_pair( 43, 42 ),
		std::make_pair( 30, 43 ),
		std::make_pair( 44, 44 ),
		std::make_pair( 45, 45 ),
		std::make_pair( 46, 46 ),
	};

	*reinterpret_cast< uint8_t*** >( 0x40808990 ) = reinterpret_cast< uint8_t** >( malloc( 0xBC ) );

	for ( const auto& SwapInfo : aIndexRedirections )
		( *reinterpret_cast< uint8_t*** >( 0x40808990 ) )[ SwapInfo.second ] = pInterfaces[ SwapInfo.first ];

	( *reinterpret_cast< uint8_t*** >( 0x40808990 ) )[ 28 ] = **reinterpret_cast< uint8_t*** >( g_aInterfaces[ 10 ] );

	free( pInterfaces );
}

__forceinline void CCheat::SetupClassIDs( )
{
	uint8_t** pClassIDs = reinterpret_cast< uint8_t** >( malloc( 0x500 ) );
	if ( !pClassIDs )
		return;

	std::vector< uint32_t > aHashes =
	{
		0xb7883b54, 0x32813ad0, 0xa7d262c6, 0x942cf950,
		0x24ee1f1e, 0x8f1973b4, 0x7d4fc4bd, 0xd3e6a40c,
		0x7516b9ee, 0x2ef753c1, 0x768fd0c, 0xf90254d3,
		0x223242db, 0xa566f59c, 0xd0aeff7d, 0x72eca53d,
		0xd51a5b5d, 0xf7171da2, 0x9fb22315, 0x9ae52888,
		0x7dded792, 0x1a1b6a6c, 0xd7f7bc2f, 0x32b6f0e1,
		0x4979ab6e, 0x8c2f098c, 0x70e1e4f9, 0xe583bd27,
		0x64ea8400, 0xe4ffbc1b, 0xbd321dc7, 0xdde3c3e3,
		0x49de996, 0x4e9cb8ab, 0x965a5dfe, 0x381820fa,
		0x9eb06990, 0x76a10120, 0xf02bcb1e, 0xd7090ee,
		0xb71ef788, 0x16c5d402, 0xe6628350, 0xce79c94c,
		0x1166b035, 0x4d762f23, 0xcd60e7ba, 0x4761ab87,
		0x2748241d, 0x394c6bfa, 0xd291d099, 0x1ed91e7,
		0xb32008a7, 0xac54e3bd, 0x5358dcbd, 0x2859c997,
		0xa1d205f2, 0x1dc51b5a, 0x268f77a6, 0x9acb6a7,
		0x1d9689c5, 0x8d7b0140, 0x509a5bdb, 0x970c7ab5,
		0x759cc71d, 0x78dd608b, 0x18c9155c, 0xafcc24a2,
		0xba015744, 0x61c0153c, 0x8f9c824, 0x8824af94,
		0xd42c2219, 0x26c49787, 0xede26d02, 0x1fc1b321,
		0xbd5c54a9, 0xa037180e, 0x8ca40aba, 0x2e20e87,
		0x3b91c340, 0x18702b57, 0x646299fa, 0x99e4c843,
		0xd736aba1, 0x7e0e5a6f, 0xbcce83ac, 0x790d1980,
		0xfe13e23f, 0x2bd7cdb4, 0x4bc8c428, 0xdad2788a,
		0x98df2266, 0x9b16e763, 0x770622b2, 0x908f376c,
		0x1677eb74, 0xbb6aca45, 0x7134339a, 0xb6945314,
		0x9addd913, 0xb9895322, 0x44f39cb, 0x21e89f5,
		0x9da4db33, 0x37fb20c8, 0x7bde450a, 0x28ce4480,
		0x8fef305d, 0x19ede519, 0xd721020d, 0xa46b008c,
		0xec7c6a83, 0x579c8d7, 0x31383b8b, 0x770cd587,
		0x9677b6de, 0x41aaa1b8, 0xcd3ec70a, 0xf69193b5,
		0x9cb32d45, 0xa94c47fb, 0xec7e41b1, 0x6dec9857,
		0xb6b6b48a, 0xb1b8c006, 0xe11a575a, 0xe0c69817,
		0xff0dd70c, 0xa92176f0, 0x967c400e, 0xd61df2c,
		0x2193a440, 0xe3fd5a06, 0xccbc6cba, 0x7110078a,
		0x4a1a865b, 0x3cfae5a, 0xb71f792e, 0xd1331b98,
		0x8339e8fc, 0x67f5624c, 0x3e68bc3f, 0x214a0703,
		0x41000dc7, 0x89d0ba65, 0xb1407d78, 0xd561bc52,
		0xaee8a0f9, 0x724e2cda, 0x107f17cc, 0x108ea97b,
		0xb53bfd18, 0xf3db2e93, 0xf8ace0f6, 0x15af85bf,
		0x140a098f, 0x24fc1818, 0xd43033b2, 0xe812160,
		0x90163410, 0xe487596c, 0x46515d92, 0xf786e164,
		0x137c51ea, 0x65f2f227, 0xb3cfe412, 0x1e2304e1,
		0xe19d6ded, 0xda5506c8, 0x667f2290, 0x3c988ad2,
		0xc019f554, 0x78761b14, 0x1998316f, 0x7a69a3cc,
		0x7ced3fa2, 0x4c1789e, 0xa4a92793, 0x1e81582d,
		0x887cd000, 0x8ca511dd, 0x352ed00c, 0xf438755e,
		0x6f914428, 0x34c21fdf, 0xd39bfe36, 0x6f1e5aee,
		0x5e082fb5, 0xbb1b8f26, 0x192780f1, 0x4fb7ad65,
		0xdc668105, 0x6d0f8463, 0xa92041e1, 0x2a1932bf,
		0xaa849722, 0xa8881f5c, 0xb33f70aa, 0x90b4e09e,
		0x9829188c, 0xd135a1e5, 0x539d6563, 0x69fc02a5,
		0x275a3b1d, 0xd48fa525, 0x57a874b8, 0xdfcd3e1d,
		0xda7fd794, 0x20b77cce, 0xfd54d1b, 0x755ae356,
		0x3aa8636, 0x7abbed2d, 0x29f19622, 0xfb95612d,
		0xa4180948, 0x1d380af7, 0xb2a08f52, 0x8b56bf7b,
		0xe9503d9e, 0x7959b51f, 0x9467909, 0x27c2cae2,
		0x83970610, 0x409293e5, 0x6e88841a, 0x3a764e1e,
		0x54259302, 0x4289c365, 0x3d77f21b, 0x4d1c4d8d,
		0x9de80b66, 0xf8cd29b6, 0x80574af9, 0xfd78c902,
		0x451faaa1, 0xe7d918fc, 0x7ba54d36, 0x61554adc,
		0xe5510013, 0xb8e18d8b, 0xe59ba27c, 0xfa84adbd,
		0x82f80351, 0x5c6b56a6, 0x4f82d4df, 0xba6cb5d1,
		0x4fcf03d1, 0x379fac3b, 0x3d9fdb49, 0x862e7b9c,
		0x9decc35f, 0x84fe326d, 0x8ef4ffea, 0x59dd3574,
		0xb0c52414, 0x2dac810a, 0x150631ff, 0xa7a6eed5,
		0xa5a6e98b, 0xb9a6e747, 0xcdac278e, 0x156c3c32,
		0x6d88f3b2, 0xec12bc14, 0xbe3a3d38, 0x94550e6d,
		0xe82f3146, 0x7635f44d, 0x299978e7, 0x6084c438,
		0x4d29713d, 0x597bc4e5, 0x5f14d568, 0xc4324771,
		0x3d177ed6, 0x95fce40d,
	};

	using GetOffset_t = uint8_t* ( __thiscall* ) ( void* thisptr, uint32_t iHash );
	for ( size_t i = 0; i < aHashes.size( ); i++ )
		pClassIDs[ i ] = ( reinterpret_cast< GetOffset_t >( 0x4024CCA0 ) )( this, aHashes[ i ] ^ 0x14881337 );

	*reinterpret_cast< uint8_t*** >( 0x407E80D8 ) = pClassIDs;

	printf( "[ t.me/violanes ] inited offsets\n" );
}

__forceinline void CCheat::SetupWeapons( )
{
	uint8_t* pWeaponManager = ( reinterpret_cast< uint8_t* ( __cdecl* ) ( void ) >( g_aAddresses[ 0 ] ) )( ) + 0x4;
	uint8_t** pWeapons = reinterpret_cast< uint8_t** >( malloc( 0x180 ) );

	std::vector< uint32_t > aHashes =
	{
		0x7e231775, 0x2e599db2, 0x991d098c, 0xc24a99db,
		0x2c2c7c50, 0xc09bd8b2, 0xfb85eff5, 0x49fe1851,
		0xf007e1a6, 0x36d57e03, 0xeaae614d, 0x4758eab6,
		0xa208816f, 0x772b6324, 0xd2027f32, 0xbece0048,
		0x7357fb36, 0x1f4ee4bd, 0x331bca1, 0xc48d940c,
		0x6c313a84, 0x4e5e1122, 0x6e6d1322, 0x3acb2f2c,
		0x4b75ae6b, 0x4d75d5f9, 0x3a409e8f, 0xb501a55a,
		0xe639a90c, 0x9588175a, 0xfcd63ff7, 0xfe88c6de,
		0xa1059a6a, 0x883fae68, 0x2ffbae41, 0x521e3292,
		0x867d0098, 0x53d3f8db, 0x9541ae39, 0x48467c03,
		0x6182de36, 0x9752be31, 0x759a05f8, 0x93f09a2f,
		0xc98a521f, 0xab154976, 0x911e68ee, 0xb3198e53,
		0xbd9f28ab, 0xdab569da, 0x4a54cc93, 0x736af4c5,
		0x2a80285e, 0xee718eaa, 0x623689a2, 0xb3dbd1c0,
		0xb881f98d, 0x97ca01b9, 0xe6fa4781, 0xf01c611f,
		0x636a5480, 0x42dbe9f8, 0xcf720c29, 0x53049288,
		0x87319bdc, 0x80cf675f, 0x77391ea2, 0x918bacf5,
		0x7c91ef0c, 0x206486fb, 0x856cd614, 0x74535f5a,
		0x6a2dc84a, 0x3f527075, 0x102533f0, 0x94539f0c,
		0x74f9f413, 0x716e0dbf, 0x123dcdb0, 0xc9557b09,
		0x647208a5, 0x15b3d41d, 0xa5487a71, 0x35580662,
		0x87f98b00, 0x7566e7ed, 0x76565f80, 0x4eddc3d0,
		0x46f65f15,
	};

	using GetWeapon_t = uint8_t* ( __thiscall* ) ( void* thisptr, uint8_t* pWeaponMgr, uint32_t iHash );
	for ( size_t i = 0; i < aHashes.size( ); i++ )
		pWeapons[ i ] = ( reinterpret_cast< GetWeapon_t >( 0x40566970 ) )( this, pWeaponManager, aHashes[ i ] ^ 0x13371337 );

	*reinterpret_cast< uint8_t*** >( 0x40808998 ) = pWeapons;
}

__forceinline void CCheat::SetupAddresses( )
{
	uint8_t** pAddresses = reinterpret_cast< uint8_t** >( malloc( 0x400 ) );
	if ( !pAddresses )
		return;

	// TODO: fix static offsets below
	pAddresses[ 17 ] = ( uint8_t* ) 0x2914;
	pAddresses[ 33 ] = ( uint8_t* ) 0xA6;
	pAddresses[ 50 ] = ( uint8_t* ) 0x9DC;
	pAddresses[ 54 ] = ( uint8_t* ) 0x32C4;
	pAddresses[ 55 ] = ( uint8_t* ) 0x32C0;
	pAddresses[ 56 ] = ( uint8_t* ) 0x230;

	pAddresses[ 0 ] = g_aOffsets[ 0 ];
	pAddresses[ 1 ] = **( uint8_t*** ) g_aOffsets[ 1 ];
	pAddresses[ 2 ] = *( uint8_t** ) g_aOffsets[ 2 ];
	pAddresses[ 3 ] = *( uint8_t** ) g_aOffsets[ 3 ];
	pAddresses[ 4 ] = *( uint8_t** ) g_aOffsets[ 4 ];
	pAddresses[ 5 ] = *( uint8_t** ) g_aOffsets[ 5 ];
	pAddresses[ 6 ] = g_aOffsets[ 6 ];
	pAddresses[ 7 ] = g_aOffsets[ 7 ];
	pAddresses[ 8 ] = g_aOffsets[ 8 ];
	pAddresses[ 9 ] = g_aOffsets[ 9 ];
	pAddresses[ 10 ] = g_aOffsets[ 10 ];
	pAddresses[ 11 ] = g_aOffsets[ 11 ];
	pAddresses[ 12 ] = g_aOffsets[ 12 ];
	pAddresses[ 14 ] = g_aOffsets[ 13 ];
	pAddresses[ 15 ] = g_aOffsets[ 14 ];
	pAddresses[ 16 ] = g_aOffsets[ 15 ];
	pAddresses[ 18 ] = g_aOffsets[ 16 ];
	pAddresses[ 19 ] = g_aOffsets[ 17 ];
	pAddresses[ 20 ] = g_aOffsets[ 18 ];
	pAddresses[ 21 ] = g_aOffsets[ 19 ];
	pAddresses[ 22 ] = g_aOffsets[ 20 ];
	pAddresses[ 23 ] = g_aOffsets[ 21 ];
	pAddresses[ 24 ] = g_aOffsets[ 22 ];
	pAddresses[ 25 ] = *( uint8_t** )( g_aOffsets[ 24 ] + 0x1 ) + ( ( uint32_t ) g_aOffsets[ 24 ] + 0x5 );
	pAddresses[ 27 ] = *( uint8_t** )( g_aOffsets[ 26 ] + 0x1 ) + ( ( uint32_t ) g_aOffsets[ 26 ] + 0x5 );
	pAddresses[ 59 ] = g_aOffsets[ 23 ];
	pAddresses[ 26 ] = g_aOffsets[ 25 ];
	pAddresses[ 28 ] = g_aOffsets[ 27 ];
	pAddresses[ 29 ] = g_aOffsets[ 28 ];
	pAddresses[ 30 ] = g_aOffsets[ 29 ];
	pAddresses[ 32 ] = g_aOffsets[ 30 ];
	pAddresses[ 31 ] = *( uint8_t** ) g_aOffsets[ 31 ];
	pAddresses[ 34 ] = *( uint8_t** ) g_aOffsets[ 32 ];
	pAddresses[ 35 ] = g_aOffsets[ 33 ];
	pAddresses[ 47 ] = g_aOffsets[ 34 ];
	pAddresses[ 36 ] = g_aOffsets[ 35 ];
	pAddresses[ 38 ] = g_aOffsets[ 36 ];
	pAddresses[ 39 ] = g_aOffsets[ 37 ];
	pAddresses[ 45 ] = g_aOffsets[ 38 ];
	pAddresses[ 40 ] = g_aOffsets[ 39 ];
	pAddresses[ 41 ] = g_aOffsets[ 40 ];
	pAddresses[ 43 ] = g_aOffsets[ 41 ];
	pAddresses[ 37 ] = g_aOffsets[ 42 ];
	pAddresses[ 44 ] = g_aOffsets[ 43 ];
	pAddresses[ 42 ] = g_aOffsets[ 44 ];
	pAddresses[ 48 ] = *( uint8_t** ) g_aOffsets[ 45 ];
	pAddresses[ 49 ] = *( uint8_t** ) g_aOffsets[ 46 ];
	pAddresses[ 51 ] = *( uint8_t** ) g_aOffsets[ 48 ];
	pAddresses[ 52 ] = *( uint8_t** ) g_aOffsets[ 49 ];
	pAddresses[ 53 ] = g_aOffsets[ 50 ];
	pAddresses[ 57 ] = g_aOffsets[ 53 ];
	pAddresses[ 58 ] = *( uint8_t** ) g_aOffsets[ 54 ];
	pAddresses[ 60 ] = *( uint8_t** ) g_aOffsets[ 55 ];
	pAddresses[ 61 ] = *( uint8_t** ) g_aOffsets[ 56 ];
	pAddresses[ 62 ] = *( uint8_t** ) g_aOffsets[ 57 ];
	pAddresses[ 63 ] = *( uint8_t** )( g_aOffsets[ 58 ] + 0x1 ) + ( ( uint32_t ) g_aOffsets[ 58 ] + 0x5 );
	pAddresses[ 64 ] = *( uint8_t** )( g_aOffsets[ 59 ] + 10 ) + ( ( uint32_t ) g_aOffsets[ 59 ] + 15 );
	pAddresses[ 65 ] = g_aOffsets[ 60 ];
	pAddresses[ 66 ] = g_aOffsets[ 61 ];
	pAddresses[ 67 ] = g_aOffsets[ 62 ];
	pAddresses[ 68 ] = g_aOffsets[ 63 ];
	pAddresses[ 69 ] = g_aOffsets[ 64 ];
	pAddresses[ 70 ] = g_aOffsets[ 65 ];
	pAddresses[ 71 ] = g_aOffsets[ 66 ];
	pAddresses[ 72 ] = g_aOffsets[ 67 ];
	pAddresses[ 73 ] = g_aOffsets[ 68 ];
	pAddresses[ 74 ] = g_aOffsets[ 69 ];
	pAddresses[ 75 ] = g_aOffsets[ 70 ];
	pAddresses[ 76 ] = g_aOffsets[ 71 ];
	pAddresses[ 77 ] = g_aOffsets[ 72 ];
	pAddresses[ 78 ] = g_aOffsets[ 73 ];
	pAddresses[ 79 ] = g_aOffsets[ 74 ];
	pAddresses[ 80 ] = g_aOffsets[ 75 ];
	pAddresses[ 82 ] = g_aOffsets[ 76 ];
	pAddresses[ 83 ] = g_aOffsets[ 77 ];
	pAddresses[ 84 ] = g_aOffsets[ 78 ];
	pAddresses[ 85 ] = g_aOffsets[ 79 ];
	pAddresses[ 86 ] = g_aOffsets[ 80 ];
	pAddresses[ 87 ] = g_aOffsets[ 81 ];
	pAddresses[ 88 ] = g_aOffsets[ 82 ];
	pAddresses[ 89 ] = g_aOffsets[ 83 ];
	pAddresses[ 90 ] = g_aOffsets[ 84 ];
	pAddresses[ 91 ] = g_aOffsets[ 85 ];
	pAddresses[ 92 ] = g_aOffsets[ 86 ];
	pAddresses[ 93 ] = g_aOffsets[ 87 ];
	pAddresses[ 94 ] = g_aOffsets[ 88 ];
	pAddresses[ 95 ] = g_aOffsets[ 89 ];
	pAddresses[ 96 ] = g_aOffsets[ 90 ];
	pAddresses[ 97 ] = g_aOffsets[ 91 ];
	pAddresses[ 98 ] = g_aOffsets[ 92 ];
	pAddresses[ 99 ] = g_aOffsets[ 93 ];
	pAddresses[ 100 ] = g_aOffsets[ 94 ];
	pAddresses[ 101 ] = g_aOffsets[ 95 ];
	pAddresses[ 102 ] = g_aOffsets[ 96 ];
	pAddresses[ 103 ] = g_aOffsets[ 97 ];
	pAddresses[ 104 ] = g_aOffsets[ 98 ];
	pAddresses[ 105 ] = g_aOffsets[ 99 ];
	pAddresses[ 106 ] = g_aOffsets[ 100 ];
	pAddresses[ 107 ] = g_aOffsets[ 101 ];
	pAddresses[ 108 ] = g_aOffsets[ 102 ];
	pAddresses[ 109 ] = g_aOffsets[ 103 ];
	pAddresses[ 110 ] = g_aOffsets[ 104 ];
	pAddresses[ 111 ] = g_aOffsets[ 105 ];
	pAddresses[ 107 ] = g_aOffsets[ 106 ];
	pAddresses[ 113 ] = g_aOffsets[ 107 ];
	pAddresses[ 114 ] = g_aOffsets[ 108 ];
	pAddresses[ 115 ] = g_aOffsets[ 109 ];
	pAddresses[ 116 ] = g_aOffsets[ 110 ];
	pAddresses[ 117 ] = g_aOffsets[ 111 ];
	pAddresses[ 118 ] = g_aOffsets[ 112 ];
	pAddresses[ 119 ] = g_aOffsets[ 113 ];
	pAddresses[ 120 ] = *( uint8_t** )( g_aOffsets[ 114 ] + 0x2 ) + ( ( uint32_t ) g_aOffsets[ 114 ] + 0x6 );
	pAddresses[ 121 ] = g_aOffsets[ 115 ];
	pAddresses[ 122 ] = g_aOffsets[ 116 ];
	pAddresses[ 123 ] = g_aOffsets[ 117 ];
	pAddresses[ 124 ] = g_aOffsets[ 118 ];
	pAddresses[ 125 ] = g_aOffsets[ 119 ];
	pAddresses[ 126 ] = g_aOffsets[ 120 ];
	pAddresses[ 127 ] = g_aOffsets[ 121 ];
	pAddresses[ 128 ] = g_aOffsets[ 122 ];
	pAddresses[ 129 ] = g_aOffsets[ 123 ];
	pAddresses[ 130 ] = g_aOffsets[ 124 ];
	pAddresses[ 131 ] = g_aOffsets[ 125 ];
	pAddresses[ 132 ] = g_aOffsets[ 126 ];
	pAddresses[ 133 ] = g_aOffsets[ 127 ];
	pAddresses[ 134 ] = g_aOffsets[ 128 ];
	pAddresses[ 135 ] = g_aOffsets[ 129 ];
	pAddresses[ 136 ] = g_aOffsets[ 130 ];
	pAddresses[ 137 ] = g_aOffsets[ 131 ];
	pAddresses[ 138 ] = g_aOffsets[ 132 ];
	pAddresses[ 139 ] = g_aOffsets[ 133 ];
	pAddresses[ 140 ] = g_aOffsets[ 134 ];
	pAddresses[ 142 ] = g_aOffsets[ 135 ];
	pAddresses[ 143 ] = g_aOffsets[ 136 ];
	pAddresses[ 144 ] = g_aOffsets[ 137 ];
	pAddresses[ 145 ] = g_aOffsets[ 138 ];
	pAddresses[ 165 ] = g_aOffsets[ 139 ];
	pAddresses[ 146 ] = g_aOffsets[ 140 ];
	pAddresses[ 147 ] = g_aOffsets[ 141 ];
	pAddresses[ 148 ] = g_aOffsets[ 142 ];
	pAddresses[ 149 ] = g_aOffsets[ 143 ];
	pAddresses[ 150 ] = g_aOffsets[ 144 ];
	pAddresses[ 152 ] = g_aOffsets[ 145 ];
	pAddresses[ 153 ] = g_aOffsets[ 146 ];
	pAddresses[ 154 ] = g_aOffsets[ 147 ];
	pAddresses[ 156 ] = g_aOffsets[ 149 ];
	pAddresses[ 157 ] = g_aOffsets[ 150 ];
	pAddresses[ 158 ] = g_aOffsets[ 151 ];
	pAddresses[ 160 ] = g_aOffsets[ 153 ];
	pAddresses[ 161 ] = g_aOffsets[ 154 ];
	pAddresses[ 162 ] = g_aOffsets[ 155 ];
	pAddresses[ 163 ] = g_aOffsets[ 156 ];
	pAddresses[ 164 ] = g_aOffsets[ 157 ];
	pAddresses[ 166 ] = g_aOffsets[ 158 ];
	pAddresses[ 167 ] = g_aOffsets[ 159 ];
	pAddresses[ 168 ] = g_aOffsets[ 160 ];
	pAddresses[ 169 ] = g_aOffsets[ 161 ];
	pAddresses[ 170 ] = g_aOffsets[ 162 ];
	pAddresses[ 171 ] = *( uint8_t** )( g_aOffsets[ 163 ] + 0x1 ) + ( ( uint32_t ) g_aOffsets[ 163 ] + 0x5 );
	pAddresses[ 172 ] = g_aOffsets[ 164 ];
	pAddresses[ 173 ] = g_aOffsets[ 165 ];
	pAddresses[ 174 ] = g_aOffsets[ 166 ];
	pAddresses[ 175 ] = g_aOffsets[ 167 ];
	pAddresses[ 176 ] = g_aOffsets[ 168 ];
	pAddresses[ 177 ] = g_aOffsets[ 169 ];
	pAddresses[ 178 ] = g_aOffsets[ 170 ];
	pAddresses[ 179 ] = g_aOffsets[ 171 ];
	pAddresses[ 180 ] = g_aOffsets[ 172 ];
	pAddresses[ 181 ] = g_aOffsets[ 173 ];
	pAddresses[ 182 ] = g_aOffsets[ 174 ];
	pAddresses[ 183 ] = g_aOffsets[ 175 ];
	pAddresses[ 184 ] = g_aOffsets[ 176 ];
	pAddresses[ 185 ] = g_aOffsets[ 177 ];
	pAddresses[ 12 ] = g_aOffsets[ 178 ];
	pAddresses[ 187 ] = g_aOffsets[ 179 ];
	pAddresses[ 188 ] = g_aOffsets[ 180 ];
	pAddresses[ 189 ] = g_aOffsets[ 181 ];
	pAddresses[ 190 ] = g_aOffsets[ 182 ];
	pAddresses[ 191 ] = g_aOffsets[ 183 ];
	pAddresses[ 192 ] = g_aOffsets[ 184 ];
	pAddresses[ 193 ] = g_aOffsets[ 185 ];
	pAddresses[ 194 ] = g_aOffsets[ 186 ];
	pAddresses[ 195 ] = g_aOffsets[ 187 ];
	pAddresses[ 196 ] = g_aOffsets[ 188 ];
	pAddresses[ 197 ] = g_aOffsets[ 189 ];
	pAddresses[ 198 ] = g_aOffsets[ 190 ];
	pAddresses[ 199 ] = g_aOffsets[ 191 ];
	pAddresses[ 200 ] = g_aOffsets[ 192 ];
	pAddresses[ 201 ] = g_aOffsets[ 193 ];
	pAddresses[ 202 ] = g_aOffsets[ 194 ];
	pAddresses[ 203 ] = g_aOffsets[ 195 ];
	pAddresses[ 204 ] = g_aOffsets[ 196 ];
	pAddresses[ 205 ] = g_aOffsets[ 197 ];
	pAddresses[ 206 ] = g_aOffsets[ 198 ];
	pAddresses[ 208 ] = g_aOffsets[ 199 ];
	pAddresses[ 209 ] = g_aOffsets[ 200 ];
	pAddresses[ 210 ] = g_aOffsets[ 201 ];
	pAddresses[ 159 ] = g_aOffsets[ 152 ];
	pAddresses[ 155 ] = g_aOffsets[ 148 ];

	*reinterpret_cast< uint8_t*** >( 0x408089B8 ) = pAddresses;

	printf( "addresses inited\n" );
}

__forceinline void CCheat::SetupHooks( )
{
	//printf( "[ t.me/violanes ] hooking...\n" );

	( reinterpret_cast< void ( __cdecl* ) ( ) >( 0x404FE600 ) )( ); // todo: SetupHooks 

	//printf( "[ t.me/violanes ] hooks have been set up\n" );
}

__forceinline void CCheat::InitPointers( )
{
	g_aInterfaces[ 0 ] = g_Utils.FindPattern( "server.dll", "A1 ? ? ? ? 8B 75 0C F3 0F 10 40 ? F3 0F 5C 44 24 ? F3 0F 2C F8" ) + 0x1;
	g_aInterfaces[ 1 ] = g_Utils.FindPattern( "client.dll", "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10" ) + 0x1;
	g_aInterfaces[ 2 ] = g_Utils.FindPattern( "engine.dll", "8B 0D ? ? ? ? 8B 01 8B 40 1C FF D0 84 C0 74 0A" ) + 0x2;
	g_aInterfaces[ 3 ] = g_Utils.FindPattern( "client.dll", "8B 0D ? ? ? ? 8B 46 08 68" ) + 0x2;
	g_aInterfaces[ 4 ] = g_Utils.FindPattern( "engine.dll", "8B 35 ? ? ? ? 83 C6 08 83 BE ? ? ? ? ? 75" ) + 0x2;
	g_aInterfaces[ 5 ] = g_Utils.FindPattern( "client.dll", "8B 0D ? ? ? ? FF B3 ? ? ? ? FF 77 08 8B 01 8B 40 70" ) + 0x2;
	g_aInterfaces[ 6 ] = g_Utils.FindPattern( "client.dll", "B9 ? ? ? ? 50 FF 56 08 85 C0 74 2D" ) + 0x1;
	g_aInterfaces[ 7 ] = g_Utils.FindPattern( "vguimatsurface.dll", "B9 ? ? ? ? 6A 02 6A 00 FF 75 14 FF 75 10 FF 75 0C FF 75 08 FF 50 30" ) + 0x1;
	g_aInterfaces[ 8 ] = g_Utils.FindPattern( "client.dll", "B9 ? ? ? ? A1 ? ? ? ? FF 10 A1 ? ? ? ? B9" ) + 0x1;
	g_aInterfaces[ 9 ] = g_Utils.FindPattern( "engine.dll", "A1 ? ? ? ? B9 ? ? ? ? 8B 40 28 FF D0 83 7B 0C 00" ) + 0x1;
	g_aInterfaces[ 10 ] = g_Utils.FindPattern( "engine.dll", "A1 ? ? ? ? 8B 80 ? ? ? ? C3" ) + 0x1;

	g_aAddresses[ 0 ] = g_Utils.FindPattern( "client.dll", "A1 ? ? ? ? 85 C0 75 53" );

	g_aOffsets[ 0 ] = g_Utils.FindPattern( "engine.dll", "8B 35 ? ? ? ? 83 C6 08 83 BE ? ? ? ? ? 75" ) + 0x2;
	g_aOffsets[ 1 ] = g_Utils.FindPattern( "client.dll", "8B 0D ? ? ? ? 8B 01 FF 50 2C 33 C9 85 FF 0F 44 F1 8B C8 8B 10 56 FF 52 04 F6 47 0C 01" ) + 0x2;
	g_aOffsets[ 2 ] = g_Utils.FindPattern( "client.dll", "8B 3D ? ? ? ? 85 FF 0F 84 ? ? ? ? 81 C7" ) + 0x2;
	g_aOffsets[ 3 ] = g_Utils.FindPattern( "client.dll", "8B 8E ? ? ? ? 85 C9 74 3E" ) + 0x2;
	g_aOffsets[ 4 ] = g_Utils.FindPattern( "client.dll", "8B 89 ? ? ? ? 8D 0C D1 3B D9 74 14" ) + 0x2;
	g_aOffsets[ 5 ] = g_Utils.FindPattern( "client.dll", "8B B9 ? ? ? ? 66 0F 1F 44 00 ? 8B 0E" ) + 0x2;
	g_aOffsets[ 6 ] = g_Utils.FindPattern( "client.dll", "56 6A 01 68 ? ? ? ? 8B F1" );
	g_aOffsets[ 7 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24" );
	g_aOffsets[ 8 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 83 E4 F8 83 EC 30 56 57 8B 3D" );
	g_aOffsets[ 9 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 56 8B F1 B9 ? ? ? ? C7 46" );
	g_aOffsets[ 10 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1 E8" );
	g_aOffsets[ 11 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 56 8B F1 51 8D 4E FC" );
	g_aOffsets[ 12 ] = g_Utils.FindPattern( "client.dll", "80 3D ? ? ? ? ? 74 16 A1 ? ? ? ? 48 C7 81" );
	g_aOffsets[ 13 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 51 56 8B F1 85 F6 74 68" );
	g_aOffsets[ 14 ] = g_Utils.FindPattern( "client.dll", "8B 81 ? ? ? ? 85 C0 0F 84 ? ? ? ? C3" );
	g_aOffsets[ 15 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 57 8B F9 8B 8F ? ? ? ? 83 F9 FF" );
	g_aOffsets[ 16 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8" );
	g_aOffsets[ 17 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 83 E4 F8 83 EC 0C 53 8B 5D 08 8B C3" );
	g_aOffsets[ 18 ] = g_Utils.FindPattern( "client.dll", "53 8B ? 83 ? ? 83 ? ? 83 ? ? 55 8B ? ? 89 ? ? ? 8B ? 83 ? ? 56 57 8B ? ? 8D" );
	g_aOffsets[ 19 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 8B 8F" );
	g_aOffsets[ 20 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 83 E4 F8 83 EC 18 56 8B F1 57" );
	g_aOffsets[ 21 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 51 56 8B F1 8B 06 FF 90 ? ? ? ? 33 D2" );
	g_aOffsets[ 22 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 53 8B 5D 08 56 8B F1 83 FB FF" );
	g_aOffsets[ 23 ] = g_Utils.FindPattern( "client.dll", "56 8B F1 85 F6 74 31" );
	g_aOffsets[ 24 ] = g_Utils.FindPattern( "client.dll", "E8 ? ? ? ? 83 7D D8 00 7C 0F" );
	g_aOffsets[ 25 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 83 E4 F8 83 EC 24 53 56 57 8B F9 8D B7" );
	g_aOffsets[ 26 ] = g_Utils.FindPattern( "client.dll", "E8 ?? ?? ?? ?? 85 FF 74 11 8B 07 8B CF FF 50 08 8B 00 89 83 ?? ?? ?? ??" );
	g_aOffsets[ 27 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 83 EC 10 53 56 57 8B F9 8B 87 ? ? ? ? C1 E8 16" );
	g_aOffsets[ 28 ] = g_Utils.FindPattern( "client.dll", "8B C1 8B 50 40" );
	g_aOffsets[ 29 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 83 E4 F8 51 56 8B F1 8B 06" );
	g_aOffsets[ 30 ] = g_Utils.FindPattern( "client.dll", "56 8B 35 ? ? ? ? 57 8B F9 8B CE 8B 06 FF 90 ? ? ? ? 8B 07" );
	g_aOffsets[ 31 ] = g_Utils.FindPattern( "client.dll", "8B 86 ? ? ? ? 83 F8 FF 74 29 85 C0" ) + 0x2;
	g_aOffsets[ 32 ] = g_Utils.FindPattern( "client.dll", "8B 86 ? ? ? ? 09 47 30" ) + 0x2;
	g_aOffsets[ 33 ] = g_Utils.FindPattern( "client.dll", "55 8B EC FF 75 18 F3 0F 10 45 ? 51" );
	g_aOffsets[ 34 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 83 E4 F8 83 EC 24 53 56 8B 75 08 8B D9 57" );
	g_aOffsets[ 35 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 8B 45 08 56 8B F1 8B 0D" );
	g_aOffsets[ 36 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 56 FF 75 08 8B F1 8B 06 FF 90 ? ? ? ? 8B 86" );
	g_aOffsets[ 37 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 83 7D 08 FF" );
	g_aOffsets[ 38 ] = g_Utils.FindPattern( "client.dll", "85 C9 74 61 85 D2 78 22 8B 41 04 85 C0 75 0A 8B 01 8B 80 ? ? ? ? EB 03 8B 40 14 3B D0 7D 0A 52 E8 ? ? ? ? 03 40 04" );
	g_aOffsets[ 39 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 56 8B F1 83 BE ? ? ? ? ? 75 14 8B 46 04 8D 4E 04 FF 50 20 85 C0 74 07 8B CE E8 ? ? ? ? 8B B6 ? ? ? ? 85 F6 74 48" );
	g_aOffsets[ 40 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 53 56 8B F1 57 83 BE ? ? ? ? ? 75 14" );
	g_aOffsets[ 41 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 56 8B F1 83 BE ? ? ? ? ? 75 14 8B 46 04 8D 4E 04 FF 50 20 85 C0 74 07 8B CE E8 ? ? ? ? 8B 8E ? ? ? ? 85 C9 74 49 83 39 00 74 44 8B 41 04" );
	g_aOffsets[ 42 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 56 8B F1 8B 86 ? ? ? ? 8D 8E" );
	g_aOffsets[ 43 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 53 8B 5D 08 56 8B F1 39 9E ? ? ? ? 74 7F" );
	g_aOffsets[ 44 ] = g_Utils.FindPattern( "client.dll", "56 8B F1 83 BE ? ? ? ? ? 75 0A 8B 06" );
	g_aOffsets[ 45 ] = g_Utils.FindPattern( "client.dll", "F3 0F 10 8E ? ? ? ? 0F 57 C0 8B 86 ? ? ? ? 0F 2E C8" ) + 0x4;
	g_aOffsets[ 46 ] = g_Utils.FindPattern( "client.dll", "C6 86 ? ? ? ? ? FF 50 04" ) + 0x2;
	g_aOffsets[ 47 ] = g_Utils.FindPattern( "client.dll", "83 BE ? ? ? ? ? 7F 67" ) + 0x2;
	g_aOffsets[ 48 ] = g_Utils.FindPattern( "client.dll", "A3 ? ? ? ? 57 8B CB" ) + 0x1;
	g_aOffsets[ 49 ] = g_Utils.FindPattern( "client.dll", "80 3D ? ? ? ? ? 53 56 57 0F 85" ) + 0x2;
	g_aOffsets[ 50 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 83 E4 F8 51 53 8A 5D 08" );
	g_aOffsets[ 51 ] = g_Utils.FindPattern( "client.dll", "89 9F ? ? ? ? 89 87 ? ? ? ? E8 ? ? ? ? 89 45 08" );
	g_aOffsets[ 52 ] = g_Utils.FindPattern( "client.dll", "81 C7 ? ? ? ? 8B 4F 0C 8B 57 04 89 4C 24 24 8D 41 01" );
	g_aOffsets[ 53 ] = g_Utils.FindPattern( "engine.dll", "55 8B EC 83 E4 F0 83 EC 28" );
	g_aOffsets[ 54 ] = g_Utils.FindPattern( "client.dll", "80 BE ? ? ? ? ? F3 0F 10 53 ? F3 0F 59 D8" ) + 0x2;
	g_aOffsets[ 55 ] = g_Utils.FindPattern( "engine.dll", "C7 45 ? ? ? ? ? B9 ? ? ? ? C7 45 ? ? ? ? ? 8D 85" ) + 0x3;
	g_aOffsets[ 56 ] = g_Utils.FindPattern( "engine.dll", "C7 45 ? ? ? ? ? B9 ? ? ? ? C7 45 ? ? ? ? ? 8D 85" ) + 15;
	g_aOffsets[ 57 ] = g_Utils.FindPattern( "engine.dll", "B9 ? ? ? ? C7 45 ? ? ? ? ? 8D 85" ) + 0x1;
	g_aOffsets[ 58 ] = g_Utils.FindPattern( "engine.dll", "E8 ? ? ? ? A1 ? ? ? ? B9 ? ? ? ? FF 50 14 6A 00 6A 00 8D 55 A8" );
	g_aOffsets[ 59 ] = g_Utils.FindPattern( "engine.dll", "FF 90 ? ? ? ? 8D 4D A8 E8 ? ? ? ? 5F 5E 5B 8B E5 5D C3" );
	g_aOffsets[ 60 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 83 E4 F8 83 EC 34 53 56 8B F1 57 8B 46 60" );
	g_aOffsets[ 61 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 81 EC ? ? ? ? 53 56 57 8B 3D" );
	g_aOffsets[ 62 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 51 53 56 57 8B F9 8B 77 60" );
	g_aOffsets[ 63 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 83 E4 F8 81 EC ? ? ? ? 56 57 8B 3D ? ? ? ? 8B F1" );
	g_aOffsets[ 64 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 51 53 56 57 8B F9 83 7F 60 00" );
	g_aOffsets[ 65 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 56 57 8B F9 8B 17 83 BA ? ? ? ? ? 74 34 83 7F 04 00 75 2E 83 BA ? ? ? ? ? 75 04 33 C0 EB 0C 8B 0D ? ? ? ? 52 8B 01 FF 50 68" );
	g_aOffsets[ 66 ] = g_Utils.FindPattern( "server.dll", "55 8B EC 83 EC 2C 53 56 8B 75 08 8B D9" );
	g_aOffsets[ 67 ] = g_Utils.FindPattern( "server.dll", "55 8B EC 8B 55 08 83 EC 30" );
	g_aOffsets[ 68 ] = g_Utils.FindPattern( "client.dll", "53 56 57 8B F9 33 F6 8B 4F 60" );
	g_aOffsets[ 69 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 51 56 8B 71 60 83 BE ? ? ? ? ? 0F 84 ? ? ? ? 8B B6 ? ? ? ? 81 C6 ? ? ? ? 0F 84 ? ? ? ? F3 0F 10 56 ? 0F 28 C2 E8 ? ? ? ? 0F 57 DB 0F 2F D8 73 6C" );
	g_aOffsets[ 70 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 51 56 8B 71 60 83 BE ? ? ? ? ? 0F 84 ? ? ? ? 8B B6 ? ? ? ? 81 C6 ? ? ? ? 0F 84 ? ? ? ? F3 0F 10 56 ? 0F 28 C2 E8 ? ? ? ? 0F 57 DB 0F 2F D8 73 75" );
	g_aOffsets[ 71 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 83 E4 F8 A1 ? ? ? ? 83 EC 20 F3 0F 10 48" );
	g_aOffsets[ 72 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 83 EC 08 56 57 8B F9 8B 77 60" );
	g_aOffsets[ 73 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 83 EC 08 8B 45 08 53 56 57 8D 1C C5" );
	g_aOffsets[ 74 ] = g_Utils.FindPattern( "engine.dll", "B9 ? ? ? ? E8 ? ? ? ? A1 ? ? ? ? 85 C0 74 06 50 E8 ? ? ? ? 8B 0D ? ? ? ? 83 C1 08" ) + 0x1;
	g_aOffsets[ 75 ] = g_Utils.FindPattern( "engine.dll", "A1 ? ? ? ? B9 ? ? ? ? FF 50 30 5F" ) + 0x1;
	g_aOffsets[ 76 ] = g_Utils.FindPattern( "engine.dll", "A3 ? ? ? ? 8D 46 0F 83 E0 F0" ) + 0x1;
	g_aOffsets[ 77 ] = g_Utils.FindPattern( "client.dll", "E8 ? ? ? ? 8B F0 EB 02 33 F6 A1 ? ? ? ? 85 C0 74 05 83 C0 04 EB 02 33 C0 51" );
	g_aOffsets[ 78 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89 4C 24 04" );
	g_aOffsets[ 79 ] = g_Utils.FindPattern( "engine.dll", "B8 ? ? ? ? 3B F0 0F 4F F0 89 5D" );
	g_aOffsets[ 80 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0" );
	g_aOffsets[ 81 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 83 E4 F8 8B 4D 08 BA ? ? ? ? E8 ? ? ? ? 85 C0 75 12" );
	g_aOffsets[ 82 ] = g_Utils.FindPattern( "tier0.dll", "A1 ? ? ? ? 83 F8 FF 74 08 56 50 FF 15 ? ? ? ? C6 86" );
	g_aOffsets[ 83 ] = g_Utils.FindPattern( "client.dll", "A1 ? ? ? ? 85 C0 75 53" );
	g_aOffsets[ 84 ] = g_Utils.FindPattern( "client.dll", "E8 ? ? ? ? 8B 8E ? ? ? ? 8B F8 85 C9" );
	g_aOffsets[ 85 ] = g_Utils.FindPattern( "engine.dll", "55 8B EC 83 E4 F8 53 57" );
	g_aOffsets[ 86 ] = g_Utils.FindPattern( "engine.dll", "55 8B EC 56 57 8B F9 8D 4F 04 C7 ?? ?? ?? ?? ?? E8 ?? ?? ?? ?? 8B ?? ?? C6 ?? ?? ?? C7" );
	g_aOffsets[ 87 ] = g_Utils.FindPattern( "engine.dll", "53 56 57 8B F9 8D ?? 38 C7 ?? ?? ?? ?? ?? C7 ?? ?? ?? ?? ?? ?? 8B" );
	g_aOffsets[ 88 ] = g_Utils.FindPattern( "engine.dll", "55 8B EC 83 EC 68 F3 0F 10 4D" );
	g_aOffsets[ 89 ] = g_Utils.FindPattern( "vguimatsurface.dll", "55 8B EC A1 ? ? ? ? B9 ? ? ? ? 6A 02 6A 00" );
	g_aOffsets[ 90 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 83 EC 38 56 8B F1 57" );
	g_aOffsets[ 91 ] = g_Utils.FindPattern( "client.dll", "8B 3D ? ? ? ? 89 75 F8 85 FF 74 26 8D 4D F8" );
	g_aOffsets[ 92 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 83 E4 F8 83 EC 1C 53 56 57 8B F1 FF 15" );
	g_aOffsets[ 93 ] = g_Utils.FindPattern( "client.dll", "0F 85 ? ? ? ? A1 ? ? ? ? 85 C0 75 29 A1 ? ? ? ? 68" );
	g_aOffsets[ 94 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 83 E4 C0 83 EC 34 53 56 8B F1 8B 4D 08" );
	g_aOffsets[ 95 ] = g_Utils.FindPattern( "client.dll", "FF 35 ? ? ? ? E8 ? ? ? ? 8B F8 85 FF 0F 84" );
	g_aOffsets[ 96 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 83 E4 F8 8B 0D ? ? ? ? 85 C9" );
	g_aOffsets[ 97 ] = g_Utils.FindPattern( "client.dll", "A3 ? ? ? ? EB 0A C7 05 ? ? ? ? ? ? ? ? 8B 0D ? ? ? ? 8B 01" );
	g_aOffsets[ 98 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 8B D9 C7 44 24 ? ? ? ? ? 8B 0D" );
	g_aOffsets[ 99 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 83 E4 F8 83 EC 20 83 3D" );
	g_aOffsets[ 100 ] = g_Utils.FindPattern( "client.dll", "A1 ? ? ? ? 56 8B F1 A8 01 75 3D" );
	g_aOffsets[ 101 ] = g_Utils.FindPattern( "client.dll", "A1 ? ? ? ? D1 E8 A8 01 74 4D" );
	g_aOffsets[ 102 ] = g_Utils.FindPattern( "client.dll", "B9 ? ? ? ? E8 ? ? ? ? 85 C0 74 0C 8D 48 EC 85 C9 74 05 E8 ? ? ? ? 80 3D" );
	g_aOffsets[ 103 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 8B 45 0C 85 C0 8B 0D ? ? ? ? 53 8B 1D" );
	g_aOffsets[ 104 ] = g_Utils.FindPattern( "client.dll", "80 3D ? ? ? ? ? 74 1D A1" );
	g_aOffsets[ 105 ] = g_Utils.FindPattern( "client.dll", "8B 0D ? ? ? ? 6A 00 6A FF 56 8B 11 6A 00" );
	g_aOffsets[ 106 ] = g_Utils.FindPattern( "client.dll", "A1 ? ? ? ? D1 E8 A8 01 74 4D 8B 0D" );
	g_aOffsets[ 107 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 83 E4 F8 51 53 56 8B D9 8B 0D" );
	g_aOffsets[ 108 ] = g_Utils.FindPattern( "client.dll", "8B 0D ? ? ? ? 85 C9 0F 84 ? ? ? ? 8B 01 8B 80" );
	g_aOffsets[ 109 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 83 E4 F8 8B 0D ? ? ? ? 81 EC ? ? ? ? 8B 01 53 56 57 FF 50 34" );
	g_aOffsets[ 110 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 83 EC 4C 8B 45 08 F3 0F 10 5A" );
	g_aOffsets[ 111 ] = g_Utils.FindPattern( "engine.dll", "55 8B EC 8B 4D 08 56 E8 ? ? ? ? 8B F0 85 F6 78 08 3B 35 ? ? ? ? 7C 0E 68 ? ? ? ? FF 15 ? ? ? ? 83 C4 04 A1 ? ? ? ? 03 F6 0F BF 44 F0" );
	g_aOffsets[ 112 ] = g_Utils.FindPattern( "engine.dll", "55 8B EC 56 8B F1 57 8B FA 85 F6 74 62" );
	g_aOffsets[ 113 ] = g_Utils.FindPattern( "client.dll", "53 8B D9 83 C8" );
	g_aOffsets[ 114 ] = g_Utils.FindPattern( "engine.dll", "50 E8 ? ? ? ? 83 C4 10 8B E5 5D C2 08 00" );
	g_aOffsets[ 115 ] = g_Utils.FindPattern( "engine.dll", "8B 0D ? ? ? ? 85 C9 79 30" ) + 0x2;
	g_aOffsets[ 116 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 83 E4 F0 83 EC 7C 56 52" );
	g_aOffsets[ 117 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 83 E4 F0 83 EC 6C 56 52" );
	g_aOffsets[ 118 ] = g_Utils.FindPattern( "engine.dll", "55 8B EC 81 EC ? ? ? ? 56 57 8B F9 C7 45" );
	g_aOffsets[ 119 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 83 E4 F0 81 EC ? ? ? ? 8B C2" );
	g_aOffsets[ 120 ] = g_Utils.FindPattern( "server.dll", "A1 ? ? ? ? F3 0F 58 40 ? F3 0F 5F 87" ) + 0x1;
	g_aOffsets[ 121 ] = g_Utils.FindPattern( "server.dll", "55 8B EC 81 EC ? ? ? ? 53 56 8B 35 ? ? ? ? 8B D9 57 8B CE" );
	g_aOffsets[ 122 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 83 EC 1C 53 8B D9 85 DB" );
	g_aOffsets[ 123 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 6A 00 FF 75 08 E8 ? ? ? ? 85 C0 74 45" );
	g_aOffsets[ 124 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 83 E4 C0 81 EC ? ? ? ? 53 8B 5D 08" );
	g_aOffsets[ 125 ] = g_Utils.FindPattern( "engine.dll", "A1 ? ? ? ? A8 01 75 1C 83 C8 01 C7 05 ? ? ? ? ? ? ? ? A3 ? ? ? ? C7 05 ? ? ? ? ? ? ? ? 53" );
	g_aOffsets[ 126 ] = g_Utils.FindPattern( "engine.dll", "8B 0D ? ? ? ? 80 7D 14 00 8B 06 0F 45 0D ? ? ? ? 6A 00 51" );
	g_aOffsets[ 127 ] = g_Utils.FindPattern( "engine.dll", "55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 56 57 E8 ? ? ? ? 8B 0D ? ? ? ? 8B 01 FF 90 ? ? ? ? 8B F0 85 F6" );
	g_aOffsets[ 128 ] = g_Utils.FindPattern( "engine.dll", "55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 56 57 E8 ? ? ? ? 8B 0D ? ? ? ? 8B 01 FF 90 ? ? ? ? 8B F0 89 74 24 2C" );
	g_aOffsets[ 129 ] = g_Utils.FindPattern( "engine.dll", "55 8B EC 81 EC ? ? ? ? 53 56 57 89 55 EC" );
	g_aOffsets[ 130 ] = g_Utils.FindPattern( "engine.dll", "55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 56 57 E8 ? ? ? ? 8B 0D ? ? ? ? 8B 01 FF 90 ? ? ? ? 8B F0 89 74 24 20 85 F6 74 07 8B 06 8B CE FF 50 08 8B 06 8B CE FF 90 ? ? ? ? 8B F8" );
	g_aOffsets[ 131 ] = g_Utils.FindPattern( "engine.dll", "55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 56 57 E8 ? ? ? ? 8B 0D ? ? ? ? 8B 01 FF 90 ? ? ? ? 8B F0 89 74 24 20 85 F6 74 07 8B 06 8B CE FF 50 08 8B 06 8B CE FF 90 ? ? ? ? 8B D8" );
	g_aOffsets[ 132 ] = g_Utils.FindPattern( "engine.dll", "55 8B EC 83 E4 F0 81 EC ? ? ? ? 56 57 E8" );
	g_aOffsets[ 133 ] = g_Utils.FindPattern( "engine.dll", "53 8B DC 83 EC 08 83 E4 F8 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 81 EC ? ? ? ? 56 57 E8" );
	g_aOffsets[ 134 ] = g_Utils.FindPattern( "client.dll", "55 8B EC A1 ? ? ? ? 83 EC 08 D1 E8" );
	g_aOffsets[ 135 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 56 8B F1 57 85 F6" );
	g_aOffsets[ 136 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 83 E4 F8 83 EC 70 6A" );
	g_aOffsets[ 137 ] = g_Utils.FindPattern( "client.dll", "C7 05 ? ? ? ? ? ? ? ? A8 02 75 2D 0F 57 C0" ) + 0x6;
	g_aOffsets[ 138 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 8B 4D 04 56 57 E8 ? ? ? ? 8B 35 ? ? ? ? 85 F6 74 57" );
	g_aOffsets[ 139 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 8B 4D 04 56 E8 ? ? ? ? 8B 75 08" );
	g_aOffsets[ 140 ] = g_Utils.FindPattern( "client.dll", "F6 05 ? ? ? ? ? 74 5B" );
	g_aOffsets[ 141 ] = g_Utils.FindPattern( "client.dll", "B9 ? ? ? ? E8 ? ? ? ? B9 ? ? ? ? C7 05 ? ? ? ? ? ? ? ? C7 05 ? ? ? ? ? ? ? ? E8 ? ? ? ? 68" ) + 0x1;
	g_aOffsets[ 142 ] = g_Utils.FindPattern( "engine.dll", "A1 ? ? ? ? 33 F6 39 B0 ? ? ? ? 0F 8E ? ? ? ? 33 C9 89 4C 24 08" ) + 0x1;
	g_aOffsets[ 143 ] = g_Utils.FindPattern( "client.dll", "A1 ? ? ? ? A8 01 75 12 83 C8 01 B9 ? ? ? ? A3 ? ? ? ? E8 ? ? ? ? B8" );
	g_aOffsets[ 144 ] = g_Utils.FindPattern( "client.dll", "8B 0D ? ? ? ? 85 C9 75 03 33 C0 C3 8B 89" );
	g_aOffsets[ 145 ] = g_Utils.FindPattern( "engine.dll", "53 56 57 8B DA 8B F9 FF 15" );
	g_aOffsets[ 146 ] = g_Utils.FindPattern( "steamclient.dll", "FF 35 ? ? ? ? FF 50 14 A1 ? ? ? ? B9" ) + 0x2;
	g_aOffsets[ 147 ] = g_Utils.FindPattern( "engine.dll", "C7 05 ? ? ? ? ? ? ? ? A2 ? ? ? ? 8B 46 04" ) + 0x2;
	g_aOffsets[ 148 ] = g_Utils.FindPattern( "client.dll", "83 3D ? ? ? ? ? 74 09 83 3D ? ? ? ? ? 75 2D" ) + 0x2;
	g_aOffsets[ 149 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 83 E4 F8 83 EC 14 53 56 57 8B DA" );
	g_aOffsets[ 150 ] = g_Utils.FindPattern( "client.dll", "55 8B EC A1 ? ? ? ? 53 56 57 8B F9 8B 08 8B 01" );
	g_aOffsets[ 151 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 51 A1 ? ? ? ? 85 C0 75 37" );
	g_aOffsets[ 152 ] = g_Utils.FindPattern( "client.dll", "A2 ? ? ? ? 8B 45 E8" ) + 0x1;
	g_aOffsets[ 153 ] = g_Utils.FindPattern( "engine.dll", "B9 ? ? ? ? 50 8D 85 ? ? ? ? 50 E8 ? ? ? ? EB 4E" ) + 0x1;
	g_aOffsets[ 154 ] = g_Utils.FindPattern( "server.dll", "55 8B EC 51 F3 0F 10 1D ? ? ? ? 56 57 6A 01 6A 00 8B F9 8B F2 51 8B 0D ? ? ? ? 57" );
	g_aOffsets[ 155 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 83 EC 18 53 56 8B 35 ? ? ? ? 8B D9" );
	g_aOffsets[ 156 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 83 EC 08 53 8B 5D 10 56" );
	g_aOffsets[ 157 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 83 EC 08 53 56 8B 75 08 57 8B F9 6B DE 34" );
	g_aOffsets[ 158 ] = g_Utils.FindPattern( "client.dll", "55 8B EC B8 ? ? ? ? 83 38 00" );
	g_aOffsets[ 159 ] = g_Utils.FindPattern( "client.dll", "57 8B F9 85 FF 74 6E E8" );
	g_aOffsets[ 160 ] = g_Utils.FindPattern( "client.dll", "51 A1 ? ? ? ? 53 8B DA" );
	g_aOffsets[ 161 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 56 8B F1 8B 06 85 C0 74 11" );
	g_aOffsets[ 162 ] = g_Utils.FindPattern( "client.dll", "8B 80 ? ? 00 00 FF D0 D9 5F ? 8B 4F ? 0F 57 C9" ) + 0x2;
	g_aOffsets[ 163 ] = g_Utils.FindPattern( "server.dll", "E8 ? ? ? ? 0F 28 C8 F3 0F 58 4D ? EB 05" );
	g_aOffsets[ 164 ] = g_Utils.FindPattern( "server.dll", "53 8B D9 F6 C3" );
	g_aOffsets[ 165 ] = g_Utils.FindPattern( "server.dll", "55 8B EC 83 EC 08 8B 45 08 56 57 8B F9 8D" );
	g_aOffsets[ 166 ] = g_Utils.FindPattern( "server.dll", "55 8B EC 83 E4 F0 81 EC ? ? ? ? 33 D2" );
	g_aOffsets[ 167 ] = g_Utils.FindPattern( "server.dll", "55 8B EC 83 E4 F0 81 EC ? ? ? ? 8B 81" );
	g_aOffsets[ 168 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 57 8B F9 83 BF ? ? 00 00 00 75 07 33 C0 5F 5D C2 04 00" );
	g_aOffsets[ 169 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 56 57 8B F9 8B 07" );
	g_aOffsets[ 170 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 83 EC 10 8B 4D 08 81 C1" );
	g_aOffsets[ 171 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 83 EC 10 53 8B D9 89 55 F8" );
	g_aOffsets[ 172 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 83 E4 F0 B8 ? ? ? ? E8 ? ? ? ? A1" );
	g_aOffsets[ 173 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 83 EC 10 53 56 57 8B 7D 10 8B D9 F3 0F 11 5D" );
	g_aOffsets[ 174 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 83 E4 F8 81 EC ? ? ? ? 8B C1 89 54 24 04 89 44 24 2C 56 57 8B 00" );
	g_aOffsets[ 175 ] = g_Utils.FindPattern( "client.dll", "56 8B F1 83 BE ? ? ? ? ? 75 14 8B 46 04 8D 4E 04 FF 50 20 85 C0 74 07 8B CE E8 ? ? ? ? 8B 86 ? ? ? ? 85 C0 74 05 83 38 00 75 02 33 C0 5E" );
	g_aOffsets[ 176 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 56 8B F1 8B 4D 08 85 C9 74 71 8B 06" );
	g_aOffsets[ 177 ] = g_Utils.FindPattern( "client.dll", "89 86 ? ? ? ? E8 ? ? ? ? 80 BE" ) + 0x2;
	g_aOffsets[ 178 ] = g_Utils.FindPattern( "client.dll", "80 3D ? ? ? ? ? 74 16 A1 ? ? ? ? 48 C7 81" );
	g_aOffsets[ 179 ] = g_Utils.FindPattern( "client.dll", "8B 92 ? ? ? ? 8D 0C 80 53 8B 1D ? ? ? ? 0F 57 C0 56" ) + 0x2;
	g_aOffsets[ 180 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 83 E4 C0 83 EC 38 56 8B F1" );
	g_aOffsets[ 181 ] = g_Utils.FindPattern( "server.dll", "55 8B EC 56 8B 75 08 57 8B F9 56 8B 07 FF 90" );
	g_aOffsets[ 182 ] = g_Utils.FindPattern( "client.dll", "FF 90 ? ? ? ? 8B D8 B2 01" ) + 0x2;
	g_aOffsets[ 183 ] = g_Utils.FindPattern( "client.dll", "56 8B F1 80 BE ? ? ? ? ? 0F 84 ? ? ? ? 8B 06 FF 90 ? ? ? ? 85 C0 74 04" );
	g_aOffsets[ 184 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 83 E4 F0 81 EC ? ? ? ? 33 D2" );
	g_aOffsets[ 185 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 83 EC 0C 56 8B F1 57 8B FA 85 F6 75 14" );
	g_aOffsets[ 186 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 56 8B F1 83 BE ? ? ? ? ? 75 14 8B 46 04 8D 4E 04 FF 50 20 85 C0 74 07 8B CE E8 ? ? ? ? 8B 8E ? ? ? ? 85 C9 74 05 83 39 00 75 02 33 C9 8B 55 08 51 FF 75 0C" );
	g_aOffsets[ 187 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 83 79 04 00 75 25" );
	g_aOffsets[ 188 ] = g_Utils.FindPattern( "client.dll", "53 8B DC 83 EC 08 83 E4 F8 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 83 EC 30 56 57 8B F9 8B 0D ? ? ? ? 8B 01 8B 80 74 01 00 00" );
	g_aOffsets[ 189 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 83 E4 F8 83 EC 70 56 57 8B F9 89 7C 24 38" );
	g_aOffsets[ 190 ] = g_Utils.FindPattern( "client.dll", "F3 0F 11 81 ? ? ? ? 8B 40 10 89 81" );
	g_aOffsets[ 191 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 53 56 57 8B F9 8B 4D 0C 85 C9" );
	g_aOffsets[ 192 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 83 EC 10 53 8B 5D 08 57 8B F9" );
	g_aOffsets[ 193 ] = g_Utils.FindPattern( "client.dll", "55 8B EC 83 E4 F8 83 EC 0C 53 56 8B F1 8B 86" );
	g_aOffsets[ 194 ] = g_Utils.FindPattern( "engine.dll", "8D 8B ? ? ? ? E8 ? ? ? ? 5F 5E 5B 8B E5" ) + 0x2;
	g_aOffsets[ 195 ] = g_Utils.FindPattern( "client.dll", "53 8B DC 83 EC 08 83 E4 F0 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 83 EC 10 0F 10 11" );
	g_aOffsets[ 196 ] = g_Utils.FindPattern( "client.dll", "A1 ? ? ? ? A8 01 75 4B" ) + 0x1;
	g_aOffsets[ 197 ] = g_Utils.FindPattern( "client.dll", "A3 ? ? ? ? 66 0F 6E 86 ? ? ? ? A1 ? ? ? ? 0F 5B C0" ) + 0x1;
	g_aOffsets[ 198 ] = g_Utils.FindPattern( "client.dll", "89 35 ? ? ? ? F3 0F 10 48" ) + 0x2;
	g_aOffsets[ 199 ] = g_Utils.FindPattern( "client.dll", "E8 ? ? ? ? 6A 01 EB 02 6A 02 6A FF 51 8B CE E8 ? ? ? ? 8B CE" );
	g_aOffsets[ 200 ] = g_Utils.FindPattern( "engine.dll", "8B 0D ? ? ? ? 8D 95 ? ? ? ? 6A 00 C6" ) + 0x2;
	g_aOffsets[ 201 ] = g_Utils.FindPattern( "engine.dll", "84 C0 75 66 8B 0D ? ? ? ? 81 F9" );
}