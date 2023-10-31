#pragma once

class CObject;

class CCollider
{
private:
	static UINT g_iNextID;

	CObject* m_pOwner;

	Vec2 m_vOffsetPos;	// 오브젝트로부터 상대적인 위치
	Vec2 m_vFinalPos;	// finalupdate에서 매 프레임마다 계산
	Vec2 m_vScale;		// 충돌체의 크기

	UINT m_iID;			// 충돌체의 고유한 ID 값
	int m_iCol;

	bool m_bActive;		// 충돌체 활성화 여부

public:
	void SetOffsetPos(Vec2 _vPos) { m_vOffsetPos = _vPos; }
	void SetScale(Vec2 _vScale) { m_vScale = _vScale; }

	Vec2 GetOffssetPos() { return m_vOffsetPos; }
	Vec2 GetScale() { return m_vScale; }

	Vec2 GetFinalPos() { return m_vFinalPos; }

	CObject* GetObj() { return m_pOwner; }

	UINT GetID() { return m_iID; }

public:
	void OnCollision(CCollider* _pOther);
	void OnCollisionEnter(CCollider* _pOther);
	void OnCollisionExit(CCollider* _pOther);

public:
	void update();
	void render(HDC _dc);

	CCollider& operator = (CCollider* _origin) = delete;

public:
	CCollider();
	CCollider(const CCollider& _origin);
	~CCollider();

	friend class CObject;
	friend class CComponent;
};

