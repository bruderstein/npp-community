// This file is part of notepad++
// Copyright (C)2009 The Notepad++ Team
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#include "precompiled_headers.h"
#include "ValueDlg.h"
#include "resource.h"

void ValueDlg::init( HINSTANCE hInst, HWND parent, int valueToSet, const TCHAR *text )
{
	Window::init(hInst, parent);
	_defaultValue = valueToSet;
	_name = text;
}

INT_PTR ValueDlg::doDialog( POINT p, bool isRTL /*= false*/ )
{
	_p = p;
	if (isRTL)
	{
		DLGTEMPLATE *pMyDlgTemplate = NULL;
		HGLOBAL hMyDlgTemplate = makeRTLResource(IDD_VALUE_DLG, &pMyDlgTemplate);
		INT_PTR result = ::DialogBoxIndirectParam(_hInst, pMyDlgTemplate, _hParent,  (DLGPROC)dlgProc, (LPARAM)this);
		::GlobalFree(hMyDlgTemplate);
		return (int)result;
	}
	return ::DialogBoxParam(_hInst, MAKEINTRESOURCE(IDD_VALUE_DLG), _hParent,  (DLGPROC)dlgProc, (LPARAM)this);
}

void ValueDlg::setNBNumber( int nbNumber )
{
	if (nbNumber > 0)
		_nbNumber = nbNumber;
}

int ValueDlg::reSizeValueBox()
{
	if (_nbNumber == DEFAULT_NB_NUMBER) return 0;
	RECT rect;
	POINT p;

	HWND hEdit = ::GetDlgItem(_hSelf, IDC_VALUE_EDIT);

	//get screen coordonnees (x,y)
	::GetWindowRect(hEdit, &rect);
	int w = rect.right - rect.left;
	int h = rect.bottom - rect.top;

	p.x = rect.left;
	p.y = rect.top;

	// convert screen coordonnees to client coordonnees
	::ScreenToClient(_hSelf, &p);

	int unit = w / (DEFAULT_NB_NUMBER + 2);
	int extraSize = (_nbNumber-DEFAULT_NB_NUMBER)*unit;
	::MoveWindow(hEdit, p.x, p.y, w + extraSize, h, FALSE);

	return extraSize;
}

BOOL CALLBACK ValueDlg::run_dlgProc( UINT Message, WPARAM wParam, LPARAM /*lParam*/ )
{
	switch (Message)
	{
	case WM_INITDIALOG :
		{
			::SetDlgItemText(_hSelf, IDC_VALUE_STATIC, _name.c_str());
			::SetDlgItemInt(_hSelf, IDC_VALUE_EDIT, _defaultValue, FALSE);

			RECT rc;
			::GetClientRect(_hSelf, &rc);
			int size = reSizeValueBox();
			::MoveWindow(_hSelf, _p.x, _p.y, rc.right - rc.left + size, rc.bottom - rc.top + 30, TRUE);

			return TRUE;
		}

	case WM_COMMAND :
		{
			switch (wParam)
			{
			case IDOK :
				{
					int i = ::GetDlgItemInt(_hSelf, IDC_VALUE_EDIT, NULL, FALSE);
					::EndDialog(_hSelf, i);
					return TRUE;
				}

			case IDCANCEL :
				::EndDialog(_hSelf, -1);
				return TRUE;

			default:
				return FALSE;
			}
		}
	default :
		return FALSE;
	}
}
