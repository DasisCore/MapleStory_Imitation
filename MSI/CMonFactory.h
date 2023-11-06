#pragma once

class CMonster;

enum class MON_TYPE
{
	NORMAL,
	RANGE,
};

class CMonFactory
{
public:
	static CMonster* CreateMonter(MON_TYPE _eType, Vec2 _vPos);



private:
	CMonFactory(){};
	~CMonFactory(){};
};

