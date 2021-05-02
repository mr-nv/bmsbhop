#include "main.h"

void* clientmode;
IVEngineClient* engine;
IClientEntityList* entitylist;
CBaseEntity* me;

void Start( )
{
	// wait for the game to initialize
	while( !GetModuleHandleA( "engine.dll" ) && !GetModuleHandleA( "client.dll" ) ) Sleep( 100 );

	clientmode = ( void* )util::PatternSearch( "client.dll", "8B 0D ? ? ? ? D9 45 0C 53 51" );
	clientmode = **( void*** )( ( DWORD )clientmode + 2 );

	engine = ( IVEngineClient* )util::GetInterface( "engine.dll", "VEngineClient015" );
	entitylist = ( IClientEntityList* )util::GetInterface( "client.dll", "VClientEntityList003" );

	Hook( );
}

BOOL WINAPI DllMain( HINSTANCE instance, DWORD reason, LPVOID reserved )
{
	if( reason == DLL_PROCESS_ATTACH )
	{
		DisableThreadLibraryCalls( instance );

		auto entry = CreateThread( 0, 0, ( LPTHREAD_START_ROUTINE )Start, reserved, 0, 0 );
		if( !entry ) return FALSE;

		CloseHandle( entry );
	}

	return TRUE;
}