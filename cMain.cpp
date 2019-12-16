#include "cMain.h"

wxBEGIN_EVENT_TABLE(cMain, wxFrame)
	EVT_BUTTON(10001, OnButtonClicked)
wxEND_EVENT_TABLE()

cMain::cMain() : wxFrame(nullptr, wxID_ANY, "Minesweeper", wxPoint(50,50), wxSize(800, 600))
{
	btn = new wxButton * [nFieldWidth * nFieldHeight];
	wxGridSizer* grid = new wxGridSizer(nFieldWidth, nFieldHeight, 0, 0);

	nMineField = new int[nFieldWidth * nFieldHeight];
	nMarker = new int[nFieldWidth * nFieldHeight];

	wxFont font(24, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);

	for (int x = 0; x < nFieldWidth; x++)
		for (int y = 0; y < nFieldHeight; y++)
		{
			btn[y * nFieldWidth + x] = new wxButton(this, 10000 + (y * nFieldWidth + x));
			btn[y * nFieldWidth + x]->SetFont(font);
			grid->Add(btn[y * nFieldWidth + x], 1, wxEXPAND | wxALL);

			btn[y * nFieldWidth + x]->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cMain::OnButtonClicked, this);
			btn[y * nFieldWidth + x]->Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(cMain::OnRightClicked), NULL, this);
			nMineField[y * nFieldWidth + x] = 0;
			nMarker[y * nFieldWidth + x] = 0;
		}

	this->SetSizer(grid);
	grid->Layout();
}

cMain::~cMain()
{
	delete[]btn;
	delete[]nMineField;
	delete[]nMarker;
}

void cMain::OnRightClicked(wxMouseEvent& evt)
{
	int x = (evt.GetId() - 10000) % nFieldWidth;
	int y = (evt.GetId() - 10000) / nFieldHeight;

		if (nMarker[y * nFieldWidth + x] != 1 && markers_count > 0)
		{
			btn[y * nFieldWidth + x]->SetLabel("X");
			nMarker[y * nFieldWidth + x] = 1;
			markers_count--;
		}
		else if (nMarker[y * nFieldWidth + x] == 1)
		{
			btn[y * nFieldWidth + x]->SetLabel("");
			nMarker[y * nFieldWidth + x] = 0;
			markers_count++;
		}
	
}

void cMain::OnButtonCheck(wxButton* button)
{
	int x = (button->GetId() - 10000) % nFieldWidth;
	int y = (button->GetId() - 10000) / nFieldHeight;

	button->Enable(false);

	int mine_neighbour = 0;
	for (int i = -1; i < 2; i++)
		for (int j = -1; j < 2; j++)
		{
			if (x + i >= 0 && x + i < nFieldWidth && y + j >= 0 && y + j < nFieldHeight)
			{
				if (nMineField[(y + j) * nFieldWidth + (x + i)] == -1)
					mine_neighbour++;
			}
		}
	if (mine_neighbour != 0)
	{
		button->SetLabel(std::to_string(mine_neighbour));
	}
	else
	{
		for (int i = -1; i < 2; i++)
			for (int j = -1; j < 2; j++)
			{
				if (x + i >= 0 && x + i < nFieldWidth && y + j >= 0 && y + j < nFieldHeight && btn[(y + j) * nFieldWidth + (x + i)]->IsEnabled())
				{
					cMain::OnButtonCheck(btn[(y + j) * nFieldWidth + (x + i)]);
				}
			}
	}
}

void cMain::OnButtonClicked(wxCommandEvent& evt)
{
	int x = (evt.GetId() - 10000) % nFieldWidth;
	int y = (evt.GetId() - 10000) / nFieldHeight;

	if (bFirstClick)
	{
		int mines = minesCount;
		while (mines)
		{
			int rx = rand() % nFieldWidth;
			int ry = rand() % nFieldHeight;

			if (nMineField[ry * nFieldWidth + rx] == 0 && rx != x && ry != y)
			{
				nMineField[ry * nFieldWidth + rx] = -1;
				mines--;
			}
		}
		bFirstClick = false;
	}

	btn[y * nFieldWidth + x]->Enable(false);

	if (nMineField[y * nFieldWidth + x] == -1)
	{
		wxMessageBox("BOOOM !! - Game Over !");

		//reset
		bFirstClick = true;
		for (int x = 0; x < nFieldWidth; x++)
			for (int y = 0; y < nFieldHeight; y++)
			{
				nMineField[y * nFieldWidth + x] = 0;
				nMarker[y * nFieldWidth + x] = 0;
				btn[y * nFieldWidth + x]->SetLabel("");
				btn[y * nFieldWidth + x]->Enable(true);
				markers_count = minesCount;
			}


	}
	else
	{
		int mine_neighbour = 0;
		for (int i = -1; i < 2; i++)
			for (int j = -1; j < 2; j++)
			{
				if (x + i >= 0 && x + i < nFieldWidth && y + j >= 0 && y + j < nFieldHeight)
				{
					if (nMineField[(y + j) * nFieldWidth + (x + i)] == -1)
						mine_neighbour++;
				}
			}
		if (mine_neighbour != 0)
		{
			btn[y * nFieldWidth + x]->SetLabel(std::to_string(mine_neighbour));
		}
		else
		{
			for(int i = -1; i <2; i++)
				for (int j = -1; j < 2; j++)
				{
					if (x + i >= 0 && x + i < nFieldWidth && y + j >= 0 && y + j < nFieldHeight)
					{
					cMain::OnButtonCheck(btn[(y + j) * nFieldWidth + (x + i)]);
					}
				}
		}
	}

	evt.Skip();
	
}