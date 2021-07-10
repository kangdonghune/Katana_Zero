#pragma once

namespace GAMEOBJECT
{
	enum TYPE{PLAYER, EFFECT, GANGSTER, BULLET, END};
}

namespace TERRAINTYPE
{
	enum TYPE{LAND, WALL, CELLING, PASSABLE, END};
}

namespace UNITTYPE
{
	enum TYPE { PLAYER, GANGSTER, END };
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
		DOOROPEN,
		DOORBREAKFULL,
		FALL,
		HURTFLY_BEGIN,
		HURTFLY_LOOB,
		HURTGROUND,
		HURTRECOVER,
		JUMP,
		DANCE,
		FLIP,
		ROLL,
		WALLSLIDE,
		END
	};
}

namespace GANGSTERSTATE
{
	enum State
	{
		IDLE,
		FALL,
		END
	};
}

