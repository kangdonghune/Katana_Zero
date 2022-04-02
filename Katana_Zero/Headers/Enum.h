#pragma once

namespace GAMEOBJECT
{
	enum TYPE
	{
		PLAYER,
		PLAYERATTACK,
		GANGSTER,
		BOSS,
		BULLET,
		LASER,
		GRANADE,
		BUTCHERKNIFE,
		SMOKE,
		EXPLOSIVE,
		EFFECT,
		UI,
		END
	};
}

namespace TERRAINTYPE
{
	enum TYPE{LAND, WALL, CELLING, PASSABLE, STAGECHANGE, T_NONE ,END};
}

namespace UNITTYPE
{
	enum TYPE {TYPENONE, PLAYER, GANGSTER, BOSS, EFFECT, END };
}

namespace ITEMTYPE
{
	enum TYPE {BUTCHERKNIFE, SMOKE, EXPLOSIVEVIAL, END};
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
		AIM,
		ENTERSTAIR,
		FALL,
		HURTFLY,
		HURTGROUND,
		LEAVESTAIR,
		RUN,
		TURN,
		WALK,
		WHIP,
		END
	};
}


namespace BOSSSTATE
{
	enum State
	{
	
		AIM,
		DASH,
		DASH_END,
		DASH_SLOWMOTION,
		BDEAD,
		HURT,
		IDLE,
		PREDASH,
		PREJUMP,
		PUTBACK,
		RECOVER,
		ROLL,
		SWEEP,
		TAKEOUT,
		TELEPORT_IN,
		TELEPORT_OUT,
		TELEPORT_IN_SWEEPL,
		TELEPORT_IN_SWEEPR,
		TELEPORT_OUT_SWEEP,
		TELEPORT_IN_GROUND,
		TELEPORT_OUT_GROUND,
		Jump,
		WALLJUMP,
		WALLJUMP_LAND,
		END
	};
}

namespace BOSSSKILL
{
	enum Skill
	{
		SettingLine,
		ShotLaser,
		ShotGranade,
		SpinShot,
		END
	};
}