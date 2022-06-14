#include <Windows.h>

namespace Hooks
{
	using CreateProcessW_t = BOOL ( __stdcall* ) ( LPCWSTR, LPWSTR, LPSECURITY_ATTRIBUTES, LPSECURITY_ATTRIBUTES,
		BOOL, DWORD, LPVOID, LPCWSTR, LPSTARTUPINFOW, LPPROCESS_INFORMATION );

	inline CreateProcessW_t o_CreateProcessW;

	BOOL __stdcall hkCreateProcessW( LPCWSTR lpApplicationName, LPWSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcAttr,
		LPSECURITY_ATTRIBUTES lpThreadAttr, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment,
		LPCWSTR lpCurrentDir, LPSTARTUPINFOW lpInfo, LPPROCESS_INFORMATION lpProc );
}