#include "global.h"
#include "CMarquee.h"
#include "CCamera.h"

CMarquee::CMarquee()
{
}

CMarquee::~CMarquee()
{
}

void CMarquee::update()
{
}

void CMarquee::render(HDC _dc)
{
	CObject::render(_dc);
	
	Graphics graphics(_dc);
	Pen pen(Color(255, 0, 0), 3);
	pen.SetDashStyle(DashStyleDash);

	Vec2 vPos = GetPos();
	vPos = CCamera::GetInst()->GetRenderPos(vPos);
	Vec2 vScale = GetScale();

	graphics.DrawRectangle(&pen, vPos.x - (vScale.x / 2.f), vPos.y - (vScale.y / 2.f), vScale.x, vScale.y);
}

