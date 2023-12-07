#include "global.h"
#include "main.h"
#include "CWorkshopWindow.h"
#include "CCore.h"
#include "CSprite.h"
#include "CMarquee.h"

#include "CWorkshopBtn.h"

#include "CKeyMgr.h"

#include "CScene.h"
#include "CSceneMgr.h"

void ControlFrameInfo(DWORD_PTR _param1, DWORD_PTR _param2, DWORD_PTR _param3);
LRESULT CALLBACK WorkshopWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

CWorkshopWindow::CWorkshopWindow()
    : m_hWndWorkshop(nullptr)
    , m_WorkshopMainDC(nullptr)
    , m_hWndList(nullptr)
    , m_memFrameDC(nullptr)
    , m_memControlDC(nullptr)
    , m_iTargetFrame(-1)
    , m_fDuration(1.f)
{
}

CWorkshopWindow::~CWorkshopWindow()
{
    DeleteDC(m_memFrameDC);
    DeleteDC(m_WorkshopMainDC);

    if (m_hWndWorkshop) DestroyWindow(m_hWndWorkshop);

    list<CWorkshopBtn*>::iterator iter = m_lBtn.begin();
    for (; iter != m_lBtn.end(); iter++) delete (*iter);
}

void CWorkshopWindow::init()
{
    HINSTANCE hInst = CCore::GetInst()->GetMainhInstance();

    const wchar_t CLASS_NAME[] = L"Animation Window";

    WNDCLASSEXW wcex_workshop;

    wcex_workshop.cbSize = sizeof(WNDCLASSEX);

    wcex_workshop.style = CS_HREDRAW | CS_VREDRAW;
    wcex_workshop.lpfnWndProc = WorkshopWndProc;
    wcex_workshop.cbClsExtra = 0;
    wcex_workshop.cbWndExtra = 0;
    wcex_workshop.hInstance = hInst;
    wcex_workshop.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_MSI));
    wcex_workshop.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex_workshop.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex_workshop.lpszMenuName = nullptr; // MAKEINTRESOURCEW(IDC_MSI);
    wcex_workshop.lpszClassName = CLASS_NAME;
    wcex_workshop.hIconSm = LoadIcon(wcex_workshop.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    RegisterClassExW(&wcex_workshop);

    HWND subhWnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"Frame Manager",                // Window text
        WS_OVERLAPPEDWINDOW & ~WS_SIZEBOX & ~WS_MAXIMIZEBOX,            // Window style
        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 720,
        NULL,       // Parent window    
        NULL,       // Menu
        hInst,      // Instance handle
        NULL        // Additional application data
    );

    // 오류 검출
    assert(subhWnd);

    // 최대화 및 최소화 버튼을 숨김
    SetWindowLong(subhWnd, GWL_STYLE, GetWindowLong(subhWnd, GWL_STYLE) & ~(WS_MAXIMIZEBOX | WS_MINIMIZEBOX));

    m_hWndWorkshop = subhWnd;

    // 리스트 뷰 생성
    // 리스트 뷰 초기화
    InitCommonControls();
    m_hWndList = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTVIEW, L"List view", WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT | LVS_SINGLESEL | LVS_EX_DOUBLEBUFFER,
        10, 200, 70, 200, subhWnd, NULL, hInst, NULL);

    // ListView 열 추가
    LVCOLUMN lvColumn;
    lvColumn.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
    lvColumn.pszText = (LPWSTR)L"프레임";
    lvColumn.cx = 65;
    lvColumn.iSubItem = 0;
    ListView_InsertColumn(m_hWndList, 0, &lvColumn);

    ShowWindow(m_hWndList, SW_SHOWNORMAL);

    shiftWindow();

    // 더블 버퍼링 준비
    m_WorkshopMainDC = GetDC(m_hWndWorkshop);
    m_memFrameDC = CreateCompatibleDC(m_WorkshopMainDC);
    m_memControlDC = CreateCompatibleDC(m_WorkshopMainDC);
    HBITMAP memFrameBit = CreateCompatibleBitmap(m_WorkshopMainDC, 300, 190);
    HBITMAP memControlBit = CreateCompatibleBitmap(m_WorkshopMainDC, 210, 220);

    SelectObject(m_memFrameDC, memFrameBit);
    SelectObject(m_memControlDC, memControlBit);
    DeleteObject(memFrameBit);
    DeleteObject(memControlBit);

    // UI 초기화
    UIinit();
}

void CWorkshopWindow::update()
{
    // workshop window를 항상 최상위에 위치시킨다.
    SetWindowPos(m_hWndWorkshop, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    UI_update();
}

void CWorkshopWindow::render(HDC _dc)
{
    FrameRender(_dc);
    
    UI_render(m_memControlDC);
}

void CWorkshopWindow::FrameRender(HDC _dc)
{
    // 이 함수에서의 _dc의 경우 메인 윈도우의 DC이기 때문에 CWorkshopWindow에 있는 memDC를 사용해야 한다.
    Graphics graphics(m_memFrameDC);

    // 구분을 위해 일단 연노랑으로 채워놓음
    graphics.Clear(Color(255, 255, 255, 128));

    Font font(L"Arial", 8);
    SolidBrush brush(Gdiplus::Color(255, 0, 0, 0)); // 검은색 브러시

    wstring temp = L"현재 프레임 : " + std::to_wstring(m_iTargetFrame + 1);
    graphics.DrawString(temp.c_str(), -1, &font, PointF(5.f, 5.f), &brush);

    CScene_Ani_Workshop* pAniWorkshop = (CScene_Ani_Workshop*)CSceneMgr::GetInst()->GetCurScene();

    // 사이즈가 1개라면 아무 것도 없음.
    if (pAniWorkshop->GetFrameList().size() == 1) m_iTargetFrame = -1;

    if (m_iTargetFrame != -1)
    {
        // 스프라이트 이미지 가져오기
        CSprite* pSprite = pAniWorkshop->GetSprite();
        Image* pImage = pSprite->GetSprite();

        tFrame tFrm = pAniWorkshop->GetFrameInfo(m_iTargetFrame + 1);

        Vec2 vLT = tFrm.vLT;
        Vec2 vScale = tFrm.vSliceSize;

        // 300, 190 비트맵의 중앙에 그려야 함.
        Vec2 vDrawLT = Vec2(284.f / 2.f, 190.f / 2.f);

        // 가로세로 비율
        float fRatio = vScale.x / vScale.y;

        Vec2 vConvert = vScale;

        if (vConvert.x >= 264.f)
        {
            vConvert.x = 264.f;
            vConvert.y = 264.f / fRatio;
        }

        if (vConvert.y >= 150.f)
        {
            vConvert.y = 150.f;
            vConvert.x = 150.f * fRatio;
        }

        vDrawLT.x -= (vConvert.x / 2.f);
        vDrawLT.y -= (vConvert.y / 2.f);

        // 그릴 위치 및 크기 설정
        Rect destRect(vDrawLT.x, vDrawLT.y, vConvert.x, vConvert.y);

        //graphics.DrawImage(pImage, destRect, srcRect.X, srcRect.Y, srcRect.Width, srcRect.Height, UnitPixel);
        graphics.DrawImage(pImage, destRect, vLT.x, vLT.y, vScale.x, vScale.y, UnitPixel);
    }

    BitBlt(m_WorkshopMainDC, 0, 0, 300, 190, m_memFrameDC, 0, 0, SRCCOPY);
}



void CWorkshopWindow::AddFrame(UINT _idx)
{
    // ListView 아이템 추가
    wstring temp1 = L"Frame ";
    temp1 += std::to_wstring(_idx);
    LVITEM lvItem;
    lvItem.mask = LVIF_TEXT;
    lvItem.iItem = _idx;
    lvItem.iSubItem = 0;
    lvItem.pszText = const_cast<LPWSTR>(temp1.c_str());

    ListView_InsertItem(m_hWndList, &lvItem);
}

void CWorkshopWindow::DeleteFrame()
{
    int itemCount = ListView_GetItemCount(m_hWndList) - 1;
    ListView_DeleteItem(m_hWndList, itemCount);

    // 리스트 뷰를 갱신하여 삭제된 아이템을 반영
    ListView_RedrawItems(m_hWndList, 0, ListView_GetItemCount(m_hWndList) - 1);
}

void CWorkshopWindow::showWindow()
{
    ShowWindow(m_hWndWorkshop, 1);
    shiftWindow();
    UpdateWindow(m_hWndWorkshop);
    SetFocus(CCore::GetInst()->GetMainHwnd());
}

void CWorkshopWindow::Destroy_window()
{
    if (m_hWndWorkshop) DestroyWindow(m_hWndWorkshop);
}

void CWorkshopWindow::shiftWindow()
{
    HWND MainhWnd = CCore::GetInst()->GetMainHwnd();

    RECT mainWndRect;
    GetWindowRect(MainhWnd, &mainWndRect);

    int mainWndX = mainWndRect.right - 10;
    int mainWndY = mainWndRect.top;

    // 변경된 스타일을 적용
    SetWindowPos(m_hWndWorkshop, nullptr, mainWndX, mainWndY, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_FRAMECHANGED);
}


// =========================================================================================
// Controller 관련 메소드들
// =========================================================================================

void CWorkshopWindow::UIinit()
{
    // UI의 구조적 한계 때문에 이 부분은 하드코딩.
    CWorkshopBtn* pBtnUI = new CWorkshopBtn;
    pBtnUI->SetName(L"▲");
    pBtnUI->SetScale(Vec2(20.f, 15.f));
    pBtnUI->SetPos(Vec2(145.f, 40.f));
    m_lBtn.push_back(pBtnUI);

    pBtnUI = new CWorkshopBtn;
    pBtnUI->SetName(L"▼");
    pBtnUI->SetScale(Vec2(20.f, 15.f));
    pBtnUI->SetPos(Vec2(165.f, 40.f));
    m_lBtn.push_back(pBtnUI);

    //

    pBtnUI = new CWorkshopBtn;
    pBtnUI->SetName(L"▲");
    pBtnUI->SetScale(Vec2(20.f, 15.f));
    pBtnUI->SetPos(Vec2(145.f, 70.f));
    m_lBtn.push_back(pBtnUI);

    pBtnUI = new CWorkshopBtn;
    pBtnUI->SetName(L"▼");
    pBtnUI->SetScale(Vec2(20.f, 15.f));
    pBtnUI->SetPos(Vec2(165.f, 70.f));
    m_lBtn.push_back(pBtnUI);

    //

    pBtnUI = new CWorkshopBtn;
    pBtnUI->SetName(L"▲");
    pBtnUI->SetScale(Vec2(20.f, 15.f));
    pBtnUI->SetPos(Vec2(145.f, 100.f));
    m_lBtn.push_back(pBtnUI);

    pBtnUI = new CWorkshopBtn;
    pBtnUI->SetName(L"▼");
    pBtnUI->SetScale(Vec2(20.f, 15.f));
    pBtnUI->SetPos(Vec2(165.f, 100.f));
    m_lBtn.push_back(pBtnUI);

    //

    pBtnUI = new CWorkshopBtn;
    pBtnUI->SetName(L"▲");
    pBtnUI->SetScale(Vec2(20.f, 15.f));
    pBtnUI->SetPos(Vec2(145.f, 130.f));
    m_lBtn.push_back(pBtnUI);

    pBtnUI = new CWorkshopBtn;
    pBtnUI->SetName(L"▼");
    pBtnUI->SetScale(Vec2(20.f, 15.f));
    pBtnUI->SetPos(Vec2(165.f, 130.f));
    m_lBtn.push_back(pBtnUI);

    //

    pBtnUI = new CWorkshopBtn;
    pBtnUI->SetName(L"▲");
    pBtnUI->SetScale(Vec2(20.f, 15.f));
    pBtnUI->SetPos(Vec2(145.f, 160.f));
    m_lBtn.push_back(pBtnUI);

    pBtnUI = new CWorkshopBtn;
    pBtnUI->SetName(L"▼");
    pBtnUI->SetScale(Vec2(20.f, 15.f));
    pBtnUI->SetPos(Vec2(165.f, 160.f));
    m_lBtn.push_back(pBtnUI);
}

void CWorkshopWindow::UI_update()
{
    // 원래는 UI Object로 Scene안에 넣어야하지만, 
    // workshop이라는 특수성 때문에 새로운 클래스를 만들어 직접 작동시켜주었다.

    m_iTargetFrame;
    CScene_Ani_Workshop* pAniWorkshop = (CScene_Ani_Workshop*)CSceneMgr::GetInst()->GetCurScene();

    int iTargetFrame = m_iTargetFrame - 1;
    CMarquee* pMarquee = pAniWorkshop->GetMarquee(iTargetFrame);

    // 하드코딩을 하면 안되는 이유... // UI를 이용해 Marquee를 미세조정하는 버튼에 함수 할당.
    int idx = 0;
    list<CWorkshopBtn*>::iterator iter = m_lBtn.begin();
    for (; iter != m_lBtn.end(); iter++)
    {
        if      (idx == 0) (*iter)->SetClickCallBack(ControlFrameInfo, (DWORD_PTR)pMarquee, 1, 1);
        else if (idx == 1) (*iter)->SetClickCallBack(ControlFrameInfo, (DWORD_PTR)pMarquee, 1, -1);
        else if (idx == 2) (*iter)->SetClickCallBack(ControlFrameInfo, (DWORD_PTR)pMarquee, 2, 1);
        else if (idx == 3) (*iter)->SetClickCallBack(ControlFrameInfo, (DWORD_PTR)pMarquee, 2, -1);
        else if (idx == 4) (*iter)->SetClickCallBack(ControlFrameInfo, (DWORD_PTR)pMarquee, 3, 1);
        else if (idx == 5) (*iter)->SetClickCallBack(ControlFrameInfo, (DWORD_PTR)pMarquee, 3, -1);
        else if (idx == 6) (*iter)->SetClickCallBack(ControlFrameInfo, (DWORD_PTR)pMarquee, 4, 1);
        else if (idx == 7) (*iter)->SetClickCallBack(ControlFrameInfo, (DWORD_PTR)pMarquee, 4, -1);
        else if (idx == 8) (*iter)->SetClickCallBack(ControlFrameInfo, (DWORD_PTR)pMarquee, 5, 1);
        else if (idx == 9) (*iter)->SetClickCallBack(ControlFrameInfo, (DWORD_PTR)pMarquee, 5, -1);
        (*iter)->update();
        idx++;
    }
    
}

void CWorkshopWindow::UI_render(HDC _dc)
{
    CScene_Ani_Workshop* pAniWorkshop = (CScene_Ani_Workshop*)CSceneMgr::GetInst()->GetCurScene();

    Graphics graphics(_dc);
    graphics.Clear(Color(255, 200, 255));
    
    Font font(L"맑은 고딕", 9);
    SolidBrush BlackBrush(Color(0, 0, 0));
    SolidBrush GrayBrush(Color(84, 84, 84));

    int FrameCount = pAniWorkshop->GetFrameList().size() - 1;

    Vec2 vtargetLT = pAniWorkshop->GetFrameInfo(m_iTargetFrame).vLT;
    Vec2 vtargetSliceSize = pAniWorkshop->GetFrameInfo(m_iTargetFrame).vSliceSize;

    graphics.DrawString(L"총 프레임 수", -1, &font, PointF(10, 10), &GrayBrush);
    graphics.DrawString(std::to_wstring(FrameCount).c_str(), -1, &font, PointF(100, 10), &BlackBrush);

    graphics.DrawString(L"X", -1, &font, PointF(10, 40), &GrayBrush);
    graphics.DrawString(setprecision_float(vtargetLT.x, 1).c_str(), -1, &font, PointF(100, 40), &BlackBrush);

    graphics.DrawString(L"Y", -1, &font, PointF(10, 70), &GrayBrush);
    graphics.DrawString(setprecision_float(vtargetLT.y, 1).c_str(), -1, &font, PointF(100, 70), &BlackBrush);

    graphics.DrawString(L"너비", -1, &font, PointF(10, 100), &GrayBrush);
    graphics.DrawString(setprecision_float(vtargetSliceSize.x, 1).c_str(), -1, &font, PointF(100, 100), &BlackBrush);

    graphics.DrawString(L"높이", -1, &font, PointF(10, 130), &GrayBrush);
    graphics.DrawString(setprecision_float(vtargetSliceSize.y, 1).c_str(), -1, &font, PointF(100, 130), &BlackBrush);

    graphics.DrawString(L"재생 시간", -1, &font, PointF(10, 160), &GrayBrush);
    graphics.DrawString(setprecision_float(m_fDuration, 1).c_str(), -1, &font, PointF(100, 160), &BlackBrush);

    list<CWorkshopBtn*>::iterator iter = m_lBtn.begin();
    for (; iter != m_lBtn.end(); iter++) (*iter)->render(_dc);

    BitBlt(m_WorkshopMainDC, 80, 190, 210, 220, _dc, 0, 0, SRCCOPY);
}

void ControlFrameInfo(DWORD_PTR _param1, DWORD_PTR _param2, DWORD_PTR _param3)
{
    int param2 = (int)_param2;
    int param3 = (int)_param3;

    float _f = 0.f;
    if (param3 == 1) _f = 0.1f;
    else _f = -0.1f;

    if (param2 == 5)
    {
        CWorkshopWindow::GetInst()->AddDuration(_f);
        return;
    }

    CMarquee* pMarquee = (CMarquee*)_param1;
    Vec2 vPos = pMarquee->GetPos();
    Vec2 vScale = pMarquee->GetScale();

    if (param2 == 1)
    {
        pMarquee->SetPos(Vec2(vPos.x + _f, vPos.y));
    }
    else if (param2 == 2)
    {
        pMarquee->SetPos(Vec2(vPos.x, vPos.y - _f));
    }
    else if (param2 == 3)
    {
        pMarquee->SetScale(Vec2(vScale.x + _f, vScale.y));
    }
    else if (param2 == 4)
    {
        pMarquee->SetScale(Vec2(vScale.x, vScale.y + _f));
    }
}


// =================================================================
// 윈도우 프로시저 함수
// =================================================================
#include "CWorkshopWindow.h"
#include "CSceneMgr.h"
#include "CScene.h";
#include "CScene_Ani_Workshop.h"

LRESULT CALLBACK WorkshopWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int selectedItem = -1;
    HWND WorkshophWnd = CWorkshopWindow::GetInst()->GetWorkshophWnd();
    HWND ListhWnd = CWorkshopWindow::GetInst()->GetListhWnd();
    switch (message)
    {
    case WM_LBUTTONDOWN:
    {
        CScene_Ani_Workshop* pWorkshop = (CScene_Ani_Workshop*)CSceneMgr::GetInst()->GetCurScene();
        pWorkshop->SetDefaultState();
    }
    break;
    case WM_KEYDOWN:
        switch (wParam)
        {
            case VK_ESCAPE:
                ShowWindow(WorkshophWnd, SW_HIDE);
                break;
            default:
                break;
        }
        break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case IDM_EXIT:
            ShowWindow(WorkshophWnd, SW_HIDE);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;

    case WM_NOTIFY:
    {
        LPNMHDR pnmh = (LPNMHDR)lParam;
        NMHDR* nmhdr = reinterpret_cast<NMHDR*>(lParam);
        CScene_Ani_Workshop* pWorkshop = (CScene_Ani_Workshop*) CSceneMgr::GetInst()->GetCurScene();
        int itemCount = ListView_GetItemCount(CWorkshopWindow::GetInst()->GetListhWnd());
        if (pnmh->code == NM_CLICK)
        {
            LPNMITEMACTIVATE lpnmia = (LPNMITEMACTIVATE)lParam;
            CWorkshopWindow::GetInst()->SetTargetFrm(lpnmia->iItem);
            
            // CScene_Ani_Workshop의 타겟 Marquee 변경
            pWorkshop->SetTargetMarquee(lpnmia->iItem);

            for (int i = 0; i < itemCount; i++)
            {
                UINT state = (i == lpnmia->iItem) ? LVIS_SELECTED : LVIF_STATE;
                ListView_SetItemState(lpnmia->hdr.hwndFrom, i, state, LVIS_SELECTED);
            }
        }
        else if (nmhdr->code == LVN_ITEMCHANGED) {
            NMLISTVIEW* pnmv = reinterpret_cast<NMLISTVIEW*>(lParam);
            if ((pnmv->uNewState & LVIS_SELECTED) && !(pnmv->uOldState & LVIS_SELECTED)) {
                // 선택된 아이템이 변경되었을 때의 처리
                int selectedItem = pnmv->iItem;

                for (int i = 0; i < itemCount; i++)
                {
                    if (i == selectedItem) continue;
                    UINT state = (i == pnmv->iItem) ? LVIS_SELECTED : LVIF_STATE;
                    ListView_SetItemState(pnmv->hdr.hwndFrom, i, state, LVIS_SELECTED);
                }
                ListView_SetItemState(pnmv->hdr.hwndFrom, selectedItem, LVIS_SELECTED, LVIS_SELECTED);
                CWorkshopWindow::GetInst()->SetTargetFrm(selectedItem);
            }
        }
    }
    break;
    case WM_CLOSE:
        ShowWindow(WorkshophWnd, SW_HIDE);
        break;
    case WM_DESTROY:
        ShowWindow(WorkshophWnd, SW_HIDE);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}