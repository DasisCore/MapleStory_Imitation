#include "global.h"
#include "CKeyMgr.h"
#include "CCore.h"
#include "CObject.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CCamera.h"
#include "CScene_Ani_Workshop.h"

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

	VK_BACK,	// BACKSPACE
	VK_DELETE,	// DELETE Ű

	VK_LBUTTON,
	VK_RBUTTON,
	VK_F1,
	VK_F2,

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
	HWND MainhWnd = CCore::GetInst()->GetMainHwnd();

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
					m_vDragStart = CCamera::GetInst()->GetRealPos(m_vCurMousePos);
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

		// Mouse ��ġ ���
		MousePos();
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

	CheckObjectTarget();
}

bool CKeyMgr::IsMouseInsideClinet(HWND _hWnd)
{
	POINT ptPos = {};
	GetCursorPos(&ptPos);
	ScreenToClient(_hWnd, &ptPos);

	RECT clientRect;
	GetClientRect(_hWnd, &clientRect);

	return PtInRect(&clientRect, ptPos);
}

bool CKeyMgr::IsMouseInObj(CObject* _pObj)
{
	Vec2 vPos = CCamera::GetInst()->GetRenderPos(_pObj->GetPos());
	Vec2 vScale = _pObj->GetScale();

	Vec2 vLT = Vec2(vPos.x - (vScale.x / 2.f), vPos.y - (vScale.y / 2.f)); // ���� �� ������
	Vec2 vRB = Vec2(vPos.x + (vScale.x / 2.f), vPos.y + (vScale.y / 2.f)); // ������ �Ʒ� ������

	if (vLT.x <= m_vCurMousePos.x && m_vCurMousePos.x <= vRB.x)
	{
		if (vLT.y <= m_vCurMousePos.y && m_vCurMousePos.y <= vRB.y)
		{
			return true;
		}
	}

	return false;
}

void CKeyMgr::MousePos()
{
	POINT ptPos = {};
	GetCursorPos(&ptPos);
	ScreenToClient(CCore::GetInst()->GetMainHwnd(), &ptPos);

	m_vCurMousePos = Vec2(ptPos);
}

void CKeyMgr::CheckObjectTarget()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();


	CScene_Ani_Workshop* pAniScene = dynamic_cast<CScene_Ani_Workshop*>(pCurScene);
	if (pAniScene != nullptr && pAniScene->isMarquee()) return;

	// Ÿ���� �׻� �ϳ�
	//if (pCurScene->GetName() == L"Tool Scene" || pCurScene->GetName() == L"Animation Workshop Scene")
	//{
		for (int i = (int)GROUP_TYPE::END - 1; i >= (int)GROUP_TYPE::DEFAULT; i--)
		{
			vector<CObject*> vecObj = pCurScene->GetGroupObject((GROUP_TYPE)i);
			for (int j = 0; j < vecObj.size(); j++)
			{
				CObject* pObj = vecObj[j];
				if (IsMouseInObj(vecObj[j]) && KEY_TAP(KEY::LBTN) && !vecObj[j]->GetTarget())
				{
					vecObj[j]->SetTarget(true);
					m_vDragStart = CCamera::GetInst()->GetRealPos(MOUSE_POS);
					m_bIgnore = false;
				}
				else if (IsMouseInObj(vecObj[j]) && KEY_HOLD(KEY::LBTN) && vecObj[j]->GetTarget() == true)
				{
					vecObj[j]->SetTarget(true);

					Vec2 vCurPos = vecObj[j]->GetPos();
					Vec2 vDiff = CCamera::GetInst()->GetRealPos(MOUSE_POS) - m_vDragStart;

					vCurPos += vDiff;
					vecObj[j]->SetPos(vCurPos);
					m_vDragStart = CCamera::GetInst()->GetRealPos(MOUSE_POS);
				}
				else if (IsMouseInObj(vecObj[j]) && KEY_AWAY(KEY::LBTN) && vecObj[j]->GetTarget() == true)
				{
					vecObj[j]->SetTarget(false);
				}
				else vecObj[j]->SetTarget(false);
			}
		}
	//}
}
