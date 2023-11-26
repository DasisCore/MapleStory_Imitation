#pragma once

class CState;
class CMonster;


class AI
{
private:
	map<MON_STATE, CState*> m_mapState;
	CState* m_pCurState;
	CMonster* m_pOwner;


public:
	void update();

public:
	void AddState(CState* _pState);
	CState* GetState(MON_STATE _eState);
	CState* GetCurState() { return m_pCurState; }
	void SetCurState(MON_STATE _eState);

	void ChangeState(MON_STATE _eState);

	CMonster* GetOwner() { return m_pOwner; }


public:
	AI();
	~AI();

	friend class CMonster;
};

