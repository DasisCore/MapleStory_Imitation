#pragma once

class CMonster;

enum class MON_TYPE
{
	PUPPET,
	NORMAL,
	EPIC,
	UNIQUE,
	LEGEND,
};

class CMonFactory
{
public:
	static CMonster* CreateMonster(MON_TYPE _eType, Vec2 _vPos);
	static CMonster* CreateMonster(MON_TYPE _eType
		, wstring _strName, Vec2 _vPos, Vec2 _vScale
		, bool _bCollider, Vec2 _vColOffset, Vec2 _vColScale
		, bool _bAnimation, vector<wstring> _vecPath
		, bool _bGravity, bool _bRigidBody);


private:
	CMonFactory(){};
	~CMonFactory(){};
};

