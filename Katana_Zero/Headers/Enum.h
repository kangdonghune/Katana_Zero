#pragma once

namespace GAMEOBJECT
{
	enum TYPE{PLAYER, END};
}

namespace TERRAINTYPE
{
	enum TYPE{LAND, WALL, CELLING, PASSABLE, END};
}

namespace UNITTYPE
{
	enum TYPE { PLAYER, END };
}

namespace PLAYERSTATE
{
	enum State
	{
		IDLE,
		IDLE_TO_WALK,
		IDLE_TO_RUN,
		RUN,
		RUN_TO_IDLE,
		WALK,
		ATTACK,
		PRECROUCH,
		CROUCH,
		POSTCROUCH,
		DoorOpen,
		DoorBreakFull,
		Fall,
		Hurtfly_begin,
		Hurtfly_loob,
		Hurtground,
		Hurtrecover,
		Jump,
		Dance,
		Flip,
		ROLL,
		WALLSLIDE,
		END
	};
}