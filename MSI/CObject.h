#pragma once

#include "global.h"

class CObject
{
private:
	wstring m_strName;	// 오브젝트의 이름
	Vec2 m_vPos;		// 오브젝트의 위치
	Vec2 m_vScale;		// 오브젝트의 크기

public:
	void SetName(const wstring& _strName) { m_strName = _strName; }
	const wstring& GetName() { return m_strName; }

	Vec2 GetPos() { return m_vPos; }
	Vec2 GetScale() { return m_vScale; }

	void SetPos(Vec2 _vPos) { m_vPos = _vPos; }
	void SetScale(Vec2 _vScale) { m_vScale = _vScale; }

public:
	virtual void update() = 0;
	virtual void render(HDC _dc);

public:
	CObject();
	virtual ~CObject();
};

