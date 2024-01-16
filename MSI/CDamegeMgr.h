#pragma once
#include "CDamege.h"


class CObject;

struct DAMEGE_INFO
{
	CObject* pObj;
	float fDamege;
	DAMEGE_TYPE eType;
	float fOffset;
	float flatencyTime;
};


class CDamegeMgr
{
	SINGLE(CDamegeMgr);

private:
	vector<DAMEGE_INFO> m_vecDamege;

public:
	void init();
	void update();
	void AddDamege(DAMEGE_INFO _tDamegeInfo) { m_vecDamege.push_back(_tDamegeInfo); };
};

