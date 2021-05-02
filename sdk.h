#pragma once
#include <Windows.h>
#include <Psapi.h>
#include <string>

template< int idx, typename T = void, typename ... Args >
inline T CallVFunc( void* base, Args&& ... args )
{
	typedef T( __thiscall* Fn )( void*, decltype( args )... );
	return ( *( Fn** )( base ) )[ idx ]( base, args... );
}

#define OFFSET_m_fFlags 0x354
#define OFFSET_m_nWaterLevel 0x180
#define OFFSET_m_MoveType ( 0x178 + sizeof( int ) )

#define	add(dst, flag)	((dst) |= (flag))
#define	del(dst, flag)	((dst) &=~(flag))
#define	not(dst, flag)	((dst) & ~(flag))
#define	chk(dst, flag)	((dst) &  (flag))

#define MOVETYPE_LADDER 9
#define IN_JUMP ( 1 << 1 )
#define FL_ONGROUND ( 1 << 0 )

class IVEngineClient
{
public:
	int GetLocalPlayer( )
	{
		return CallVFunc< 12, int >( this );
	}
};

class CBaseEntity;

class IClientEntityList
{
public:
	virtual void* pad1( ) = 0;
	virtual void* pad2( ) = 0;
	virtual void* pad3( ) = 0;
	virtual CBaseEntity* GetBaseEntity( int index ) = 0;
};

class CBaseEntity
{
public:
	unsigned char GetMoveType( )
	{
		return *( unsigned char* )( this + OFFSET_m_MoveType );
	}

	int GetFlags( )
	{
		return *( int* )( this + OFFSET_m_fFlags );
	}

	int GetWaterLevel( )
	{
		return *( int* )( this + OFFSET_m_nWaterLevel );
	}
};

class CUserCmd
{
public:
	char pad[ 0x24 ];
	int	buttons;
	char pad2[ 0x14 ];
};

typedef void* ( __cdecl* CreateInterface_t )( const char*, int* );
typedef void* ( *CreateInterfaceFn )( const char* pName, int* pReturnCode );