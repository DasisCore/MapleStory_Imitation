#include "global.h"
#include "main.h"
#include "CWorkshopWindow.h"
#include "CCore.h"

LRESULT CALLBACK WorkshopWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

CWorkshopWindow::CWorkshopWindow()
    : m_hWndWorkshop(nullptr)
    , m_WorkshopMainDC(nullptr)
    , m_hWndList(nullptr)
    , m_iTargetFrame(0)
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

    // 오류 검출
    assert(subhWnd);

    // 최대화 및 최소화 버튼을 숨김
    SetWindowLong(subhWnd, GWL_STYLE, GetWindowLong(subhWnd, GWL_STYLE) & ~(WS_MAXIMIZEBOX | WS_MINIMIZEBOX));

    m_hWndWorkshop = subhWnd;

    // 리스트 뷰 생성
    // 리스트 뷰 초기화
    InitCommonControls();
    m_hWndList = CreateWindowEx(0, WC_LISTVIEW, L"List view", WS_CHILD | WS_VISIBLE | LVS_REPORT,
        10, 200, 260, 200, subhWnd, NULL, hInst, NULL);

    // ListView 열 추가
    LVCOLUMN lvColumn;
    lvColumn.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
    lvColumn.pszText = (LPWSTR)L"프레임";
    lvColumn.cx = 260;
    lvColumn.iSubItem = 0;
    ListView_InsertColumn(m_hWndList, 0, &lvColumn);

    ShowWindow(m_hWndList, SW_SHOWNORMAL);

    shiftWindow();

    // 더블 버퍼링 준비
    m_WorkshopMainDC = GetDC(m_hWndWorkshop);
    m_memDC = CreateCompatibleDC(m_WorkshopMainDC);
    HBITMAP memBitmap = CreateCompatibleBitmap(m_WorkshopMainDC, 300, 190);

    SelectObject(m_memDC, memBitmap);
    DeleteObject(memBitmap);

    Graphics graphics(m_memDC);

    // 구분을 위해 일단 연노랑으로 채워놓음
    graphics.Clear(Color(255, 255, 255, 128));

}

void CWorkshopWindow::update()
{


}

void CWorkshopWindow::render(HDC _dc)
{
    BitBlt(m_WorkshopMainDC, 0, 0, 300, 190, m_memDC, 0, 0, SRCCOPY);
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

// =================================================================
// 윈도우 프로시저 함수
// =================================================================
#include "CWorkshopWindow.h"

LRESULT CALLBACK WorkshopWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HWND ToolhWnd = CWorkshopWindow::GetInst()->GetWorkshophWnd();

    switch (message)
    {
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_ESCAPE:
            ShowWindow(ToolhWnd, SW_HIDE);
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
            ShowWindow(ToolhWnd, SW_HIDE);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;

    case WM_NOTIFY:
    {
        LPNMHDR pnmh = (LPNMHDR)lParam;
        if (pnmh->code == NM_CLICK)
        {
            LPNMITEMACTIVATE lpnmia = (LPNMITEMACTIVATE)lParam;
            CWorkshopWindow::GetInst()->SetTargetFrm(lpnmia->iItem);

            int itemCount = ListView_GetItemCount(CWorkshopWindow::GetInst()->GetListhWnd());
            for (int i = 0; i < itemCount; i++)
            {
                UINT state = (i == lpnmia->iItem) ? LVIS_DROPHILITED : 0;
                ListView_SetItemState(lpnmia->hdr.hwndFrom, i, state, LVIS_DROPHILITED);
            }
        }
    }
    break;
    case WM_CLOSE:
        ShowWindow(ToolhWnd, SW_HIDE);
        break;
    case WM_DESTROY:
        ShowWindow(ToolhWnd, SW_HIDE);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}