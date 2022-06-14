#include <iostream>

class CCheat
{
	
public:

	void Init( );

private:

	__forceinline void SetupInterfaces( );
	__forceinline void SetupHooks( );

	__forceinline uint8_t* FindPattern( std::string sModule, std::string sPattern );

};