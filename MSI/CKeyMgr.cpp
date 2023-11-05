#include "global.h"
#include "CKeyMgr.h"
#include "CCore.h"

// KEY와 같은 순서로 만들어진 배열
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
	// 모든 키에 대한 정보를 초기화 시켜준다.
	for (int i = 0; i < (int)KEY::LAST; i++)
	{
		m_vecKey.push_back({ KEY_STATE::NONE, false });
	}
}

// 키 입력 확인 함수
void CKeyMgr::update()
{
	HWND hWnd = GetFocus();

	if (hWnd != nullptr)
	{
		for (int i = 0; i < (int)KEY::LAST; i++)
		{
			// 해당 키의 입력이 확인되면
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

		// Mouse 위치 계산	// 해당 윈도우가 포커싱 중일 때, 한번은 창 밖이 클릭이 가능함. 버그 고쳐야 함.
		POINT ptPos = {};
		GetCursorPos(&ptPos);

		ScreenToClient(CCore::GetInst()->GetMainHwnd(), &ptPos);

		m_vCurMousePos = Vec2(ptPos);
	}
	// 윈도우 포커싱 해제 상태
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
