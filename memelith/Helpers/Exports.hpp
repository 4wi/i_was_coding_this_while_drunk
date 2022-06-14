#include <iostream>

extern "C"
{
	__declspec( dllexport ) uint8_t* g_aInterfaces[ 15 ];
	__declspec( dllexport ) uint8_t* g_aAddresses[ 5 ];
	__declspec( dllexport ) uint8_t* g_aOffsets[ 210 ];
}