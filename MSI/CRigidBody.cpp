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
	// ���� ũ��
	float fForce = m_vForce.Length();

	if (fForce != 0.f)
	{
		// ���� ����
		m_vForce.Normalize();

		// ���ӵ��� ũ��
		float m_fAccel = fForce / m_fMass;

		// ���ӵ�
		m_vAccel = m_vForce * m_fAccel;
	}

	// �߰� ���ӵ�
	m_vAccel += m_vAccelA;

	// �ӵ�
	m_vVelocity += m_vAccel * fDT;

	// ������ ����
	// ���� �������� ��� ��Ȳ���� �������� �߻��Ѵٴ� �����̹Ƿ�.
	// ���� ��Ȳ������ �������� �����ؾ���.
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
			// �������� ���� �ӵ����� ũ�ٸ�
			m_vVelocity = Vec2(0.f, 0.f);
		}
		else
		{
			m_vVelocity += vFriction;
		}
	}

	// �ӵ� ���� (�� ����)
	if (abs(m_vMaxVelocity.x) < abs(m_vVelocity.x))
	{
		m_vVelocity.x =	(m_vVelocity.x / abs(m_vVelocity.x)) * abs(m_vMaxVelocity.x);
	}

	if (abs(m_vMaxVelocity.y) < abs(m_vVelocity.y))
	{
		m_vVelocity.y = (m_vVelocity.y / abs(m_vVelocity.y)) * abs(m_vMaxVelocity.y);
	}

	// �ӵ��� ���� �̵�
	Move();

	// �� �ʱ�ȭ
	m_vForce = Vec2(0.f, 0.f);
	
	// �߰� ���ӵ� �ʱ�ȭ
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
