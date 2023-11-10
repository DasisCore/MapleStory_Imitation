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

void ChangeAIState(AI* _pAI, MON_STATE _eNextState)
{
	tEvent eve = {};
	eve.eEvent = EVENT_TYPE::CHANGE_AI_STATE;
	eve.lParam = (DWORD_PTR)_pAI;
	eve.wParam = (DWORD_PTR)_eNextState;

	CEventMgr::GetInst()->AddEvent(eve);
}

void SaveWString(const wstring& _str, FILE* _pFile)
{
	const wchar_t* pStrName = _str.c_str();
	size_t iLen = _str.length();

	fwrite(&iLen, sizeof(size_t), 1, _pFile);		// 문자 길이 저장
	fwrite(pStrName, sizeof(wchar_t), iLen, _pFile);	// 문자열 저장
}

void LoadWString(wstring& _str, FILE* _pFile)
{
	size_t iLen = 0;
	fread(&iLen, sizeof(size_t), 1, _pFile);

	wchar_t szBuff[256] = {};
	fread(szBuff, sizeof(wchar_t), iLen, _pFile);

	_str = szBuff;
}
