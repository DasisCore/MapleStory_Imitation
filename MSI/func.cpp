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

	fwrite(&iLen, sizeof(size_t), 1, _pFile);		// ���� ���� ����
	fwrite(pStrName, sizeof(wchar_t), iLen, _pFile);	// ���ڿ� ����
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
	// �ش� ������ ������Ʈ�� ��
	UINT vecSize = _vecObj.size();
	fwrite(&vecSize, sizeof(UINT), 1, _pFile);

	for (int i = 0; i < vecSize; i++)
	{
		CObject* pObj = _vecObj[i];

		// �̸� ����
		SaveWString(pObj->GetName(), _pFile);

		// Position ����
		Vec2 vPos = pObj->GetPos();
		float fPosX = vPos.x;
		float fPosY = vPos.y;
		fwrite(&fPosX, sizeof(float), 1, _pFile);
		fwrite(&fPosY, sizeof(float), 1, _pFile);

		// Scale ����
		Vec2 vScale = pObj->GetScale();
		float fScaleX = vScale.x;
		float fScaleY = vScale.y;
		fwrite(&fScaleX, sizeof(float), 1, _pFile);
		fwrite(&fScaleY, sizeof(float), 1, _pFile);

		// �ݶ��̴� ����
		CCollider* pCol = pObj->GetComponent()->GetCollider();
		bool bCollider = pCol == nullptr ? false : true;
		fwrite(&bCollider, sizeof(bool), 1, _pFile);

		// �ݶ��̴��� �ִٸ� 
		if (pCol != nullptr)
		{
			// Offset Position ����
			Vec2 vColOffsetPos = pCol->GetOffsetPos();
			float fOffsetX = vColOffsetPos.x;
			float fOffsetY = vColOffsetPos.y;
			fwrite(&fOffsetX, sizeof(float), 1, _pFile);
			fwrite(&fOffsetY, sizeof(float), 1, _pFile);

			// Collider Scale ����
			Vec2 vColScale = pCol->GetScale();
			float fColScaleX = vColScale.x;
			float fColScaleY = vColScale.y;
			fwrite(&fColScaleX, sizeof(float), 1, _pFile);
			fwrite(&fColScaleY, sizeof(float), 1, _pFile);
		}

		// �ִϸ��̼� ����
		CAnimator* pAni = pObj->GetComponent()->GetAnimator();
		bool bAnimator = pAni == nullptr ? false : true;
		fwrite(&bAnimator, sizeof(bool), 1, _pFile);

		if (pAni != nullptr)
		{
			map<wstring, CAnimation*> mapAnim = pAni->GetMapAnim();
			// �ش� ������Ʈ�� ����ϴ� animation ���� ����
			UINT animSize = mapAnim.size();
			fwrite(&animSize, sizeof(UINT), 1, _pFile);

			for (auto iter = mapAnim.begin(); iter != mapAnim.end(); iter++)
			{
				// �ҷ��� �ִϸ��̼� ��θ� ����
				wstring animName = iter->first;
				// �ִϸ��̼� ���� ��� ����
				wstring animPath = L"Animation\\" + animName + L".anim";
				SaveWString(animPath, _pFile);
			}
		}

		// ��ü (RigidBody) ����
		CRigidBody* pRigid = pObj->GetComponent()->GetRigidbody();
		bool bRigidBody = pRigid == nullptr ? false : true;
		fwrite(&bRigidBody, sizeof(bool), 1, _pFile);

		// �߷� ���� ����
		CGravity* pGravity = pObj->GetComponent()->GetGravity();
		bool bGravity = pGravity == nullptr ? false : true;
		fwrite(&bGravity, sizeof(bool), 1, _pFile);
	}
}
