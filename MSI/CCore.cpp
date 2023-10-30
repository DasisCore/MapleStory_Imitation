#include "global.h"

#include "CCore.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CSceneMgr.h"
#include "CPathMgr.h"
#include "CCollisionMgr.h"
#include "CEventMgr.h"

CCore::CCore()
	: m_hWnd{0}
	, m_ptResolution{0}
	, m_hDC{0}
	, m_hBit{0}
	, m_memDC{0}
	, m_arrBrush{}
	, m_arrPen{}
{

}
CCore::~CCore()
{
	ReleaseDC(m_hWnd, m_hDC);

	DeleteDC(m_memDC);
	DeleteObject(m_hBit);

	for (int i = 0; i < (UINT)PEN_TYPE::END; i++)
	{
		DeleteObject(m_arrPen[i]);
	}

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

	// =======================================
	// ���� ���۸��� ���� ��� ���� �ʱ�ȭ
	// =======================================
	m_hBit = CreateCompatibleBitmap(m_hDC, m_ptResolution.x, m_ptResolution.y);
	m_memDC = CreateCompatibleDC(m_hDC);

	HBITMAP hOldBit = (HBITMAP)SelectObject(m_memDC, m_hBit);
	DeleteObject(hOldBit);


	// ���� ����� �� �� �귯�� �ʱ�ȭ
	CreateBrushPen();


	// =======================================
	// �Ŵ��� �ʱ�ȭ
	// =======================================
	CPathMgr::GetInst()->init();
	CTimeMgr::GetInst()->init();
	CKeyMgr::GetInst()->init();
	CSceneMgr::GetInst()->init();

	return S_OK;
}

void CCore::progress()
{
	// =======================================
	// �� ������ �� �Ŵ��� ������Ʈ
	// =======================================
	CTimeMgr::GetInst()->update();
	CKeyMgr::GetInst()->update();

	CSceneMgr::GetInst()->update();

	CCollisionMgr::GetInst()->update();
	

	// =======================================
	// ������ �۾�
	// =======================================
	Rectangle(m_memDC, -1, -1, m_ptResolution.x + 11, m_ptResolution.y + 1);

	CSceneMgr::GetInst()->render(m_memDC);

	BitBlt(m_hDC, 0, 0, m_ptResolution.x, m_ptResolution.y, m_memDC, 0, 0, SRCCOPY);


	// =======================================
	// �̺�Ʈ ����ó��
	// =======================================
	CEventMgr::GetInst()->update();
}

void CCore::CreateBrushPen()
{
	m_arrBrush[(UINT)BRUSH_TYPE::HOLLOW] = (HBRUSH)GetStockObject(HOLLOW_BRUSH);

	m_arrPen[(UINT)PEN_TYPE::RED] = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	m_arrPen[(UINT)PEN_TYPE::BLUE] = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	m_arrPen[(UINT)PEN_TYPE::GREEN] = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));


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
}
