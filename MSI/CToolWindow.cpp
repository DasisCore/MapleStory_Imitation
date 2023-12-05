#include "global.h"
#include "main.h"
#include "CToolWindow.h"
#include "CCore.h"

LRESULT CALLBACK ToolWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

CToolWindow::CToolWindow()
    : m_hWndTool(nullptr)
{
}

CToolWindow::~CToolWindow()
{
    if(m_hWndTool) DestroyWindow(m_hWndTool);
}

void CToolWindow::init()
{
    HINSTANCE hInst = CCore::GetInst()->GetMainhInstance();

    const wchar_t CLASS_NAME[] = L"Tool Window";

    WNDCLASSEXW wcex_tool;

    wcex_tool.cbSize = sizeof(WNDCLASSEX);

    wcex_tool.style = CS_HREDRAW | CS_VREDRAW;
    wcex_tool.lpfnWndProc = ToolWndProc;
    wcex_tool.cbClsExtra = 0;
    wcex_tool.cbWndExtra = 0;
    wcex_tool.hInstance = hInst;
    wcex_tool.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_MSI));
    wcex_tool.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex_tool.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex_tool.lpszMenuName = nullptr; // MAKEINTRESOURCEW(IDC_MSI);
    wcex_tool.lpszClassName = CLASS_NAME;
    wcex_tool.hIconSm = LoadIcon(wcex_tool.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    RegisterClassExW(&wcex_tool);

    HWND subhWnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"Tool Manager",                // Window text
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
    
    m_hWndTool = subhWnd;

    // ����Ʈ �� ����
        // ����Ʈ �� �ʱ�ȭ
    InitCommonControls();
    HWND hWndList = CreateWindowEx(0, WC_LISTVIEW, L"", WS_CHILD | WS_VISIBLE | LVS_REPORT,
        10, 10, 460, 200, subhWnd, NULL, hInst, NULL);

    // ListView �� �߰�
    LVCOLUMN lvColumn;
    lvColumn.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
    lvColumn.pszText = (LPWSTR)L"Column 1";
    lvColumn.cx = 150;
    lvColumn.iSubItem = 0;
    ListView_InsertColumn(hWndList, 0, &lvColumn);

    lvColumn.pszText = (LPWSTR)L"Column 2";
    lvColumn.cx = 150;
    lvColumn.iSubItem = 1;
    ListView_InsertColumn(hWndList, 1, &lvColumn);

    // ListView ������ �߰�
    for (int i = 0; i < 4; ++i) {
        wstring temp1 = L"Data ";
        temp1 += std::to_wstring(i);
        LVITEM lvItem;
        lvItem.mask = LVIF_TEXT;
        lvItem.iItem = i;
        lvItem.iSubItem = 0;
        lvItem.pszText = const_cast<LPWSTR>(temp1.c_str());
        ListView_InsertItem(hWndList, &lvItem);

        temp1 = L"Additional Data ";
        temp1 += std::to_wstring(i);
        lvItem.iSubItem = 1;
        lvItem.pszText = const_cast<LPWSTR>(temp1.c_str());
        ListView_SetItem(hWndList, &lvItem);
    }

    ShowWindow(hWndList, SW_SHOWNORMAL);


    shiftWindow();
}

void CToolWindow::showWindow()
{
    ShowWindow(m_hWndTool, 1);
    shiftWindow();
    UpdateWindow(m_hWndTool);
    SetFocus(CCore::GetInst()->GetMainHwnd());
}

void CToolWindow::Destroy_window()
{
    if(m_hWndTool) DestroyWindow(m_hWndTool);
}

void CToolWindow::shiftWindow()
{
    HWND MainhWnd = CCore::GetInst()->GetMainHwnd();

    RECT mainWndRect;
    GetWindowRect(MainhWnd, &mainWndRect);

    int mainWndX = mainWndRect.right - 10;
    int mainWndY = mainWndRect.top;

    // ����� ��Ÿ���� ����
    SetWindowPos(m_hWndTool, nullptr, mainWndX, mainWndY, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_FRAMECHANGED);
}

// =================================================================
// ������ ���ν��� �Լ�
// =================================================================

#include "CToolWindow.h"

LRESULT CALLBACK ToolWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HWND ToolhWnd = CToolWindow::GetInst()->GetToolhWnd();

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