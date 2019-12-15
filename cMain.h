#pragma once
#include "wx/wx.h"

class cMain : public wxFrame
{
public:
	cMain();
	~cMain();

public:
	int nFieldWidth = 20;
	int nFieldHeight = 20;
	wxButton** btn;
	int *nMineField = nullptr;
	int *nMarker = nullptr;
	bool bFirstClick = true;
	int minesCount = 30;
	int markers_count = 30;

	void OnButtonClicked(wxCommandEvent& evt);
	void OnRightClicked(wxMouseEvent& evt);

	wxDECLARE_EVENT_TABLE();
};

