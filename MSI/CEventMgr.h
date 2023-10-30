#pragma once

class CObject;

struct tEvent
{
	EVENT_TYPE eEvent;
	DWORD_PTR lParam;
	DWORD_PTR wParam;
};


class CEventMgr
{
	SINGLE(CEventMgr);

private:
	vector<tEvent> m_vecEvent;
	vector<CObject*> m_vecDead;

public:
	void update();

private:
	void Excute(const tEvent& _eve);	// 이벤트 처리

public:
	void AddEvent(const tEvent& _eve) { m_vecEvent.push_back(_eve); }

};

