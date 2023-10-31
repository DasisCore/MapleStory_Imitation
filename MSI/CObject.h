#pragma once

#include "global.h"

class CComponent;
class CCollider;
class CAnimator;

class CObject
{
private:
	wstring m_strName;	// ������Ʈ�� �̸�
	Vec2 m_vPos;		// ������Ʈ�� ��ġ
	Vec2 m_vScale;		// ������Ʈ�� ũ��

	CComponent* m_pComponent;

	bool m_bAlive;

public:
	void SetName(const wstring& _strName) { m_strName = _strName; }
	const wstring& GetName() { return m_strName; }

	Vec2 GetPos() { return m_vPos; }
	Vec2 GetScale() { return m_vScale; }

	void SetPos(Vec2 _vPos) { m_vPos = _vPos; }
	void SetScale(Vec2 _vScale) { m_vScale = _vScale; }

	bool IsDead() { return !m_bAlive; }

private:
	void SetDead() { m_bAlive = false; }


public:
	void CreateComponent();
	void CreateCollider();
	void CreateAnimation();

	virtual void OnCollision(CCollider* _pOther) {};
	virtual void OnCollisionEnter(CCollider* _pOther) {};
	virtual void OnCollisionExit(CCollider* _pOther) {};


public:
	virtual void update() = 0;
	virtual void finalupdate() final;	// �� �̻� �������̵��� �� �� ����
	virtual void render(HDC _dc);

	CComponent* GetComponent() { return m_pComponent; }
	void component_render(HDC _dc);

	// �ڱ� �ڽ��� ���� ������ �����ϴ� �Լ�
	virtual CObject* Clone() = 0;

public:
	CObject();
	CObject(const CObject& _origin);
	virtual ~CObject();

	friend class CEventMgr;
};
