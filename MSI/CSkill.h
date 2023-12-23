#pragma once

class CSkill
{
private:
	CObject* m_pOwner;
	bool m_bPassive;


public:
	//virtual void action();

public:
	CSkill(CObject* _pOwner, bool _bPassive);
	~CSkill();
};

