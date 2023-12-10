#pragma once
#include "CWorkshopBtn.h"


class CWorkshopCheckBox :
    public CWorkshopBtn
{
private:
	bool m_bIsCheck;

private:
	void DrawCheckBox(Graphics& graphics, const RectF& rect, bool checked);

public:
	bool GetIsCheck() { return m_bIsCheck; }
	void FlipISCheck() { m_bIsCheck = !m_bIsCheck; }

public:
	virtual void render(HDC _dc) override;
	virtual void update() override;

public:
	CWorkshopCheckBox();
	~CWorkshopCheckBox();
};

