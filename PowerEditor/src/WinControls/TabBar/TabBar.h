//this file is part of notepad++
//Copyright (C)2003 Don HO ( donho@altern.org )
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#ifndef TAB_BAR_H
#define TAB_BAR_H

#ifndef WINDOW_CONTROL_H
#include "Window.h"
#endif

//Notification message
#define TCN_TABDROPPED (TCN_FIRST - 10)
#define TCN_TABDROPPEDOUTSIDE (TCN_FIRST - 11)
#define TCN_TABDELETE (TCN_FIRST - 12)

#define WM_TABSETSTYLE	(WM_APP + 0x024)

#define nbCtrlMax 10

extern const TCHAR TABBAR_ACTIVEFOCUSEDINDCATOR[64];
extern const TCHAR TABBAR_ACTIVEUNFOCUSEDINDCATOR[64];
extern const TCHAR TABBAR_ACTIVETEXT[64];
extern const TCHAR TABBAR_INACTIVETEXT[64];

struct TBHDR {
	NMHDR hdr;
	int tabOrigin;
};

class TabBar : public Window
{
public:
	TabBar();
	virtual ~TabBar();

	virtual void destroy();
	virtual void init(HINSTANCE hInst, HWND hwnd, bool isVertical = false, bool isTraditional = false, bool isMultiLine = false);
	virtual void reSizeTo(RECT & rc2Ajust);
	int insertAtEnd(const TCHAR *subTabName);

	void activateAt(int index) const;
	void getCurrentTitle(TCHAR *title, int titleLen);

	int getCurrentTabIndex() const;
	void deletItemAt(int index);

	void deletAllItem();

	void setImageList(HIMAGELIST himl);

    int nbItem() const {
        return _nbItem;
    };

	void setFont(TCHAR *fontName, size_t fontSize);

	void setVertical(bool b) {
		_isVertical = b;
	};

	void setMultiLine(bool b) {
		_isMultiLine = b;
	};


protected:
	int  _nbItem;
	bool _hasImgLst;
	HFONT _hFont;
	HFONT _hLargeFont;
	HFONT _hVerticalFont;
	HFONT _hVerticalLargeFont;

	int _ctrlID;
	bool _isTraditional;

	bool _isVertical;
	bool _isMultiLine;

	long getRowCount() const;
};


struct CloseButtonZone;

class TabBarPlus : public TabBar
{
public :

	TabBarPlus();
	virtual ~TabBarPlus();

	enum tabColourIndex {
		activeText, activeFocusedTop, activeUnfocusedTop, inactiveText, inactiveBg
	};

	static void doDragNDrop(bool justDoIt) {
        _doDragNDrop = justDoIt;
    };

	virtual void init(HINSTANCE hInst, HWND hwnd, bool isVertical = false, bool isTraditional = false, bool isMultiLine = false);

    static bool doDragNDropOrNot() {
        return _doDragNDrop;
    };

	int getSrcTabIndex() const {
        return _nSrcTab;
    };

    int getTabDraggedIndex() const {
        return _nTabDragged;
    };

	POINT getDraggingPoint() const {
		return _draggingPoint;
	};

	void resetDraggingPoint() {
		_draggingPoint.x = 0;
		_draggingPoint.y = 0;
	};

	static void doOwnerDrawTab();
	static void doVertical();

	static void doMultiLine();
	static bool isOwnerDrawTab() {return true;}
	static bool drawTopBar() {return _drawTopBar;};
	static bool drawInactiveTab() {return _drawInactiveTab;};
	static bool drawTabCloseButton() {return _drawTabCloseButton;};
	static bool isDbClk2Close() {return _isDbClk2Close;};
	static bool isVertical() { return _isCtrlVertical;};
	static bool isMultiLine() { return _isCtrlMultiLine;};

	static void setDrawTopBar(bool b) {
		_drawTopBar = b;
		doOwnerDrawTab();
	};
	static void setDrawInactiveTab(bool b) {
		_drawInactiveTab = b;
		doOwnerDrawTab();
	};
	static void setDrawTabCloseButton(bool b) {
		_drawTabCloseButton = b;
		doOwnerDrawTab();
	};

	static void setDbClk2Close(bool b) {
		_isDbClk2Close = b;
	};

	static void setVertical(bool b) {
		_isCtrlVertical = b;
		doVertical();
	};

	static void setMultiLine(bool b) {
		_isCtrlMultiLine = b;
		doMultiLine();
	};

	static void setColour(COLORREF colour2Set, tabColourIndex i);
	static COLORREF getColour(tabColourIndex i);

protected:
    // it's the boss to decide if we do the drag N drop
    static bool _doDragNDrop;
	// drag N drop members
	bool _isDragging;
	bool _isDraggingInside;
    int _nSrcTab;
	int _nTabDragged;
	POINT _draggingPoint; // coordinate of Screen
	WNDPROC _tabBarDefaultProc;

	RECT _currentHoverTabRect;
	int _currentHoverTabItem;

	CloseButtonZone* _closeButtonZone;
	bool _isCloseHover;
	int _whichCloseClickDown;
	bool _lmbdHit; // Left Mouse Button Down Hit

	LRESULT runProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);

	static LRESULT CALLBACK TabBarPlus_Proc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
		return (((TabBarPlus *)(::GetWindowLongPtr(hwnd, GWLP_USERDATA)))->runProc(hwnd, Message, wParam, lParam));
	};
	void exchangeItemData(POINT point);


	// it's the boss to decide if we do the ownerDraw style tab
	static bool _drawInactiveTab;
	static bool _drawTopBar;
	static bool _drawTabCloseButton;
	static bool _isDbClk2Close;
	static bool _isCtrlVertical;
	static bool _isCtrlMultiLine;

	static COLORREF _activeTextColour;
	static COLORREF _activeTopBarFocusedColour;
	static COLORREF _activeTopBarUnfocusedColour;
	static COLORREF _inactiveTextColour;
	static COLORREF _inactiveBgColour;

	static int _nbCtrl;
	static HWND _hwndArray[nbCtrlMax];

	void drawItem(DRAWITEMSTRUCT *pDrawItemStruct);
	void draggingCursor(POINT screenPoint);

	int getTabIndexAt(const POINT & p) {
		return getTabIndexAt(p.x, p.y);
	};

	int getTabIndexAt(int x, int y);

	bool isPointInParentZone(POINT screenPoint) const {
        RECT parentZone;
        ::GetWindowRect(_hParent, &parentZone);
	    return (((screenPoint.x >= parentZone.left) && (screenPoint.x <= parentZone.right)) &&
			    (screenPoint.y >= parentZone.top) && (screenPoint.y <= parentZone.bottom));
    };
};

#endif // TAB_BAR_H
