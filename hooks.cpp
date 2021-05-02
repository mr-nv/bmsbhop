#include "main.h"

CVMTHookManager* clientmodehook;

typedef bool( __thiscall* CreateMoveFn )( void*, float, CUserCmd* );
CreateMoveFn oCreateMove;

bool __fastcall CreateMove( void* ecx, void*, float frametime, CUserCmd* cmd )
{
	auto ret = oCreateMove( ecx, frametime, cmd );

	me = entitylist->GetBaseEntity( engine->GetLocalPlayer( ) );
	if( !me ) return ret;

	if( chk( cmd->buttons, IN_JUMP ) && !chk( me->GetFlags( ), FL_ONGROUND ) )
	{
		if( me->GetWaterLevel( ) == 0 && me->GetMoveType( ) != MOVETYPE_LADDER )
			del( cmd->buttons, IN_JUMP );
	}

	return ret;
}

void Hook( )
{
	clientmodehook = new CVMTHookManager( ( DWORD** )clientmode );
	oCreateMove = ( CreateMoveFn )clientmodehook->Hook( ( DWORD )CreateMove, 21 );
}