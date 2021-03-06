#pragma once
#include "main.h"

namespace util
{
#define INRANGE(x,a,b)    (x >= a && x <= b)
#define getBits( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define getByte( x )    (getBits(x[0]) << 4 | getBits(x[1]))
	inline DWORD PatternSearch( const char* modulename, std::string pattern )
	{
		auto module = GetModuleHandleA( modulename );
		const char* pat = pattern.c_str( );
		DWORD firstMatch = 0;
		DWORD rangeStart = ( DWORD )module;
		MODULEINFO miModInfo;
		GetModuleInformation( GetCurrentProcess( ), ( HMODULE )rangeStart, &miModInfo, sizeof( MODULEINFO ) );
		DWORD rangeEnd = rangeStart + miModInfo.SizeOfImage;
		for( DWORD pCur = rangeStart; pCur < rangeEnd; pCur++ )
		{
			if( !*pat )
				return firstMatch;

			if( *( PBYTE )pat == '\?' || *( BYTE* )pCur == getByte( pat ) )
			{
				if( !firstMatch )
					firstMatch = pCur;

				if( !pat[ 2 ] )
					return firstMatch;

				if( *( PWORD )pat == '\?\?' || *( PBYTE )pat != '\?' )
					pat += 3;

				else
					pat += 2; //one ?
			}
			else
			{
				pat = pattern.c_str( );
				firstMatch = 0;
			}
		}
		return NULL;
	}

	inline void* GetInterface( const char* dll, const char* name )
	{
		const auto factory = ( CreateInterfaceFn )( GetProcAddress( GetModuleHandleA( dll ), "CreateInterface" ) );
		return factory( name, 0 );
	}
}