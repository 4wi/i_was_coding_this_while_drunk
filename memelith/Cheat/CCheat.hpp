class CCheat
{

public:

	void Init( );

private:

	__forceinline void SetupInterfaces( );
	__forceinline void SetupClassIDs( );
	__forceinline void SetupWeapons( );
	__forceinline void SetupAddresses( );
	__forceinline void SetupHooks( );

	__forceinline void InitPointers( );

};