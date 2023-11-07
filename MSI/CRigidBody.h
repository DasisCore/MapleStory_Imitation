#pragma once

class CRigidBody
{
private:
	CObject* m_pOwner;

	Vec2 m_vForce;		// ũ��, ����
	Vec2 m_vAccel;		// ���ӵ�
	Vec2 m_vVelocity;	// �ӵ�

	float m_fMass;		// ����
	float m_fMaxSpeed;	// �ִ� �ӵ�
	float m_fFricCoeff;	// ���� ���

public:
	void AddForce(Vec2 _vF) { m_vForce += _vF; }

	void SetMass(float _fMass) { m_fMass = _fMass; }
	float GetMass() { return m_fMass; }

	void SetVelocity(Vec2 _v) { m_vVelocity = _v; }
	void AddVelocity(Vec2 _v) { m_vVelocity += _v; }
	void SetMaxVelocity(float _fSpeed) { m_fMaxSpeed = _fSpeed; }

private:
	void Move();


public:
	void finalupdate();


public:
	CRigidBody();
	~CRigidBody();
	
	friend class CComponent;
	friend class CObject;
};

