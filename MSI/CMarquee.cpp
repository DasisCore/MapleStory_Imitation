#include "global.h"
#include "CMarquee.h"
#include "CCamera.h"

#include "CKeyMgr.h"

CMarquee::CMarquee()
	: m_bTarget(1)
{
}

CMarquee::~CMarquee()
{
}

void CMarquee::update()
{
	if(KEY_TAP(KEY::LBTN))
	{
		m_vDragStart = CCamera::GetInst()->GetRealPos(MOUSE_POS);
	}

	if (m_bTarget)
	{
		bool bMouseOn = CKeyMgr::GetInst()->IsMouseInObj(this);

		Vec2 vCurPos = GetPos();

		if (bMouseOn && KEY_TAP(KEY::LBTN))
		{
			m_vDragStart = CCamera::GetInst()->GetRealPos(MOUSE_POS);
		}

		if (bMouseOn && KEY_HOLD(KEY::LBTN))
		{
			Vec2 vDiff = CCamera::GetInst()->GetRealPos(MOUSE_POS) - m_vDragStart;

			vCurPos += vDiff;
			SetPos(vCurPos);
			m_vDragStart = CCamera::GetInst()->GetRealPos(MOUSE_POS);
		}

		if (KEY_TAP(KEY::UP))
		{
			SetPos(vCurPos + Vec2(0.f, -0.2f));
		}

		if (KEY_TAP(KEY::RIGHT))
		{
			SetPos(vCurPos + Vec2(0.2f, 0.f));
		}

		if (KEY_TAP(KEY::DOWN))
		{
			SetPos(vCurPos + Vec2(0.f, 0.2f));
		}

		if (KEY_TAP(KEY::LEFT))
		{
			SetPos(vCurPos + Vec2(-0.2f, 0.f));
		}
	}
}

void CMarquee::render(HDC _dc)
{
	CObject::render(_dc);
	
	Graphics graphics(_dc);
	Pen pen(Color(255, 0, 0), 2);
	if (m_bTarget) pen.SetColor(Color(0, 0, 255));
	pen.SetDashStyle(DashStyleDash);

	Vec2 vPos = GetPos();
	vPos = CCamera::GetInst()->GetRenderPos(vPos);
	Vec2 vScale = GetScale();

	graphics.DrawRectangle(&pen, vPos.x - (vScale.x / 2.f), vPos.y - (vScale.y / 2.f), vScale.x, vScale.y);
}
