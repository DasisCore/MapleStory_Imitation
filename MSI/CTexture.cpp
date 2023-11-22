#include "global.h"

#include "CTexture.h"
#include "CCore.h"

#include "SelectGDI.h"

CTexture::CTexture()
	: m_dc(0)
	, m_hBit(0)
	, m_bitInfo{}
{

}

CTexture::~CTexture()
{
	DeleteDC(m_dc);
	DeleteObject(m_hBit);
}


void CTexture::Load(const wstring& _strFilePath, int _iDir)
{
	// �̹��� �ҷ�����
	Image* image = Image::FromFile(_strFilePath.c_str());

	// �ҷ��� �̹����κ��� width���� height�� ���
	UINT iWidth = image->GetWidth();
	UINT iHeight = image->GetHeight();

	// �̹��� ũ�⿡ �´� bitmap����
	Bitmap bitmap(iWidth, iHeight, PixelFormat32bppARGB);
	// bitmap �ڵ� m_hBit�� �����ϱ�
	bitmap.GetHBITMAP(0, &m_hBit);

	// ���� DC�� ȣȯ�Ǵ� �޸� DC ����
	m_dc = CreateCompatibleDC(CCore::GetInst()->GetMainDC());

	// ��Ʈ�ʰ� DC ����
	HBITMAP hPrevBit = (HBITMAP)SelectObject(m_dc, m_hBit);
	// ���� object�� �ʿ� �����Ƿ� ����
	DeleteObject(hPrevBit);

	// GDI+�� �̿��ؼ� m_dc�� �׸� �׸���
	Graphics graphics(m_dc);

	if (!_iDir)
	{
		graphics.DrawImage(image, 0, 0, iWidth, iHeight);
	}
	else
	{
		graphics.DrawImage(image, iWidth, 0, (int)iWidth * -1, iHeight);
	}

	// m_hBit�κ��� m_bitInfo�� ��Ʈ�� ���� ä���
	GetObject(m_hBit, sizeof(BITMAP), &m_bitInfo);
}

void CTexture::Create(UINT _iWidth, UINT _iHeight)
{
	HDC mainDC = CCore::GetInst()->GetMainDC();

	m_hBit = CreateCompatibleBitmap(mainDC, _iWidth, _iHeight);
	m_dc = CreateCompatibleDC(mainDC);

	HBITMAP hOldBit = (HBITMAP)SelectObject(m_dc, m_hBit);
	DeleteObject(hOldBit);

	GetObject(m_hBit, sizeof(BITMAP), &m_bitInfo);
}
