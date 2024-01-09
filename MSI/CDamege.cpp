#include "global.h"
#include "CDamege.h"
#include "CTimeMgr.h"

#include "CRandom.h"
#include "CPathMgr.h"
#include "CCamera.h"

CDamege::CDamege(int _iDamege, DAMEGE_TYPE _eDamegeType)
	: m_iDamege(_iDamege)
	, m_pImage(nullptr)
	, m_fDisplay(0.4f)
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
	m_pBitmap = new Bitmap(iWidth + 10, 40, PixelFormat32bppARGB);
	
	Graphics graphics(m_pBitmap);

	wstring contentPath = CPathMgr::GetInst()->GetContentPath();
	contentPath += L"Texture\\Damege\\";

	// 1234567890 -> 0987654321
	wstring number = std::to_wstring(_iDamege);
	
	for (int i = 0; i < number.length(); i++)
	{
		int iHeight = CRandom::GetInst()->GetBetweenInt(1, 3);
		wstring damegeType;

		switch (_eDamegeType)
		{
		case DAMEGE_TYPE::NORED:
			damegeType = L"NORED";
			break;
		case DAMEGE_TYPE::VIOLET:
			damegeType = L"VIOLET";
			break;
		default:
			damegeType = L"NORED";
			break;
		}

		wstring absolutePath = contentPath + damegeType + number[i] + L".png";
		Image* numberImage = Image::FromFile(absolutePath.c_str());
		graphics.DrawImage(numberImage, (i * 24), iHeight, 31 + iHeight, 33 + iHeight);
		delete numberImage;
	}
	// �ش� �������� �´� �̹����� �ҷ��� m_pBitmap�� �׸���.

	SetScale(Vec2(iWidth, 40));
}

CDamege::~CDamege()
{
	if (m_pBitmap) delete m_pBitmap;
}


void CDamege::update()
{
	if (m_fDisplay < 0) DeleteObject(this);

	Vec2 vPos = GetPos();
	SetPos(Vec2(vPos.x, vPos.y - (20.f * fDT)));
	m_fAlpha -= fDT * 500.f;
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