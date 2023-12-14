#include "global.h"
#include "main.h"
#include "CToolWindow.h"
#include "CCore.h"

LRESULT CALLBACK ToolWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

CToolWindow::CToolWindow()
    : m_hWndTool(nullptr)
    , m_hDCTool(nullptr)
    , m_hWndName(nullptr)
    , m_hWndObjType(nullptr)
    , m_hWndDirection(nullptr)
    , m_hWndState(nullptr)
    , m_hWndPosX(nullptr)
    , m_hWndPosY(nullptr)
    , m_hWndScaleX(nullptr)
    , m_hWndScaleY(nullptr)
    , m_hWndColCheckBox(nullptr)
    , m_hWndColOffsetX(nullptr)
    , m_hWndColOffsetY(nullptr)
    , m_hWndColScaleX(nullptr)
    , m_hWndColScaleY(nullptr)
    , m_hWndAniCheckBox(nullptr)
    , m_hWndAniLoadBtn(nullptr)
    , m_hWndAniListView(nullptr)
    , m_hWndGravityCheckBox(nullptr)
    , m_hWndRigidBodyCheckBox(nullptr)
    , m_hWndCreateObjBtn(nullptr)
    , m_hWndObjectCombo(nullptr)
    , m_hWndEdit(nullptr)
    , m_hDCBack(nullptr)
{
}

CToolWindow::~CToolWindow()
{
    DeleteDC(m_hDCBack);
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
        CW_USEDEFAULT, CW_USEDEFAULT, 440, 778,
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
    m_hDCTool = GetDC(m_hWndTool);

    //m_hWndObjectCombo = CraeteComboBox(L"combo box", Vec2(10, 30), Vec2(200, 200));

    m_hWndName = CreateEdit(L"Name", Vec2(10, 70), Vec2(100, 30), (HMENU)IDC_EDIT_NAME);

    m_hWndObjType = CraeteComboBox(L"Object Type", Vec2(10, 120), Vec2(100, 200), (HMENU)IDC_EDIT_OBJTYPE);
    m_hWndDirection = CraeteComboBox(L"Direction", Vec2(120, 120), Vec2(100, 200), (HMENU)IDC_EDIT_DIRECTION);
    m_hWndState = CraeteComboBox(L"State", Vec2(230, 120), Vec2(100, 200), (HMENU)IDC_EDIT_STATE);

    m_hWndPosX = CreateEdit(L"Position X", Vec2(10, 170), Vec2(100, 30), (HMENU)IDC_EDIT_POS_X);
    m_hWndPosY = CreateEdit(L"Position Y", Vec2(120, 170), Vec2(100, 30), (HMENU)IDC_EDIT_POS_Y);

    m_hWndScaleX = CreateEdit(L"Scale Width", Vec2(10, 220), Vec2(100, 30), (HMENU)IDC_EDIT_SCALE_X);
    m_hWndScaleY = CreateEdit(L"Scale Height", Vec2(120, 220), Vec2(100, 30), (HMENU)IDC_EDIT_SCALE_Y);

    m_hWndColCheckBox = CreateCheckBox(L"Collider", Vec2(10, 280), Vec2(10, 10), (HMENU)IDC_CHECKBOX_COLLIDER);
    // �⺻���´� ��Ȱ��ȭ ����
    m_hWndColOffsetX = CreateEdit(L"Offset X", Vec2(10, 310), Vec2(100, 30), (HMENU)IDC_EDIT_COLLIDER_OFFSET_X);
    EnableWindow(m_hWndColOffsetX, 0);
    m_hWndColOffsetY = CreateEdit(L"Offset Y", Vec2(120, 310), Vec2(100, 30), (HMENU)IDC_EDIT_COLLIDER_OFFSET_Y);
    EnableWindow(m_hWndColOffsetY, 0);
    m_hWndColScaleX = CreateEdit(L"Offset Scale X", Vec2(10, 360), Vec2(100, 30), (HMENU)IDC_EDIT_COLLIDER_SCALE_X);
    EnableWindow(m_hWndColScaleX, 0);
    m_hWndColScaleY = CreateEdit(L"Offset Scale Y", Vec2(120, 360), Vec2(100, 30), (HMENU)IDC_EDIT_COLLIDER_SCALE_Y);
    EnableWindow(m_hWndColScaleY, 0);

    m_hWndAniCheckBox = CreateCheckBox(L"Animation", Vec2(10, 420), Vec2(10, 10), (HMENU)IDC_CHECKBOX_ANIMATION);
    m_hWndAniLoadBtn = CreateButton(L"Load", Vec2(360, 420), Vec2(50.f, 20.f), (HMENU)IDC_BUTTON_ANIMATION);
    EnableWindow(m_hWndAniLoadBtn, 0);
    m_hWndAniListView = CreateListView(L"Animation List view", Vec2(10, 430), Vec2(400, 200), (HMENU)IDC_LISTVIEW_ANIMATION);
    EnableWindow(m_hWndAniListView, 0);

    m_hWndGravityCheckBox = CreateCheckBox(L"Gravity", Vec2(10, 660), Vec2(10, 10), (HMENU)IDC_CHECKBOX_GRAVITY);
    m_hWndRigidBodyCheckBox = CreateCheckBox(L"RigidBody", Vec2(10, 690), Vec2(10, 10), (HMENU)IDC_CHECKBOX_RIGIDBODY);

    m_hWndCreateObjBtn = CreateButton(L"Create", Vec2(340, 710), Vec2(70.f, 30.f), (HMENU)IDC_BUTTON_OBJECT);


    HDC MainDC = GetDC(m_hWndTool);
    //Graphics MainG(MainDC);

    m_hDCBack = CreateCompatibleDC(MainDC);
    HBITMAP hBitmap = CreateCompatibleBitmap(MainDC, 100, 30);
    SelectObject(m_hDCBack, hBitmap);
    DeleteObject(hBitmap);

    Graphics BackG(m_hDCBack);
    BackG.Clear(Color(240, 240, 240));
    Font font(L"Noto Sans", 14, FontStyleBold);
    SolidBrush brush(Color(255, 0, 0, 0));
    BackG.DrawString(L"Option", -1, &font, PointF(0, 0), &brush);


    //shiftWindow();
}

void CToolWindow::showWindow()
{
    ShowWindow(m_hWndTool, SW_SHOWNOACTIVATE);
    shiftWindow();
    UpdateWindow(m_hWndTool);
    SetFocus(CCore::GetInst()->GetMainHwnd());
}

void CToolWindow::update()
{
}

void CToolWindow::render()
{
    BitBlt(m_hDCTool, 10, 10, 100, 30, m_hDCBack, 0, 0, SRCCOPY);

    //HDC dc = GetDC(m_hWndTool);

    //Graphics graphics(dc);
    //Font font(L"Noto Sans", 14, FontStyleBold);
    //SolidBrush brush(Color(255, 0, 0, 0));
    //graphics.DrawString(L"Option", -1, &font, PointF(10, 10), &brush);
}

HWND CToolWindow::CraeteComboBox(wstring _strName, Vec2 _vPos, Vec2 _vScale, HMENU _idc)
{
    HINSTANCE hInst = CCore::GetInst()->GetMainhInstance();

    // ���̺� ����
    HWND hLabel = CreateWindow(
        L"STATIC",              // Ŭ���� �̸�
        L"Object Type",    // �ؽ�Ʈ
        WS_CHILD | WS_VISIBLE,
        _vPos.x, _vPos.y - 20.f, _vScale.x, 20.f,
        m_hWndTool, NULL, hInst, NULL);

    HWND hComboBox = CreateWindow(
        L"COMBOBOX",        // Ŭ���� �̸�
        L"Combo Box Name",  // �޺� �ڽ��� �̸�
        CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
        _vPos.x, _vPos.y, _vScale.x, _vScale.y,
        m_hWndTool, (HMENU)_idc, hInst, NULL);

    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)L"Default");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)L"BackGround");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)L"Ground");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)L"Monster");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)L"Player");

    // ���õ� �׸��� ù��° ���ڷ� ����
    SendMessage(hComboBox, CB_SETCURSEL, 4, 4);
    return hComboBox;
}

HWND CToolWindow::CreateEdit(wstring _strName, Vec2 _vPos, Vec2 _vScale, HMENU _idc)
{
    HINSTANCE hInst = CCore::GetInst()->GetMainhInstance();
    
    // ���̺� ����
    HWND hLabel = CreateWindow(
        L"STATIC",              // Ŭ���� �̸�
        _strName.c_str(),        // �ؽ�Ʈ
        WS_CHILD | WS_VISIBLE,
        _vPos.x, _vPos.y - 20.f, _vScale.x, 20.f,
        m_hWndTool, NULL, hInst, NULL);

    HWND hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_LEFT,
        _vPos.x, _vPos.y, _vScale.x, 20.f, m_hWndTool, (HMENU)_idc, hInst, NULL);

    return hEdit;
}

HWND CToolWindow::CreateCheckBox(wstring _strName, Vec2 _vPos, Vec2 _vScale, HMENU _idc)
{
    HINSTANCE hInst = CCore::GetInst()->GetMainhInstance();

    // ���̺� ����
    HWND hLabel = CreateWindow(
        L"STATIC",              // Ŭ���� �̸�
        _strName.c_str(),        // �ؽ�Ʈ
        WS_CHILD | WS_VISIBLE,
        _vPos.x, _vPos.y - 20.f, 70.f, 20.f,
        m_hWndTool, (HMENU)_idc, hInst, NULL);

    HWND hWndCheckBox = CreateWindow(L"BUTTON", _strName.c_str(), WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
        _vPos.x + 70.f, _vPos.y - 20.f, 20.f, 20.f, m_hWndTool, (HMENU)_idc, hInst, NULL);

    return hWndCheckBox;
}

HWND CToolWindow::CreateListView(wstring _strName, Vec2 _vPos, Vec2 _vScale, HMENU _idc)
{
    HINSTANCE hInst = CCore::GetInst()->GetMainhInstance();

    INITCOMMONCONTROLSEX icex = { sizeof(INITCOMMONCONTROLSEX), ICC_LISTVIEW_CLASSES };
    InitCommonControlsEx(&icex);
    //InitCommonControls();
    HWND hWndList = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTVIEW, L"", WS_VISIBLE | WS_CHILD | LVS_REPORT | LVS_NOCOLUMNHEADER | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT,
        _vPos.x, _vPos.y, _vScale.x, _vScale.y, m_hWndTool, _idc, hInst, NULL);

    //SendMessage(hWndList, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT);
    SendMessage(hWndList, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_NOSCROLL);
  
    // ListView �� �߰�
    LVCOLUMN lvColumn = { 0 };
    lvColumn.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
    lvColumn.pszText = const_cast<LPWSTR>(L"Column 1");
    lvColumn.cx = _vScale.x - 5.f;
    lvColumn.iSubItem = 0;
    ListView_InsertColumn(hWndList, 0, &lvColumn);

    // ����Ʈ �信 ������ �߰�
    LVITEM lvItem = { 0 };
    lvItem.mask = LVIF_TEXT;
    lvItem.iItem = 0; // ������ �ε���
    lvItem.iSubItem = 0; // ��������� �ε���
    lvItem.pszText = const_cast<LPWSTR>(L"Item 2"); // ������ �ؽ�Ʈ
    ListView_InsertItem(hWndList, &lvItem);

    return hWndList;
}

HWND CToolWindow::CreateButton(wstring _strName, Vec2 _vPos, Vec2 _vScale, HMENU _idc)
{
    HINSTANCE hInst = CCore::GetInst()->GetMainhInstance();

    HWND hWndBtn = CreateWindow(L"BUTTON", _strName.c_str(), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        _vPos.x, _vPos.y - 20.f, _vScale.x, _vScale.y, m_hWndTool, (HMENU)_idc, hInst, NULL);

    return hWndBtn;
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
#include <sstream>
#include <iomanip>


static WCHAR lastValidInput[256] = L"";

void CheckPrecision(HWND hWnd, WPARAM wParam, LPARAM lParam, HWND _edit);

LRESULT CALLBACK ToolWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    CToolWindow* pToolWindow = CToolWindow::GetInst();

    switch (message)
    {
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_ESCAPE:
            ShowWindow(pToolWindow->GetToolhWnd(), SW_HIDE);
            break;
        }
    break;
    case WM_ERASEBKGND:
    {
        HDC hdc = (HDC)wParam;

        RECT rect;
        GetClientRect(hWnd, &rect);
        FillRect(hdc, &rect, CreateSolidBrush(RGB(240, 240, 240)));
        return 1;
    }
    break;
    case WM_COMMAND:
    {
        if (HIWORD(wParam) == BN_CLICKED)
        {
            if ((HWND)lParam == pToolWindow->GethWndColCheckBox())
            {
                int isChecked = SendMessage(pToolWindow->GethWndColCheckBox(), BM_GETCHECK, 0, 0);
                EnableWindow(pToolWindow->GethWndColScaleX(), isChecked);
                EnableWindow(pToolWindow->GethWndColScaleY(), isChecked);
                EnableWindow(pToolWindow->GethWndColOffsetX(), isChecked);
                EnableWindow(pToolWindow->GethWndColOffsetY(), isChecked);
            }

            if ((HWND)lParam == pToolWindow->GethWndAniCheckBox())
            {
                int isChecked = SendMessage(pToolWindow->GethWndAniCheckBox(), BM_GETCHECK, 0, 0);
                EnableWindow(pToolWindow->GethWndAniListView(), isChecked);
                EnableWindow(pToolWindow->GethWndAniLoadBtn(), isChecked);
            }
        }

        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case IDM_EXIT:
            ShowWindow(pToolWindow->GetToolhWnd(), SW_HIDE);
            break;
        
        // �Ҽ��� ù°�ڸ� or �Ǽ��� �ޱ�.
        HWND hEdit;
        case IDC_EDIT_POS_X:
            hEdit = pToolWindow->GethWndPosX();
            CheckPrecision(hWnd, wParam, lParam, hEdit);
            break;
        case IDC_EDIT_POS_Y:
            hEdit = pToolWindow->GethWndPosY();
            CheckPrecision(hWnd, wParam, lParam, hEdit);
            break;
        case IDC_EDIT_SCALE_X:
            hEdit = pToolWindow->GethWndScaleX();
            CheckPrecision(hWnd, wParam, lParam, hEdit);
            break;
        case IDC_EDIT_SCALE_Y:
            hEdit = pToolWindow->GethWndScaleY();
            CheckPrecision(hWnd, wParam, lParam, hEdit);
            break;
        case IDC_EDIT_COLLIDER_OFFSET_X:
            hEdit = pToolWindow->GethWndColOffsetX();
            CheckPrecision(hWnd, wParam, lParam, hEdit);
            break;
        case IDC_EDIT_COLLIDER_OFFSET_Y:
            hEdit = pToolWindow->GethWndColOffsetY();
            CheckPrecision(hWnd, wParam, lParam, hEdit);
            break;
        case IDC_EDIT_COLLIDER_SCALE_X:
            hEdit = pToolWindow->GethWndColScaleX();
            CheckPrecision(hWnd, wParam, lParam, hEdit);
            break;
        case IDC_EDIT_COLLIDER_SCALE_Y:
            hEdit = pToolWindow->GethWndColScaleY();
            CheckPrecision(hWnd, wParam, lParam, hEdit);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_CLOSE:
        ShowWindow(pToolWindow->GetToolhWnd(), SW_HIDE);
        break;
    case WM_DESTROY:
        ShowWindow(pToolWindow->GetToolhWnd(), SW_HIDE);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


void CheckPrecision(HWND hWnd, WPARAM wParam, LPARAM lParam, HWND _edit)
{
    if (HIWORD(wParam) == EN_CHANGE)
    {
        TCHAR buffer[256];
        GetWindowText(_edit, buffer, 256);

        // ���۰� ����ٸ� Ȯ������ ����.
        if (wcslen(buffer) == 0) return;

        // �Էµ� ���� �Ǽ����� Ȯ��
        std::wistringstream ss(buffer);
        float floatValue;
        if (ss >> std::setprecision(3) >> floatValue && ss.eof())
        {
            // ��ȿ�� �Ǽ� ���̸� ó��
            // ���⿡�� floatValue�� ����Ͽ� �ʿ��� �۾� ����
            // �Ҽ��� ��°�ڸ� �ʰ� ���� Ȯ��
            if (wcschr(buffer, L'.') != nullptr)
            {
                int decimalCount = wcslen(buffer) - (wcschr(buffer, L'.') - buffer) - 1;
                if (decimalCount > 1)
                {
                    // ���� ��ȿ�� ������ ����
                    MessageBox(hWnd, L"�Ҽ� ù��° �ڸ������� �Է� �����մϴ�.", L"Error", MB_OK | MB_ICONERROR);

                    SetWindowText(_edit, lastValidInput);
                    return;
                }
            }
            // ��ȿ�� �Ǽ� ���̸� ó��
            wcscpy_s(lastValidInput, buffer);
            // ���⿡�� floatValue�� ����Ͽ� �ʿ��� �۾� ����
        }
        else
        {
            // ��ȿ���� ���� �Է� ó��
            MessageBox(hWnd, L"���ڸ� �Է� �����մϴ�.", L"Error", MB_OK | MB_ICONERROR);
            // ���� ��ȿ�� ������ ����
            SetWindowText(_edit, lastValidInput);
        }
    }
}