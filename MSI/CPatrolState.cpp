#include "global.h"
#include "CPatrolState.h"

#include "CSceneMgr.h"
#include "CScene.h"

#include "CPlayer.h"
#include "CMonster.h"

#include "CRandom.h"
#include "CTimeMgr.h"

#include "CComponent.h"
#include "CRigidBody.h"


CPatrolState::CPatrolState()
	: CState(MON_STATE::PATROL)
	, m_fMoveTime(0.f)
	, m_iDir(1)
{
}

CPatrolState::~CPatrolState()
{
}



void CPatrolState::Enter()
{
	m_fMoveTime = CRandom::GetInst()->GetBetweenReal(1.f, 1.f);
	m_iDir = CRandom::GetInst()->GetRandomOne();
}

void CPatrolState::Exit()
{
}

void CPatrolState::update()
{
	Patrol();

	if (InRangePlayer())
	{
		ChangeAIState(GetAI(), MON_STATE::TRACE);
	}

	if (m_fMoveTime <= 0.f)
	{
		ChangeAIState(GetAI(), MON_STATE::IDLE);
	}

	m_fMoveTime -= fDT;
}

void CPatrolState::Patrol()
{
	CMonster* pMonster = GetMonster();
	CRigidBody* vMonRigid = pMonster->GetComponent()->GetRigidbody();

	float fMonScale = pMonster->GetScale().x;
	pMonster->SetMonDir(m_iDir);
	// ���� �̵�
	if (m_iDir == -1)
	{
		vMonRigid->SetVelocity(Vec2(-200.f, 0.f));
		if (pMonster->GetRemainDist().x < fMonScale)
		{
			if (CRandom::GetInst()->GetRandomBool())
			{
				vMonRigid->SetVelocity(Vec2(0.f, 0.f));
				ChangeAIState(GetAI(), MON_STATE::IDLE);
			}
			m_iDir = 1;
		}
	}
	// ���� �̵�
	else if (m_iDir == 1)
	{
		vMonRigid->SetVelocity(Vec2(200.f, 0.f));
		if (pMonster->GetRemainDist().y < fMonScale)
		{
			if (CRandom::GetInst()->GetRandomBool())
			{
				vMonRigid->SetVelocity(Vec2(0.f, 0.f));
				ChangeAIState(GetAI(), MON_STATE::IDLE);
			}
			m_iDir = -1;
		}
	}
}

bool CPatrolState::InRangePlayer()
{
	// Player�� ��ġ üũ
	CPlayer* pPlayer = (CPlayer*)CSceneMgr::GetInst()->GetCurScene()->GetPlayer();
	if (pPlayer == nullptr) return false;

	Vec2 vPlayerPos = pPlayer->GetPos();
	Vec2 vPlayerScale = pPlayer->GetScale();

	// ������ ���� �ȿ� ������ �������·� ��ȯ
	CMonster* pMonster = GetMonster();
	Vec2 vMonPos = pMonster->GetPos();
	Vec2 vMonScale = pMonster->GetScale();

	// ������ ���� ��ǥ
	float fMonLeftX = vMonPos.x - (vPlayerScale.x / 2.f);
	float fMonLeftY = vMonPos.y + (vMonScale.y / 2.f);

	// �÷��̾��� ���� ��ǥ
	float fPlayerLeftX = vPlayerPos.x - (vPlayerScale.x / 2.f);
	// �÷��̾��� ���� ��ǥ
	float fPlayerRightX = vPlayerPos.x + (vPlayerScale.x / 2.f);
	// �÷��̾��� �غ� ��ǥ
	float fPlayerY = vPlayerPos.y + (vPlayerScale.y / 2.f);

	// ������ �����Ÿ�
	float fMonRcogRangeX = pMonster->GetInfo().vRecogRange.x;

	// �÷��̾ ������ �������� ��쿡�� �������� ���Ƿ�,
	// ���� ������ ������ ũ�� ����� ����.
	if (pMonster->GetInfo().iDir == 1)
	{
		if (vMonPos.x < fPlayerLeftX && fPlayerLeftX < vMonPos.x + fMonRcogRangeX)
		{
			if (fMonLeftY >= fPlayerY && fPlayerY >= fMonLeftY - vMonScale.y)
			{
				return true;
			}
		}
	}
	else if (pMonster->GetInfo().iDir == -1)
	{
		if (vMonPos.x > fPlayerRightX && fPlayerRightX > vMonPos.x - fMonRcogRangeX)
		{
			if (fMonLeftY >= fPlayerY && fPlayerY >= fMonLeftY - vMonScale.y)
			{
				return true;
			}
		}
	}

	return false;
}

