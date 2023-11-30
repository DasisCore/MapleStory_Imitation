#pragma once

#include "global.h"
#include "CCamera.h"

class CComponent;
class CCollider;
class CAnimator;

class CObject
{
private:
	wstring m_strName;	// 오브젝트의 이름
	Vec2 m_vPos;		// 오브젝트의 위치
	Vec2 m_vScale;		// 오브젝트의 크기

	CComponent* m_pComponent;

	bool m_bAlive;
	bool m_bGroundCheck;

public:
	void SetName(const wstring& _strName) { m_strName = _strName; }
	const wstring& GetName() { return m_strName; }

	Vec2 GetPos() { return m_vPos; }
	Vec2 GetScale() { return m_vScale; }

	void SetPos(Vec2 _vPos) { m_vPos = _vPos; }
	void SetScale(Vec2 _vScale) { m_vScale = _vScale; }

	bool IsDead() { return !m_bAlive; }
	bool GetGroundCheck() { return m_bGroundCheck; }
	void SetGroundCheck(bool _b) { m_bGroundCheck = _b; }

private:
	void SetDead() { m_bAlive = false; }


public:
	void CreateComponent();
	void CreateCollider();
	void CreateAnimation();
	void CreateRigidbody();
	void CreateGravity();

	virtual void OnCollision(CCollider* _pOther) {};
	virtual void OnCollisionEnter(CCollider* _pOther) {};
	virtual void OnCollisionExit(CCollider* _pOther) {};


public:
	virtual void start() {};	// Scene이 시작되기 직전에 호출되는 함수
	virtual void update() = 0;
	virtual void finalupdate();	// 더 이상 오버라이딩을 할 수 없음
	virtual void render(HDC _dc);

	CComponent* GetComponent() { return m_pComponent; }
	void component_render(HDC _dc);

	// 자기 자신의 복제 버전을 리턴하는 함수
	virtual CObject* Clone() = 0;

public:
	CObject();
	CObject(const CObject& _origin);
	virtual ~CObject();

	friend class CEventMgr;
};

