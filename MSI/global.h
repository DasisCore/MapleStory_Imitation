#pragma once

// ����Ƽ ���带 ����ϱ� ���� pch�� ������� �ʰ�,
// global.h�� ����� ����� ��.

#include <Windows.h>
#include <iostream>

// ����Ʈ �並 ���� ���
#include <CommCtrl.h>
// + �߰� ���Ӽ��� comctl32.lib


#include <math.h>
#include <assert.h>

#include "define.h"
#include "struct.h"

// GDI+ (�̹��� ó��)
#include <gdiplus.h>

using namespace Gdiplus;

#pragma comment (lib, "Gdiplus.lib")

// for TransparentBlt
#pragma comment(lib, "Msimg32.lib")

#include <list>
using std::list;


#include <vector>
using std::vector;

#include <string>
using std::string;
using std::wstring;

#include <map>
using std::map;
using std::make_pair;

#include "func.h"
