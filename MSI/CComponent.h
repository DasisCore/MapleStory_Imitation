#pragma once

class CObject;
class CCollider;
class CAnimator;
class CRigidBody;

class CComponent
{
private:
	CObject* m_pOwner;
	CCollider* m_pCollider;
	CAnimator* m_pAnimator;
	CRigidBody* m_pRigidbody;

public:
	CObject* GetObj() { return m_pOwner; }
	
	CCollider* GetCollider() { return m_pCollider; }
	CAnimator* GetAnimator() { return m_pAnimator; }
	CRigidBody* GetRigidbody() { return m_pRigidbody; }
	

public:
	void finalupdate();

public:
	CComponent();
	CComponent(const CComponent& _origin);
	~CComponent();

	friend class CObject;
};

