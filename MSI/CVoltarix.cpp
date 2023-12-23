#include "global.h"
#include "CVoltarix.h"

#include "CComponent.h"
#include "CAnimator.h"
#include "CCollider.h"
#include "CRigidBody.h"
#include "CGravity.h"
#include "CTexture.h"
#include "CResMgr.h"

#include "CKeyMgr.h"

CVoltarix::CVoltarix()
	:CPlayer()
{
	//CreateComponent();

	// 캐릭터 기본 콜라이더 크기 (45, 70)
	CreateCollider();
	GetComponent()->GetCollider()->SetScale(Vec2(45.f, 70.f));
	GetComponent()->GetCollider()->SetOffsetPos(Vec2(0.f, 0.f));

	//CreateRigidbody();
	//CreateAnimation();
	CreateGravity();

	//CTexture* m_pVoltarix_LeftTex = CResMgr::GetInst()->LoadTexture(L"VoltarixLeft", L"Texture\\Player\\Voltarix.png");
	//CTexture* m_pVoltarix_RightTex = CResMgr::GetInst()->LoadTexture(L"VoltarixRight", L"Texture\\Player\\Voltarix.png", 1);

	//GetComponent()->GetAnimator()->CreateAnimation_rewind(L"VOLTARIX_LEFT_IDLE", m_pVoltarix_LeftTex, Vec2(1302.f, 0.f), Vec2(73.f, 75.f), Vec2(73.f, 0.f), 0.5f, 3);
	//GetComponent()->GetAnimator()->CreateAnimation_rewind(L"VOLTARIX_RIGHT_IDLE", m_pVoltarix_RightTex, Vec2(375.f, 0.f), Vec2(73.f, 75.f), Vec2(73.f, 0.f), 0.5f, 3);

	//GetComponent()->GetAnimator()->CreateAnimation(L"VOLTARIX_LEFT_WALK", m_pVoltarix_LeftTex, Vec2(1522.f, 0.f), Vec2(73.f, 75.f), Vec2(73.f, 0.f), 0.1f, 4);
	//GetComponent()->GetAnimator()->CreateAnimation(L"VOLTARIX_RIGHT_WALK", m_pVoltarix_RightTex, Vec2(85.f, 0.f), Vec2(73.f, 75.f), Vec2(73.f, 0.f), 0.1f, 4);

	//GetComponent()->GetAnimator()->CreateAnimation(L"VOLTARIX_LEFT_JUMP", m_pVoltarix_LeftTex, Vec2(719.f, 0.f), Vec2(73.f, 75.f), Vec2(0.f, 0.f), 0.1f, 1);
	//GetComponent()->GetAnimator()->CreateAnimation(L"VOLTARIX_RIGHT_JUMP", m_pVoltarix_RightTex, Vec2(1105.f, 0.f), Vec2(73.f, 75.f), Vec2(0.f, 0.f), 0.1f, 1);

	//GetComponent()->GetAnimator()->CreateAnimation(L"VOLTARIX_LEFT_PRONE", m_pVoltarix_LeftTex, Vec2(937.f, 0.f), Vec2(85.f, 75.f), Vec2(0.f, 0.f), 1.f, 1);
	//GetComponent()->GetAnimator()->CreateAnimation(L"VOLTARIX_RIGHT_PRONE", m_pVoltarix_RightTex, Vec2(876.f, 0.f), Vec2(73.f, 75.f), Vec2(0.f, 0.f), 1.f, 1);

	//GetComponent()->GetAnimator()->CreateAnimation(L"VOLTARIX_LEFT_ATT_A", m_pVoltarix_LeftTex, Vec2(937.f, 0.f), Vec2(85.f, 75.f), Vec2(0.f, 0.f), 1.f, 1);
	//GetComponent()->GetAnimator()->CreateAnimation(L"VOLTARIX_RIGHT_ATT_A", m_pVoltarix_RightTex, Vec2(876.f, 0.f), Vec2(73.f, 75.f), Vec2(0.f, 0.f), 1.f, 1);
}

CVoltarix::~CVoltarix()
{
}

void CVoltarix::update_move()
{
	CPlayer::update_move();

	skill_teleport();
}

void CVoltarix::skill_teleport()
{
	CRigidBody* pRigid = GetComponent()->GetRigidbody();

	if (KEY_TAP(KEY::LSHIFT))
	{
		Vec2 vPos = GetPos();
		if (KEY_HOLD(KEY::UP))
		{
			vPos.y -= 200.f;
			pRigid->SetVelocity(Vec2(0.f, 0.f));
		}

		if (KEY_HOLD(KEY::LEFT))
		{
			vPos.x -= 200.f;
			pRigid->SetVelocity(Vec2(-100.f, pRigid->GetVelocity().y));
		}

		if (KEY_HOLD(KEY::RIGHT))
		{
			vPos.x += 200.f;
			pRigid->SetVelocity(Vec2(100.f, pRigid->GetVelocity().y));
		}

		if (KEY_HOLD(KEY::DOWN))
		{
			vPos.y += 200.f;
			pRigid->SetVelocity(Vec2(0.f, 0.f));
		}

		SetPos(vPos);
	}
}
