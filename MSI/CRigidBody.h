#pragma once

class CRigidBody
{
private:
	CObject* m_pOwner;

	Vec2 m_vForce;		// 크기, 방향
	Vec2 m_vAccel;		// 가속도
	Vec2 m_vVelocity;	// 속도

	float m_fMass;		// 질량
	float m_fMaxSpeed;	// 최대 속도
	float m_fFricCoeff;	// 마찰 계수

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

