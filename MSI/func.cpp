#include "global.h"
#include "func.h"

#include "CEventMgr.h"

void CreateObject(CObject* _pObj, GROUP_TYPE _eGroup)
{
	tEvent eve = {};
	eve.eEvent = EVENT_TYPE::CREATE_OBJECT;
	eve.lParam = (DWORD_PTR) _pObj;
	eve.wParam = (DWORD_PTR)_eGroup;

	CEventMgr::GetInst()->AddEvent(eve);
}

void DeleteObject(CObject* _pObj)
{
	tEvent eve = {};
	eve.eEvent = EVENT_TYPE::DELETE_OBJECT;
	eve.lParam = (DWORD_PTR)_pObj;

	CEventMgr::GetInst()->AddEvent(eve);
}

void ChangeScene(SCENE_TYPE _eNext)
{
	tEvent eve = {};
	eve.eEvent = EVENT_TYPE::SCENE_CHANGE;
	eve.lParam = (DWORD_PTR)_eNext;

	CEventMgr::GetInst()->AddEvent(eve);
}
