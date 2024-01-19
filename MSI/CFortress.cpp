#include "global.h"
#include "CFortress.h"
#include "CTimeMgr.h"

#include "CPlayer.h"
#include "CComponent.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CTimeMgr.h"
#include "CMonster.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CObject.h"
#include "CSkillUI.h"

#include "CSoundMgr.h"

CFortress::CFortress(CObject* _pObj)
	: m_pOwner(_pObj)
	, m_fCastBegin(0.54f)
	, m_fCastRepeat(3.f)
	, m_fCastEnd(0.48f)
{
	CreateComponent();
	CreateCollider();

	CCollider* pCol = GetComponent()->GetCollider();
	pCol->SetScale(Vec2(580.f, 300.f));
	pCol->SetOffsetPos(Vec2(0.f, -100.f));

	CreateAnimator();
	CAnimator* pAni = GetComponent()->GetAnimator();
	pAni->LoadAnimation(L"Animation\\FORTRESS_BEGIN.anim");		// 9 프레임
	pAni->LoadAnimation(L"Animation\\FORTRESS_REPEAT.anim");	// 16 프레임
	pAni->LoadAnimation(L"Animation\\FORTRESS_END.anim");		// 8 프레임
	pAni->Play(L"FORTRESS_BEGIN", true);


	// skill ui에 해당 스킬의 쿨타임 표시
	vector<CObject*> vecObj = CSceneMgr::GetInst()->GetCurScene()->GetGroupObject(GROUP_TYPE::UI);
	CObject* pObj = nullptr;
	for (int i = 0; i < vecObj.size(); i++)
	{
		if (vecObj[i]->GetName() == L"SKILLBAR")
		{
			pObj = vecObj[i];
			break;
		}
	}

	CSkillUI* pSkillUI = dynamic_cast<CSkillUI*>(pObj);
	if (pSkillUI != nullptr) pSkillUI->SetCoolTimeSkill2();

	CSoundMgr::GetInst()->CreateSound(L"Fortress", "Fortress.mp3", false);
	CSoundMgr::GetInst()->Play(L"Fortress", SOUND_TYPE::EFFECT1);
}

CFortress::~CFortress()
{
}

void CFortress::CalCastingTime()
{
	CAnimator* pAni = GetComponent()->GetAnimator();

	if (m_fCastBegin > 0)
	{
		m_fCastBegin -= fDT;
	}
	else if (m_fCastBegin < 0 && m_fCastRepeat > 0)
	{
		pAni->Play(L"FORTRESS_REPEAT", true);
		m_fCastRepeat -= fDT;
	}
	else if (m_fCastRepeat < 0 && m_fCastEnd > 0)
	{
		pAni->Play(L"FORTRESS_END", true);
		m_fCastEnd -= fDT;
	}
	else if (m_fCastEnd < 0) DeleteObject(this);
}

void CFortress::update()
{
	// 위치 갱신
	SetPos(m_pOwner->GetPos());
		

	CalCastingTime();
}

void CFortress::OnCollisionEnter(CCollider* _pOther)
{
}

void CFortress::OnCollision(CCollider* _pOther)
{
}
