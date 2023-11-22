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
	// 이미지 불러오기
	Image* image = Image::FromFile(_strFilePath.c_str());

	// 불러온 이미지로부터 width값과 height값 얻기
	UINT iWidth = image->GetWidth();
	UINT iHeight = image->GetHeight();

	// 이미지 크기에 맞는 bitmap생성
	Bitmap bitmap(iWidth, iHeight, PixelFormat32bppARGB);
	// bitmap 핸들 m_hBit에 연결하기
	bitmap.GetHBITMAP(0, &m_hBit);

	// 메인 DC에 호환되는 메모리 DC 생성
	m_dc = CreateCompatibleDC(CCore::GetInst()->GetMainDC());

	// 비트맵과 DC 연결
	HBITMAP hPrevBit = (HBITMAP)SelectObject(m_dc, m_hBit);
	// 이전 object는 필요 없으므로 삭제
	DeleteObject(hPrevBit);

	// GDI+를 이용해서 m_dc에 그림 그리기
	Graphics graphics(m_dc);

	if (!_iDir)
	{
		graphics.DrawImage(image, 0, 0, iWidth, iHeight);
	}
	else
	{
		graphics.DrawImage(image, iWidth, 0, (int)iWidth * -1, iHeight);
	}

	// m_hBit로부터 m_bitInfo에 비트맵 정보 채우기
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
