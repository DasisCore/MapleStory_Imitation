#include "global.h"
#include "CKeyMgr.h"
#include "CCore.h"

// KEY�� ���� ������ ������� �迭
int g_arrVK[int(KEY::LAST)] =
{
	VK_LEFT,	//LEFT,
	VK_RIGHT,	//RIGHT,
	VK_UP,		//UP,
	VK_DOWN,	//DOWN,
	'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
	'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',
	'Z', 'X', 'C', 'V', 'B', 'N', 'M',
	VK_MENU,	//ALT,
	VK_LSHIFT,	//LSHIFT,
	VK_SPACE,	//SAPCE,
	VK_RETURN,	//ENTER,
	VK_CONTROL,	//CTRL,
	VK_ESCAPE,	//ESC,

	VK_LBUTTON,
	VK_RBUTTON,

	//LAST,
};


CKeyMgr::CKeyMgr()
{

}

CKeyMgr::~CKeyMgr()
{

}

void CKeyMgr::init()
{
	// ��� Ű�� ���� ������ �ʱ�ȭ �����ش�.
	for (int i = 0; i < (int)KEY::LAST; i++)
	{
		m_vecKey.push_back({ KEY_STATE::NONE, false });
	}
}

// Ű �Է� Ȯ�� �Լ�
void CKeyMgr::update()
{
	HWND hWnd = GetFocus();

	if (hWnd != nullptr)
	{
		for (int i = 0; i < (int)KEY::LAST; i++)
		{
			// �ش� Ű�� �Է��� Ȯ�εǸ�
			if (GetAsyncKeyState(g_arrVK[i]) & 0x8000)
			{
				if (m_vecKey[i].bPrevPush)
				{
					m_vecKey[i].eState = KEY_STATE::HOLD;
				}
				else
				{
					m_vecKey[i].eState = KEY_STATE::TAP;
				}
				m_vecKey[i].bPrevPush = true;
			}
			else
			{
				if (m_vecKey[i].bPrevPush)
				{
					m_vecKey[i].eState = KEY_STATE::AWAY;
				}
				else
				{
					m_vecKey[i].eState = KEY_STATE::NONE;
				}
				m_vecKey[i].bPrevPush = false;
			}
		}

		// Mouse ��ġ ���	// �ش� �����찡 ��Ŀ�� ���� ��, �ѹ��� â ���� Ŭ���� ������. ���� ���ľ� ��.
		POINT ptPos = {};
		GetCursorPos(&ptPos);

		ScreenToClient(CCore::GetInst()->GetMainHwnd(), &ptPos);

		m_vCurMousePos = Vec2(ptPos);
	}
	// ������ ��Ŀ�� ���� ����
	else
	{
		for (int i = 0; i < (int)KEY::LAST; i++)
		{
			m_vecKey[i].bPrevPush = false;
			if (m_vecKey[i].eState == KEY_STATE::TAP || m_vecKey[i].eState == KEY_STATE::HOLD)
			{
				m_vecKey[i].eState = KEY_STATE::AWAY;
			}
			else
			{
				m_vecKey[i].eState = KEY_STATE::NONE;
			}
		}

	}
}
