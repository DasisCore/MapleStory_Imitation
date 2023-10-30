#pragma once

class CObject;
class CCollider;

class CComponent
{
private:
	CObject* m_pOwner;
	CCollider* m_pCollider;

public:
	CObject* GetObj() { return m_pOwner; }
	
	CCollider* GetCollider() { return m_pCollider; }
	

public:
	void finalupdate();

public:
	CComponent();
	CComponent(const CComponent& _origin);
	~CComponent();

	friend class CObject;
};

