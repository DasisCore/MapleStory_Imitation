#pragma once

// Singleton ¸ÅÅ©·Î
#define SINGLE(type) public:\
						static type* GetInst()\
						{\
							static type mgr;\
							return &mgr;\
						}\
					private:\
						type();\
						~type();

#define fDT CTimeMgr::GetInst()->GetfDT()
#define DT CTimeMgr::GetInst()->GetDT()

#define KEY_CHECK(key, state) CKeyMgr::GetInst()->GetKeyState(key) == state
#define KEY_NONE(key) KEY_CHECK(key, KEY_STATE::NONE)
#define KEY_TAP(key) KEY_CHECK(key, KEY_STATE::TAP)
#define KEY_HOLD(key) KEY_CHECK(key, KEY_STATE::HOLD)
#define KEY_AWAY(key) KEY_CHECK(key, KEY_STATE::AWAY)

#define MOUSE_POS CKeyMgr::GetInst()->GetMousePos()

#define TILE_SIZE 64


enum class SCENE_TYPE
{
	START,
	TOOL,
	ANIMATION_WORKSHOP,


	END,
};


enum class GROUP_TYPE
{
	DEFAULT,
	TILE,
	GROUND,
	PLAYER,
	MONSTER,
	PROJ_PLAYER,
	PROJ_MONSTER,
	MISSILE,
	DETECT,

	UI = 31,
	END = 32,
};

enum class BRUSH_TYPE
{
	HOLLOW,
	BLACK,
	END,
};

enum class PEN_TYPE
{
	RED,
	GREEN,
	BLUE,
	END,
};

enum class EVENT_TYPE
{
	CREATE_OBJECT,
	DELETE_OBJECT,
	SCENE_CHANGE,
	CHANGE_AI_STATE,

	END,
};


enum class MON_STATE
{
	IDLE,
	PATROL,
	TRACE,
	ATT,
	RUN,
	DEAD,
};