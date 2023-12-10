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
	// 이전 프레임에서 등록해둔 Dead 오브젝트들을 삭제한다.
	// =============================================

	for (size_t i = 0; i < m_vecDead.size(); i++)
	{
		delete m_vecDead[i];
	}
	m_vecDead.clear();

	// =============================================
	// Event 처리
	// =============================================

	for (size_t i = 0; i < m_vecEvent.size(); i++)
	{
		Excute(m_vecEvent[i]);

		// 씬 체인지가 발생했다면 다른 이벤트들은 삭제시켜줘야 한다.
		if (m_vecEvent[i].eEvent == EVENT_TYPE::SCENE_CHANGE) break;
	}
	m_vecEvent.clear();
}

// 이벤트 처리
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
		// Object를 Dead 상태로 변경
		// 삭제 예정 오브젝트들을 모아둔다.
		CObject* pDeadObj = (CObject*)_eve.lParam;
		pDeadObj->SetDead();
		m_vecDead.push_back(pDeadObj);
	}
		break;

	case EVENT_TYPE::SCENE_CHANGE:
	{
		// lParam : Next Scene Type
		// Scene 변경
		CSceneMgr::GetInst()->ChangeScene((SCENE_TYPE)_eve.lParam);

		//// 포커스 UI 해제 (이전 Scene 의 UI를 가리키고 있기 때문)
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
