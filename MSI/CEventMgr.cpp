#include "global.h"

#include "CEventMgr.h"

#include "CObject.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CUIMgr.h"
#include "CSprite.h"

#include "CState.h"
#include "AI.h"

CEventMgr::CEventMgr()
{

}

CEventMgr::~CEventMgr()
{

}

void CEventMgr::update()
{
	// =============================================
	// ���� �����ӿ��� ����ص� Dead ������Ʈ���� �����Ѵ�.
	// =============================================

	for (size_t i = 0; i < m_vecDead.size(); i++)
	{
		delete m_vecDead[i];
	}
	m_vecDead.clear();

	// =============================================
	// Event ó��
	// =============================================

	for (size_t i = 0; i < m_vecEvent.size(); i++)
	{
		Excute(m_vecEvent[i]);

		// �� ü������ �߻��ߴٸ� �ٸ� �̺�Ʈ���� ����������� �Ѵ�.
		if (m_vecEvent[i].eEvent == EVENT_TYPE::SCENE_CHANGE) break;
	}
	m_vecEvent.clear();
}

// �̺�Ʈ ó��
void CEventMgr::Excute(const tEvent& _eve)
{
	switch (_eve.eEvent)
	{
	case EVENT_TYPE::CREATE_OBJECT:
	{
		CObject* pNewObj = (CObject*)_eve.lParam;
		GROUP_TYPE eType = (GROUP_TYPE)_eve.wParam;

		CSceneMgr::GetInst()->GetCurScene()->AddObject(pNewObj, eType);
	}
		break;

	case EVENT_TYPE::DELETE_OBJECT:
	{
		// lParam : Object Address
		// Object�� Dead ���·� ����
		// ���� ���� ������Ʈ���� ��Ƶд�.
		CObject* pDeadObj = (CObject*)_eve.lParam;
		pDeadObj->SetDead();
		m_vecDead.push_back(pDeadObj);
	}
		break;

	case EVENT_TYPE::SCENE_CHANGE:
	{
		// lParam : Next Scene Type
		// Scene ����
		CSceneMgr::GetInst()->ChangeScene((SCENE_TYPE)_eve.lParam);

		//// ��Ŀ�� UI ���� (���� Scene �� UI�� ����Ű�� �ֱ� ����)
		CUIMgr::GetInst()->SetFocusedUI(nullptr);
	}
		break;
	case EVENT_TYPE::CHANGE_AI_STATE:
	{
		// lParam : AI
		// wParam : next type
		AI* pAI = (AI*)_eve.lParam;
		MON_STATE eState = (MON_STATE)_eve.wParam;
		pAI->ChangeState(eState);
	}
		break;
	case EVENT_TYPE::CHANGE_SPRITE:
	{
		CSprite* pOldSprite = (CSprite*)_eve.lParam;
		CSprite* pNewSprite = (CSprite*)_eve.wParam;
		DeleteObject(pOldSprite);
		CSceneMgr::GetInst()->GetCurScene()->AddObject(pNewSprite, GROUP_TYPE::SPRITE);
	}
	break;
	}
}
