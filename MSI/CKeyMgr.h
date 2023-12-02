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

class CObject;

class CKeyMgr
{
	SINGLE(CKeyMgr);

private:
	vector<tKeyInfo> m_vecKey;
	Vec2 m_vCurMousePos;

public:
	void init();
	void update();
	// �ش� ������ �ȿ��� ���콺�� Ŭ���ߴ����� �˷��ִ� �Լ�
	bool IsMouseInsideClinet(HWND _hWnd = CCore::GetInst()->GetMainHwnd());

	// ���콺�� �ش� object ���� �ִ��� Ȯ���ϴ� �Լ�.
	bool IsMouseInObj(CObject* _pObj);

public:
	// �ش� Ű�� ���� �������� üũ�Ͽ� �������ش�.
	KEY_STATE GetKeyState(KEY _eKey) { return m_vecKey[(int)_eKey].eState; }
	// ���콺�� ���� ��ġ
	Vec2 GetMousePos() { return m_vCurMousePos;	}
};

