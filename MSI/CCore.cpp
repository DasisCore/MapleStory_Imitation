#include "global.h"

#include "CCore.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"

CCore::CCore()
	: m_hWnd{0}
	, m_ptResolution{0}
	, m_hDC{0}
	, m_hBit{0}
	, m_memDC{0}

{

}
CCore::~CCore()
{

}

int CCore::init(HWND _hWnd, POINT _ptResolution)
{
	// �⺻ ��� ���� �ʱ�ȭ
	m_hWnd = _hWnd;
	m_ptResolution = _ptResolution;
	m_hDC = GetDC(m_hWnd);

	// �ػ󵵿� �°� ������ ũ�� ����
	ChangeWindowSize(GetResolution(), false);

	// �޴��� ����
	SetMenu(m_hWnd, nullptr);

	///////////////////////////////////////////////
	// ���� ���۸��� ���� ��� ���� �ʱ�ȭ
	///////////////////////////////////////////////
	m_hBit = CreateCompatibleBitmap(m_hDC, m_ptResolution.x, m_ptResolution.y);
	m_memDC = CreateCompatibleDC(m_hDC);

	HBITMAP hOldBit = (HBITMAP)SelectObject(m_memDC, m_hBit);
	DeleteObject(hOldBit);

	///////////////////////////////////////////////
	// �Ŵ��� �ʱ�ȭ
	///////////////////////////////////////////////
	CKeyMgr::GetInst()->init();
	CTimeMgr::GetInst()->init();

	return S_OK;
}

void CCore::progress()
{

	//////////////////////////////////////////////
	// �� ������ �� �Ŵ��� ������Ʈ
	//////////////////////////////////////////////
	CKeyMgr::GetInst()->update();
	CTimeMgr::GetInst()->update();



	render();

}

void CCore::ChangeWindowSize(Vec2 _vResolution, bool _bMenu)
{
	// �簢���� ��Ÿ���� ����ü RECT
	RECT rt = { 0, 0, (long)_vResolution.x, (long)_vResolution.y };
	
	// �޴��� ������ ���߾� window�� ũ�⸦ �����Ѵ�.
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, _bMenu);
	
	// �������� ��ġ�� ũ�⸦ ����
	SetWindowPos(m_hWnd, nullptr, 100, 100, rt.right - rt.left, rt.bottom - rt.top, 0);
}

void CCore::render()
{
	Rectangle(m_memDC, -1, -1, m_ptResolution.x + 11, m_ptResolution.y + 1);

	static float x = 100.f;
	static float y = 100.f;

	if (KEY_HOLD(KEY::UP))
	{
		x -= 100.f * DT;
	}

	if (KEY_HOLD(KEY::RIGHT))
	{
		y += 100.f * DT;
	}

	if(KEY_HOLD(KEY::DOWN))
	{
		x += 100.f * DT;
	}

	if (KEY_HOLD(KEY::LEFT))
	{
		y -= 100.f * DT;
	}

	Rectangle(m_memDC, y, x, y + 100, x + 100);

	BitBlt(m_hDC, 0, 0, m_ptResolution.x, m_ptResolution.y, m_memDC, 0, 0, SRCCOPY);
}
