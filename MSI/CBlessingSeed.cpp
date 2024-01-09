#include "global.h"
#include "CBlessingSeed.h"
#include "CTimeMgr.h"

#include "CComponent.h"
#include "CCollider.h"
#include "CAnimator.h"

#include "CSeed.h"
#include "CSceneMgr.h"
#include "CScene.h"

CBlessingSeed::CBlessingSeed(CObject* _pObj)
	: m_pOwner(_pObj)
	, m_fCastTime(1.14f)
{
	SetPos(m_pOwner->GetPos());

	CreateComponent();
	CreateCollider();

	CCollider* pCol = GetComponent()->GetCollider();
	pCol->SetScale(Vec2(600.f, 300.f));
	pCol->SetOffsetPos(Vec2(0.f, -100.f));

	CreateAnimator();
	CAnimator* pAni = GetComponent()->GetAnimator();
	pAni->LoadAnimation(L"Animation\\BLESSINGSEED.anim");		// 19 프레임
	pAni->Play(L"BLESSINGSEED", false);


	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	vector<CObject*> vecFoothold = pCurScene->GetGroupObject(GROUP_TYPE::FOOTHOLD);

	// Finalupdate가 돌기 전이라 수동으로 더해주어야 함.
	Vec2 vPos = GetPos() + GetComponent()->GetCollider()->GetOffsetPos();
	Vec2 vScale = GetComponent()->GetCollider()->GetScale();

	for (int i = 0; i < vecFoothold.size(); i++)
	{
		Vec2 vFootHoldPos = vecFoothold[i]->GetComponent()->GetCollider()->GetFinalPos();
		Vec2 vFootHoldScale = vecFoothold[i]->GetComponent()->GetCollider()->GetScale();

		// 100px 간격으로 해당 위치에 Seed가 있는지 확인
		float startX = max(vPos.x - (vScale.x / 2.f), vFootHoldPos.x - (vFootHoldScale.x / 2.f));
		float endX = min(vPos.x + (vScale.x / 2.f), vFootHoldPos.x + (vFootHoldScale.x / 2.f));
		float Y = vFootHoldPos.y - (vFootHoldScale.y / 2.f);

		if (Y <= vPos.y - (vScale.y / 2.f) || Y >= vPos.y + (vScale.y / 2.f)) continue;

		for (float j = startX; j < endX; j += 80.f)
		{
			vector<CObject*> vecSkill = pCurScene->GetGroupObject(GROUP_TYPE::SKILL);

			// 해당 위치에 Seed가 있는지 없는지.
			bool flag = false;

			for (int k = 0; k < vecSkill.size(); k++)
			{
				if (vecSkill[k]->GetName() == L"Seed")
				{
					//Vec2 vSkillPos = vecSkill[k]->GetPos();
					Vec2 vSkillPos = vecSkill[k]->GetComponent()->GetCollider()->GetFinalPos();
					//Vec2 vSkillScale = vecSkill[k]->GetScale();
					Vec2 vSkillScale = vecSkill[k]->GetComponent()->GetCollider()->GetScale();

					Vec2 vPoint = Vec2(j, Y - 30.f);

					// vPoint가 이미 Seed 안에 있다면.
					if (vSkillPos.x - (vSkillScale.x / 2.f) <= vPoint.x
						&& vSkillPos.x + (vSkillScale.x / 2.f) >= vPoint.x
						&& vSkillPos.y - (vSkillScale.y / 2.f) <= vPoint.y
						&& vSkillPos.y + (vSkillScale.y / 2.f) >= vPoint.y)
					{
						flag = true;
						break;
					}
				}
			}

			if (!flag)
			{
				CObject* pObj = (CObject*) new CSeed(_pObj);
				pObj->SetPos(Vec2(j, Y + 20.f));
				CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
				pCurScene->AddObject(pObj, GROUP_TYPE::SKILL);
			}
		}
	}
}

CBlessingSeed::~CBlessingSeed()
{
}


void CBlessingSeed::CalCastingTime()
{
	if (m_fCastTime < 0) DeleteObject(this);

	m_fCastTime -= fDT;
}

void CBlessingSeed::update()
{
	SetPos(m_pOwner->GetPos());

	CalCastingTime();
}

void CBlessingSeed::OnCollisionEnter(CCollider* _pOther)
{
}

void CBlessingSeed::OnCollision(CCollider* _pOther)
{
}

