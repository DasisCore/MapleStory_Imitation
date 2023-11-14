#include "global.h"

#include "CCore.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CSceneMgr.h"
#include "CPathMgr.h"
#include "CCollisionMgr.h"
#include "CEventMgr.h"
#include "CCamera.h"
#include "CUIMgr.h"
#include "CResMgr.h"
#include "CCamera.h"
#include "CSoundMgr.h"

#include "CSound.h"

#include "CTexture.h"

#include "SelectGDI.h"
#include "resource.h"

CCore::CCore()
	: m_hWnd{0}
	, m_ptResolution{0}
	, m_hDC{0}
	, m_arrBrush{}
	, m_arrPen{}
	, m_gdiplusToken(0)
{

}
CCore::~CCore()
{
	ReleaseDC(m_hWnd, m_hDC);

	for (int i = 0; i < (UINT)PEN_TYPE::END; i++)
	{
		DeleteObject(m_arrPen[i]);
	}
	
	DestroyMenu(m_hMenu);

	// ���α׷� ����� GDI+ ����
	GdiplusShutdown(m_gdiplusToken);
}

int CCore::init(HWND _hWnd, POINT _ptResolution, HINSTANCE _hInstance)
{
	// �⺻ ��� ���� �ʱ�ȭ
	m_hWnd = _hWnd;
	m_ptResolution = _ptResolution;
	m_hDC = GetDC(m_hWnd);
	m_hInstance = _hInstance;

	// �ػ󵵿� �°� ������ ũ�� ����
	ChangeWindowSize(m_ptResolution, false);
	m_hMenu = LoadMenu(nullptr, MAKEINTRESOURCEW(IDC_MSI));


	// �� ó�� �������� ��ġ�� ȭ�� �� �߾����� ��ġ ��Ų��.
	// ������� �ػ� ���
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// ȭ�� �� �߾��� �� ���
	int centerX = (screenWidth - 1280) / 2;
	int centerY = (screenHeight - 720) / 2;

	RECT rt = { 0, 0, (long)_ptResolution.x, (long)_ptResolution.y };
	SetWindowPos(m_hWnd, nullptr, centerX, centerY, rt.right - rt.left, rt.bottom - rt.top, 0);


	// �޴��� ����
	//SetMenu(m_hWnd, nullptr);

	// =======================================
	// ���� ���۸� �뵵�� �ؽ�ó ����
	// =======================================
	m_pMemTex = CResMgr::GetInst()->CreateTexture(L"BackBuffer", (UINT)m_ptResolution.x, (UINT)m_ptResolution.y);


	// ���� ����� �� �� �귯�� �ʱ�ȭ
	CreateBrushPen();


	// =======================================
	// �Ŵ��� �ʱ�ȭ
	// =======================================
	CPathMgr::GetInst()->init();
	CTimeMgr::GetInst()->init();
	CKeyMgr::GetInst()->init();
	CCamera::GetInst()->init();
	CSoundMgr::GetInst()->init();
	CSceneMgr::GetInst()->init();


	// ���� �׽�Ʈ ���� fmod�� ����
	CResMgr::GetInst()->LoadSound(L"BGM_01", L"Sound\\DM.wav");
	//CSound* pNewSound = CResMgr::GetInst()->FindSound(L"BGM_01");

	//pNewSound->PlayToBGM(true);
	//pNewSound->Play();

	//pNewSound->SetPosition(0.f);	// ��з�, �Ҹ� ��ġ ���� (��� ��ġ)
	//pNewSound->SetVolume(1.f);


	// PNG �̹��� ó���� ���� GDI+ �ʱ�ȭ
	GdiplusStartupInput gdiplusStartupInput;
	gdiplusStartupInput.SuppressBackgroundThread = FALSE;	// PNG�� ALPHA ó��
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

	return S_OK;
}

void CCore::progress()
{
	// =======================================
	// �� ������ �� �Ŵ��� ������Ʈ
	// =======================================
	CTimeMgr::GetInst()->update();
	CKeyMgr::GetInst()->update();
	CCamera::GetInst()->update();

	// Scene update
	CSceneMgr::GetInst()->update();

	// �浹 üũ
	CCollisionMgr::GetInst()->update();
	
	// UI �̺�Ʈ üũ
	CUIMgr::GetInst()->update();


	// =======================================
	// ������ �۾�
	// =======================================

	HDC currentDC = m_pMemTex->GetDC();
	Clear();

	CSceneMgr::GetInst()->render(currentDC);
	CCamera::GetInst()->render(currentDC);

	BitBlt(m_hDC, 0, 0, m_ptResolution.x, m_ptResolution.y, currentDC, 0, 0, SRCCOPY);


	// =======================================
	// �̺�Ʈ ����ó��
	// =======================================
	CEventMgr::GetInst()->update();
}

void CCore::CreateBrushPen()
{
	m_arrBrush[(UINT)BRUSH_TYPE::HOLLOW] = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	m_arrBrush[(UINT)BRUSH_TYPE::BLACK] = (HBRUSH)GetStockObject(BLACK_BRUSH);

	m_arrPen[(UINT)PEN_TYPE::RED] = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	m_arrPen[(UINT)PEN_TYPE::BLUE] = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	m_arrPen[(UINT)PEN_TYPE::GREEN] = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
}

void CCore::Clear()
{
	HDC currentDC = m_pMemTex->GetDC();
	SelectGDI a(currentDC, BRUSH_TYPE::BLACK);
	Rectangle(currentDC, -1, -1, m_ptResolution.x + 11, m_ptResolution.y + 1);
}

void CCore::DockMenu()
{
	SetMenu(m_hWnd, m_hMenu);
	ChangeWindowSize(m_ptResolution, true);
}

void CCore::DivideMenu()
{
	SetMenu(m_hWnd, nullptr);
	ChangeWindowSize(m_ptResolution, false);
}

void CCore::ChangeWindowSize(Vec2 _vResolution, bool _bMenu)
{
	// �簢���� ��Ÿ���� ����ü RECT
	RECT rt = { 0, 0, (long)_vResolution.x, (long)_vResolution.y };
	
	// �޴��� ������ ���߾� window�� ũ�⸦ �����Ѵ�.
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, _bMenu);
	
	// ���� �������� ��ġ�� ���´�. �� ��ġ�� �����ϸ鼭 window�� ����� �����Ѵ�.
	HWND MainhWnd = CCore::GetInst()->GetMainHwnd();
	RECT mainWndRect;
	GetWindowRect(MainhWnd, &mainWndRect);

	int mainWndX = mainWndRect.left;
	int mainWndY = mainWndRect.top;

	// �������� ��ġ�� ũ�⸦ ����
	SetWindowPos(m_hWnd, nullptr, mainWndX, mainWndY, rt.right - rt.left, rt.bottom - rt.top, 0);
}

void CCore::render()
{
}
