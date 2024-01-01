#include "global.h"
#include "CDamege.h"
#include "CTimeMgr.h"

#include "CRandom.h"
#include "CPathMgr.h"
#include "CCamera.h"


CDamege::CDamege(int _iDamege)
	: m_iDamege(_iDamege)
	, m_fDisplay(0.35f)
	, m_fAlpha(255.f)
{
	CreateComponent();
	CreateAnimator();
	CreateCollider();

	int iDamege = _iDamege;
	int iWidth = 0;
	
	// �ڸ��� ã��.
	while (iDamege) 
	{
		iDamege /= 10;
		iWidth++;
	}
	
	iWidth *= 25;

	// ����� �������� �´� ũ���� ��Ʈ���� �����Ѵ�.
	// 300�� �ƴ϶� �ڸ���
	m_pBitmap = new Bitmap(iWidth, 40, PixelFormat32bppARGB);
	
	Graphics graphics(m_pBitmap);

	wstring contentPath = CPathMgr::GetInst()->GetContentPath();
	contentPath += L"Texture\\Damege\\";

	// 1234567890 -> 0987654321
	wstring number = std::to_wstring(_iDamege);
	reverse(number.begin(), number.end());
	
	for (int i = 0; i < number.length(); i++)
	{
		int iHeight = CRandom::GetInst()->GetBetweenInt(1, 3);
		wstring no = contentPath + L"NoRed" + number[i] + L".png";
		Image* numberImage = Image::FromFile(no.c_str());

		graphics.DrawImage(numberImage, (i * 23.5), iHeight, 31 + iHeight, 33 + iHeight);
	}
	// �ش� �������� �´� �̹����� �ҷ��� m_pBitmap�� �׸���.

	SetScale(Vec2(iWidth, 40));
}

CDamege::~CDamege()
{
}



void CDamege::update()
{
	if (m_fDisplay < 0) DeleteObject(this);

	Vec2 vPos = GetPos();
	SetPos(Vec2(vPos.x, vPos.y - (12.f * fDT)));
	m_fAlpha -= fDT * 550.f;
	m_fDisplay -= fDT;
}

void CDamege::render(HDC _dc)
{
	Vec2 vPos = GetPos();
	Vec2 vScale = GetScale();

	Graphics g(_dc);
	
	// ������ ��� �׵θ� (���� Ȯ�ο�)
	Pen pen(Color(100, 200, 255), 2.f);
	DrawWithAlpha(_dc, m_pBitmap, m_fAlpha);
}

void CDamege::DrawWithAlpha(HDC hdc, Image* image, float alpha)
{
	ImageAttributes imageAttributes;

	// ���İ� ����
	ColorMatrix colorMatrix = {
		1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, static_cast<float>(alpha) / 255.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f
	};
	imageAttributes.SetColorMatrix(&colorMatrix);

	Vec2 vPos = GetPos();
	vPos = CCamera::GetInst()->GetRenderPos(vPos);
	Vec2 vScale = GetScale();

	float fWidth = m_pBitmap->GetWidth();
	float fHeight = m_pBitmap->GetHeight();

	// Draw the image with alpha
	Graphics graphics(hdc);
	graphics.DrawImage(image
		, Rect(vPos.x - (vScale.x / 2.f) - 10.f, vPos.y - (vScale.y / 2.f) - 20.f, fWidth, fHeight)
		, 0, 0, fWidth, fHeight, UnitPixel, &imageAttributes);
}