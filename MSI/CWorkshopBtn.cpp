#include "global.h"
#include "CWorkshopBtn.h"
#include "CWorkshopWindow.h"

#include "CKeyMgr.h"

CWorkshopBtn::CWorkshopBtn()
	: m_pFunc(nullptr)
	, m_param1(0)
	, m_param2(0)
	, m_bMouseOn(false)
	, m_bLbtnDown(false)
	, color(Color(0, 0, 0))
{
}

CWorkshopBtn::~CWorkshopBtn()
{
}

void CWorkshopBtn::render(HDC _dc)
{
	Graphics graphics(_dc);

	Pen pen(color, 1.f);

	Vec2 vPos = GetPos();
	Vec2 vScale = GetScale();
	graphics.DrawRectangle(&pen, Rect(vPos.x, vPos.y, vScale.x, vScale.y));

	Font font(L"Arial", 8);
	SolidBrush brush(color);
	graphics.DrawString(GetName().c_str(), -1, &font, PointF(vPos.x + 3.f, vPos.y), &brush);

}

void CWorkshopBtn::update()
{
	HWND workshopHwnd = CWorkshopWindow::GetInst()->GetWorkshophWnd();
	POINT ptPos = {};
	GetCursorPos(&ptPos);
	ScreenToClient(workshopHwnd, &ptPos);

	// 마우스 위치 (그려지고 있는 Control DC 기준)
	Vec2 vCurMousePos = Vec2(ptPos) - Vec2(80.f, 190.f);

	Vec2 vPos = GetPos();
	Vec2 vScale = GetScale();

	Vec2 vLT = Vec2(vPos.x - (vScale.x / 2.f), vPos.y - (vScale.y / 2.f));

	if (vPos.x <= vCurMousePos.x && vCurMousePos.x <= vPos.x + vScale.x
		&& vPos.y <= vCurMousePos.y && vCurMousePos.y <= vPos.y + vScale.y)
	{
		MouseOn();
		m_bMouseOn = true;
		color = Color(255, 0, 0);
	}
	else
	{
		m_bMouseOn = false;
		m_bLbtnDown = false;
		color = Color(0, 0, 0);
	}

	if (m_bMouseOn)
	{
		if (KEY_TAP(KEY::LBTN))
		{
			MouseLbtnDown();
			m_bLbtnDown = true;
		}

		if (KEY_AWAY(KEY::LBTN))
		{
			MouseLbtnUp(); 
			if (m_bLbtnDown) MouseLbtnClicked();
			m_bLbtnDown = false;
		}
	}
}

void CWorkshopBtn::MouseOn()
{
	
}

void CWorkshopBtn::MouseLbtnDown()
{
}

void CWorkshopBtn::MouseLbtnUp()
{
}

void CWorkshopBtn::MouseLbtnClicked()
{
	if (m_pFunc != nullptr)
	{
		m_pFunc(m_param1, m_param2, m_param3);
	}
}


