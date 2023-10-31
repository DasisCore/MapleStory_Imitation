#pragma once

class CObject;
class CCollider;
class CAnimator;

class CComponent
{
private:
	CObject* m_pOwner;
	CCollider* m_pCollider;
	CAnimator* m_pAnimator;

public:
	CObject* GetObj() { return m_pOwner; }
	
	CCollider* GetCollider() { return m_pCollider; }
	CAnimator* GetAnimator() { return m_pAnimator; }
	

public:
	void finalupdate();

public:
	CComponent();
	CComponent(const CComponent& _origin);
	~CComponent();

	friend class CObject;
};

