#include "main.h"

CVMTHookManager* clientmodehook;

typedef bool( __thiscall* CreateMoveFn )( void*, float, CUserCmd* );
CreateMoveFn oCreateMove;

bool __fastcall CreateMove( void* ecx, void*, float frametime, CUserCmd* cmd )
{
	auto ret = oCreateMove( ecx, frametime, cmd );

	me = entitylist->GetBaseEntity( engine->GetLocalPlayer( ) );
	if( !me ) return ret;

	// code from gir489's black mesa darkstorm
	// required to fix the long jump module
	static bool jumpreleased = true;
	if( chk( cmd->buttons, IN_JUMP ) )
	{
		if( jumpreleased )
			jumpreleased = false;
		else
		{
			if( !chk( me->GetFlags( ), FL_ONGROUND ) && me->GetWaterLevel( ) == 0 && me->GetMoveType( ) != MOVETYPE_LADDER )
				del( cmd->buttons, IN_JUMP );
		}
	}
	else if( !jumpreleased )
		jumpreleased = true;

	return ret;
}

void Hook( )
{
	clientmodehook = new CVMTHookManager( ( DWORD** )clientmode );
	oCreateMove = ( CreateMoveFn )clientmodehook->Hook( ( DWORD )CreateMove, 21 );
}
