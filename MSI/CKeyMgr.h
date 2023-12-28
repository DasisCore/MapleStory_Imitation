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

	BACKSPACE,
	DEL,

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

class CObject;

class CKeyMgr
{
	SINGLE(CKeyMgr);

private:
	vector<tKeyInfo> m_vecKey;
	Vec2 m_vCurMousePos;

	Vec2 m_vDragStart;
	bool m_bIgnore;

private:
	// 현재 마우스 위치를 계산하는 함수
	void MousePos();

	// 해당 오브젝트를 클릭했을 때, 타겟중인 오브젝트로 변경하고 + 드래그 기능
	void CheckObjectTarget();

public:
	void init();
	void update();
	// 해당 윈도우 안에서 마우스를 클릭했는지를 알려주는 함수
	bool IsMouseInsideClinet(HWND _hWnd = CCore::GetInst()->GetMainHwnd());

	// 마우스가 해당 object 위에 있는지 확인하는 함수.
	bool IsMouseInObj(CObject* _pObj);

public:
	// 해당 키가 무슨 상태인지 체크하여 리턴해준다.
	KEY_STATE GetKeyState(KEY _eKey) { return m_vecKey[(int)_eKey].eState; }
	// 마우스의 현재 위치
	Vec2 GetMousePos() { return m_vCurMousePos;	}
	Vec2 GetDragStart() { return m_vDragStart; }
};

