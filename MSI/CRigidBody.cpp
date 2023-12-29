#include "global.h"
#include "CRigidBody.h"

#include "CObject.h"
#include "CTimeMgr.h"
#include "CPlayer.h"

CRigidBody::CRigidBody()
	: m_pOwner(nullptr)
	, m_fMass(1.f)
	, m_fFricCoeff(900.f)
	, m_vMaxVelocity(Vec2(220.f, 800.f))
{
}

CRigidBody::~CRigidBody()
{
}

void CRigidBody::finalupdate()
{
	// 힘의 크기
	float fForce = m_vForce.Length();

	if (fForce != 0.f)
	{
		// 힘의 방향
		m_vForce.Normalize();

		// 가속도의 크기
		float m_fAccel = fForce / m_fMass;

		// 가속도
		m_vAccel = m_vForce * m_fAccel;
	}

	// 추가 가속도
	m_vAccel += m_vAccelA;

	// 속도
	m_vVelocity += m_vAccel * fDT;

	// 마찰력 적용
	// 현재 마찰력은 모든 상황에서 마찰력이 발생한다는 가정이므로.
	// 공중 상황에서의 마찰력은 제거해야함.
	bool flag = true;
	CPlayer* pPlayer = dynamic_cast<CPlayer*> (m_pOwner);
	if (pPlayer != nullptr && pPlayer->GetIsAir())
	{
		flag = false;
	}

	if (!m_vVelocity.IsZero())
	{
		if (!flag) m_fFricCoeff = 100.f;
		else m_fFricCoeff = 900.f;

		Vec2 vFricDir = -m_vVelocity;
		vFricDir.Normalize();

		Vec2 vFriction = vFricDir * m_fFricCoeff * fDT * m_fMass;

		if (m_vVelocity.Length() <= vFriction.Length())
		{
			// 마찰력이 본래 속도보다 크다면
			m_vVelocity = Vec2(0.f, 0.f);
		}
		else
		{
			m_vVelocity += vFriction;
		}
	}

	// 속도 제한 (축 별로)
	if (abs(m_vMaxVelocity.x) < abs(m_vVelocity.x))
	{
		m_vVelocity.x =	(m_vVelocity.x / abs(m_vVelocity.x)) * abs(m_vMaxVelocity.x);
	}

	if (abs(m_vMaxVelocity.y) < abs(m_vVelocity.y))
	{
		m_vVelocity.y = (m_vVelocity.y / abs(m_vVelocity.y)) * abs(m_vMaxVelocity.y);
	}

	// 속도에 따른 이동
	Move();

	// 힘 초기화
	m_vForce = Vec2(0.f, 0.f);
	
	// 추가 가속도 초기화
	m_vAccel = Vec2(0.f, 0.f);
	m_vAccelA = Vec2(0.f, 0.f);
}


void CRigidBody::Move()
{
	if (m_vVelocity.Length() != 0.f)
	{
		Vec2 vPos = m_pOwner->GetPos();
		vPos += m_vVelocity * fDT;

		m_pOwner->SetPos(vPos);
	}
}
