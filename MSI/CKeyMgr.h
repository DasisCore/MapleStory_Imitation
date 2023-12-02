#pragma once

enum class KEY_STATE
{
	NONE,
	TAP,
	HOLD,
	AWAY
};

enum class KEY
{
	LEFT,
	RIGHT,
	UP,
	DOWN,
	Q, W, E, R, T, Y, U, I, O, P,
	A, S, D, F, G, H, J, K, L,
	Z, X, C, V, B, N, M,
	ALT,
	LSHIFT,
	SPACE,
	ENTER,
	CTRL,
	ESC,

	LBTN,
	RBTN,
	F1,
	F2,

	LAST,
};

struct tKeyInfo
{
	KEY_STATE eState;
	bool bPrevPush;
};

class CKeyMgr
{
	SINGLE(CKeyMgr);

private:
	vector<tKeyInfo> m_vecKey;
	Vec2 m_vCurMousePos;

public:
	void init();
	void update();
	// 해당 윈도우 안에서 마우스를 클릭했는지를 알려주는 함수
	bool IsMouseInsideClinet(HWND _hWnd = CCore::GetInst()->GetMainHwnd());

public:
	// 해당 키가 무슨 상태인지 체크하여 리턴해준다.
	KEY_STATE GetKeyState(KEY _eKey) { return m_vecKey[(int)_eKey].eState; }
	// 마우스의 현재 위치
	Vec2 GetMousePos() { return m_vCurMousePos;	}
};

