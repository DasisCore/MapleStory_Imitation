#include "global.h"
#include "func.h"

#include "CEventMgr.h"
#include "CComponent.h"
#include "CAnimator.h"
#include "CAnimation.h"
#include "CCollider.h"
#include "CRigidBody.h"
#include "CGravity.h"
#include "CObject.h"

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

void ChangeSprite(CSprite* _eOld, CSprite* _eNew)
{
	tEvent eve = {};
	eve.eEvent = EVENT_TYPE::CHANGE_SPRITE;
	eve.lParam = (DWORD_PTR)_eOld;
	eve.wParam = (DWORD_PTR)_eNew;

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

wstring setprecision_float(float _f, int cnt)
{
	wstring temp = std::to_wstring(_f);

	wstring result = L"";
	for (int i = 0; i < temp.length(); i++)
	{
		if (temp[i] == L'.')
		{
			result += temp[i];
			for (int j = 0; j < cnt; j++) result += temp[i + j + 1];
			break;
		}
		result += temp[i];
	}

	return result;
}

void SaveObject(const vector<CObject*> _vecObj, FILE* _pFile)
{
	// 해당 유형의 오브젝트의 수
	UINT vecSize = _vecObj.size();
	fwrite(&vecSize, sizeof(UINT), 1, _pFile);

	for (int i = 0; i < vecSize; i++)
	{
		CObject* pObj = _vecObj[i];

		// 이름 저장
		SaveWString(pObj->GetName(), _pFile);

		// Position 저장
		Vec2 vPos = pObj->GetPos();
		float fPosX = vPos.x;
		float fPosY = vPos.y;
		fwrite(&fPosX, sizeof(float), 1, _pFile);
		fwrite(&fPosY, sizeof(float), 1, _pFile);

		// Scale 저장
		Vec2 vScale = pObj->GetScale();
		float fScaleX = vScale.x;
		float fScaleY = vScale.y;
		fwrite(&fScaleX, sizeof(float), 1, _pFile);
		fwrite(&fScaleY, sizeof(float), 1, _pFile);

		// 콜라이더 유무
		CCollider* pCol = pObj->GetComponent()->GetCollider();
		bool bCollider = pCol == nullptr ? false : true;
		fwrite(&bCollider, sizeof(bool), 1, _pFile);

		// 콜라이더가 있다면 
		if (pCol != nullptr)
		{
			// Offset Position 저장
			Vec2 vColOffsetPos = pCol->GetOffsetPos();
			float fOffsetX = vColOffsetPos.x;
			float fOffsetY = vColOffsetPos.y;
			fwrite(&fOffsetX, sizeof(float), 1, _pFile);
			fwrite(&fOffsetY, sizeof(float), 1, _pFile);

			// Collider Scale 저장
			Vec2 vColScale = pCol->GetScale();
			float fColScaleX = vColScale.x;
			float fColScaleY = vColScale.y;
			fwrite(&fColScaleX, sizeof(float), 1, _pFile);
			fwrite(&fColScaleY, sizeof(float), 1, _pFile);
		}

		// 애니메이션 유무
		CAnimator* pAni = pObj->GetComponent()->GetAnimator();
		bool bAnimator = pAni == nullptr ? false : true;
		fwrite(&bAnimator, sizeof(bool), 1, _pFile);

		if (pAni != nullptr)
		{
			map<wstring, CAnimation*> mapAnim = pAni->GetMapAnim();
			// 해당 오브젝트가 사용하는 animation 갯수 저장
			UINT animSize = mapAnim.size();
			fwrite(&animSize, sizeof(UINT), 1, _pFile);

			for (auto iter = mapAnim.begin(); iter != mapAnim.end(); iter++)
			{
				// 불러올 애니메이션 경로를 저장
				wstring animName = iter->first;
				// 애니메이션 파일 경로 저장
				wstring animPath = L"Animation\\" + animName + L".anim";
				SaveWString(animPath, _pFile);
			}
		}

		// 강체 (RigidBody) 유무
		CRigidBody* pRigid = pObj->GetComponent()->GetRigidbody();
		bool bRigidBody = pRigid == nullptr ? false : true;
		fwrite(&bRigidBody, sizeof(bool), 1, _pFile);

		// 중력 적용 유무
		CGravity* pGravity = pObj->GetComponent()->GetGravity();
		bool bGravity = pGravity == nullptr ? false : true;
		fwrite(&bGravity, sizeof(bool), 1, _pFile);
	}
}
