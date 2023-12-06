#include "global.h"
#include "main.h"
#include "CWorkshopWindow.h"
#include "CCore.h"
#include "CSprite.h"

LRESULT CALLBACK WorkshopWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

CWorkshopWindow::CWorkshopWindow()
    : m_hWndWorkshop(nullptr)
    , m_WorkshopMainDC(nullptr)
    , m_hWndList(nullptr)
    , m_memDC(nullptr)
    , m_iTargetFrame(-1)
{
}

CWorkshopWindow::~CWorkshopWindow()
{
    DeleteDC(m_memDC);
    DeleteDC(m_WorkshopMainDC);

    if (m_hWndWorkshop) DestroyWindow(m_hWndWorkshop);
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

    // ���� ����
    assert(subhWnd);

    // �ִ�ȭ �� �ּ�ȭ ��ư�� ����
    SetWindowLong(subhWnd, GWL_STYLE, GetWindowLong(subhWnd, GWL_STYLE) & ~(WS_MAXIMIZEBOX | WS_MINIMIZEBOX));

    m_hWndWorkshop = subhWnd;

    // ����Ʈ �� ����
    // ����Ʈ �� �ʱ�ȭ
    InitCommonControls();
    m_hWndList = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTVIEW, L"List view", WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT | LVS_SINGLESEL | LVS_EX_DOUBLEBUFFER,
        10, 200, 70, 200, subhWnd, NULL, hInst, NULL);

    // ListView �� �߰�
    LVCOLUMN lvColumn;
    lvColumn.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
    lvColumn.pszText = (LPWSTR)L"������";
    lvColumn.cx = 65;
    lvColumn.iSubItem = 0;
    ListView_InsertColumn(m_hWndList, 0, &lvColumn);

    ShowWindow(m_hWndList, SW_SHOWNORMAL);

    shiftWindow();

    // ���� ���۸� �غ�
    m_WorkshopMainDC = GetDC(m_hWndWorkshop);
    m_memDC = CreateCompatibleDC(m_WorkshopMainDC);
    HBITMAP memBitmap = CreateCompatibleBitmap(m_WorkshopMainDC, 300, 190);

    SelectObject(m_memDC, memBitmap);
    DeleteObject(memBitmap);

    Graphics graphics(m_memDC);

}

void CWorkshopWindow::update()
{


}

void CWorkshopWindow::render(HDC _dc)
{
    Graphics graphics(m_memDC);

    // ������ ���� �ϴ� ��������� ä������
    graphics.Clear(Color(255, 255, 255, 128));

    Font font(L"Arial", 8);
    SolidBrush brush(Gdiplus::Color(255, 0, 0, 0)); // ������ �귯��

    wstring temp = L"���� ������ : " + std::to_wstring(m_iTargetFrame + 1);
    graphics.DrawString(temp.c_str(), -1, &font, PointF(5.f, 5.f), &brush);

    if (m_iTargetFrame != -1)
    {
        CScene_Ani_Workshop* pAniWorkshop = (CScene_Ani_Workshop*)CSceneMgr::GetInst()->GetCurScene();
        
        // ��������Ʈ �̹��� ��������
        CSprite* pSprite = pAniWorkshop->GetSprite();
        Image* pImage = pSprite->GetSprite();

        tFrame tFrm = pAniWorkshop->GetFrameInfo(m_iTargetFrame);

        Vec2 vLT = tFrm.vLT;
        Vec2 vScale = tFrm.vSliceSize;
        
        // 300, 190 ��Ʈ���� �߾ӿ� �׷��� ��.
        Vec2 vDrawLT = Vec2(284.f / 2.f, 190.f / 2.f);



        // ���μ��� ����
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

        // �׸� ��ġ �� ũ�� ����
        Rect destRect(vDrawLT.x, vDrawLT.y, vConvert.x, vConvert.y);

        //graphics.DrawImage(pImage, destRect, srcRect.X, srcRect.Y, srcRect.Width, srcRect.Height, UnitPixel);
        graphics.DrawImage(pImage, destRect, vLT.x, vLT.y, vScale.x, vScale.y, UnitPixel);
    }

    BitBlt(m_WorkshopMainDC, 0, 0, 300, 190, m_memDC, 0, 0, SRCCOPY);
}

void CWorkshopWindow::AddFrame(UINT _idx)
{
    // ListView ������ �߰�
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

    // ����Ʈ �並 �����Ͽ� ������ �������� �ݿ�
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

    // ����� ��Ÿ���� ����
    SetWindowPos(m_hWndWorkshop, nullptr, mainWndX, mainWndY, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_FRAMECHANGED);
}

// =================================================================
// ������ ���ν��� �Լ�
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
            
            // CScene_Ani_Workshop�� Ÿ�� Marquee ����
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
                // ���õ� �������� ����Ǿ��� ���� ó��
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