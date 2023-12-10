#include "global.h"
#include "CWorkshopCheckBox.h"

#include "CWorkshopWindow.h"

CWorkshopCheckBox::CWorkshopCheckBox()
	: m_bIsCheck(false)
{
}

CWorkshopCheckBox::~CWorkshopCheckBox()
{
}

void CWorkshopCheckBox::render(HDC _dc)
{
	Graphics graphics(_dc);
	RectF checkBoxRect(GetPos().x, GetPos().y, GetScale().x, GetScale().y);
	if (m_bIsCheck)
	{
		DrawCheckBox(graphics, checkBoxRect, true);
	}
	else
	{
		DrawCheckBox(graphics, checkBoxRect, false);
	}
}

void CWorkshopCheckBox::DrawCheckBox(Graphics& graphics, const RectF& rect, bool checked)
{
	Pen pen(Color(0, 0, 0));
	graphics.DrawRectangle(&pen, rect);

	if (checked) {
		SolidBrush brush(Color(0, 0, 0));
		graphics.FillRectangle(&brush, rect);
	}
}

void CWorkshopCheckBox::update()
{
	CWorkshopBtn::update();
}

