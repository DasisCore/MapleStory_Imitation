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
        L"Object Manager",                // Window text
        WS_OVERLAPPEDWINDOW & ~WS_SIZEBOX & ~WS_MAXIMIZEBOX,            // Window style
        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, 600, 778,
        NULL,       // Parent window    
        NULL,       // Menu
        hInst,      // Instance handle
        NULL        // Additional application data
    );

    // 오류 검출
    assert(subhWnd);

    // 최대화 및 최소화 버튼을 숨김
    SetWindowLong(subhWnd, GWL_STYLE, GetWindowLong(subhWnd, GWL_STYLE) & ~(WS_MAXIMIZEBOX | WS_MINIMIZEBOX));
    
    m_hWndTool = subhWnd;

    m_hWndObjectCombo = CraeteComboBox(L"combo box", Vec2(10, 30), Vec2(200, 200));
    
    m_hWndEdit = CreateEdit(L"Edit box", Vec2(10, 100), Vec2(100, 30));

    // 콤보 박스 아이템 추가



    //// 리스트 뷰 생성
    //// 리스트 뷰 초기화
    //InitCommonControls();
    //HWND hWndList = CreateWindowEx(0, WC_LISTVIEW, L"", WS_CHILD | WS_VISIBLE | LVS_REPORT,
    //    10, 10, 460, 200, subhWnd, NULL, hInst, NULL);

    //// ListView 열 추가
    //LVCOLUMN lvColumn;
    //lvColumn.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
    //lvColumn.pszText = (LPWSTR)L"Column 1";
    //lvColumn.cx = 150;
    //lvColumn.iSubItem = 0;
    //ListView_InsertColumn(hWndList, 0, &lvColumn);

    //lvColumn.pszText = (LPWSTR)L"Column 2";
    //lvColumn.cx = 150;
    //lvColumn.iSubItem = 1;
    //ListView_InsertColumn(hWndList, 1, &lvColumn);

    //// ListView 아이템 추가
    //for (int i = 0; i < 4; ++i) {
    //    wstring temp1 = L"Data ";
    //    temp1 += std::to_wstring(i);
    //    LVITEM lvItem;
    //    lvItem.mask = LVIF_TEXT;
    //    lvItem.iItem = i;
    //    lvItem.iSubItem = 0;
    //    lvItem.pszText = const_cast<LPWSTR>(temp1.c_str());
    //    ListView_InsertItem(hWndList, &lvItem);

    //    temp1 = L"Additional Data ";
    //    temp1 += std::to_wstring(i);
    //    lvItem.iSubItem = 1;
    //    lvItem.pszText = const_cast<LPWSTR>(temp1.c_str());
    //    ListView_SetItem(hWndList, &lvItem);
    //}

    //ShowWindow(hWndList, SW_SHOWNORMAL);


    shiftWindow();
}

void CToolWindow::showWindow()
{
    ShowWindow(m_hWndTool, 1);
    shiftWindow();
    UpdateWindow(m_hWndTool);
    SetFocus(CCore::GetInst()->GetMainHwnd());
}

HWND CToolWindow::CraeteComboBox(wstring _strName, Vec2 _vPos, Vec2 _vScale)
{
    HINSTANCE hInst = CCore::GetInst()->GetMainhInstance();

    HWND hComboBox = CreateWindow(
        L"COMBOBOX",        // 클래스 이름
        L"Combo Box Name",  // 콤보 박스의 이름
        CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
        _vPos.x, _vPos.y, _vScale.x, _vScale.y,
        m_hWndTool, NULL, hInst, NULL);


    // 레이블 생성
    HWND hLabel = CreateWindow(
        L"STATIC",              // 클래스 이름
        L"Object Type",    // 텍스트
        WS_CHILD | WS_VISIBLE,
        _vPos.x, _vPos.y - 20.f, _vScale.x, 20.f,
        m_hWndTool, NULL, hInst, NULL);

    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)L"Default");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)L"BackGround");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)L"Ground");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)L"Player");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)L"Monster");

    // 선택된 항목을 첫번째 인자로 설정
    SendMessage(hComboBox, CB_SETCURSEL, 0, 0);
    return hComboBox;
}

HWND CToolWindow::CreateEdit(wstring _strName, Vec2 _vPos, Vec2 _vScale)
{
    HINSTANCE hInst = CCore::GetInst()->GetMainhInstance();
    HWND hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_LEFT,
        _vPos.x, _vPos.y - 20.f, _vScale.x, 20.f, m_hWndTool, (HMENU)IDC_EDIT, hInst, NULL);

    return hEdit;
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

    // 변경된 스타일을 적용
    SetWindowPos(m_hWndTool, nullptr, mainWndX, mainWndY, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_FRAMECHANGED);
}

// =================================================================
// 윈도우 프로시저 함수
// =================================================================

#include "CToolWindow.h"
#include <sstream>
#include <iomanip>

static WCHAR lastValidInput[256] = L"";

LRESULT CALLBACK ToolWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HWND ToolhWnd = CToolWindow::GetInst()->GetToolhWnd();
    HWND hEdit = CToolWindow::GetInst()->GetEdit();

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
        
        case IDC_EDIT:
        case IDC_EDIT2:
            if (HIWORD(wParam) == EN_CHANGE)
            {
                TCHAR buffer[256];
                GetWindowText(hEdit, buffer, 256);

                // 입력된 값이 실수인지 확인
                std::wistringstream ss(buffer);
                float floatValue;
                if (ss >> std::setprecision(3) >> floatValue && ss.eof())
                {
                    // 유효한 실수 값이면 처리
                    // 여기에서 floatValue를 사용하여 필요한 작업 수행
                    // 소수점 둘째자리 초과 여부 확인
                    if (wcschr(buffer, L'.') != nullptr)
                    {
                        int decimalCount = wcslen(buffer) - (wcschr(buffer, L'.') - buffer) - 1;
                        if (decimalCount > 1)
                        {
                            MessageBox(hWnd, L"소수 첫번째 자리까지만 입력 가능합니다.", L"Error", MB_OK | MB_ICONERROR);
                            // 이전 유효한 값으로 복원
                            SetWindowText(hEdit, lastValidInput);
                            return 0;
                        }
                    }
                    // 유효한 실수 값이면 처리
                    wcscpy_s(lastValidInput, buffer);
                    // 여기에서 floatValue를 사용하여 필요한 작업 수행
                }
                else
                {
                    // 유효하지 않은 입력 처리
                    MessageBox(hWnd, L"숫자만 입력 가능합니다.", L"Error", MB_OK | MB_ICONERROR);
                    // 이전 유효한 값으로 복원
                    SetWindowText(hEdit, lastValidInput);
                }
            }
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
