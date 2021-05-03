#include "stdafx.h"
#include "TerrainEditor.h"

// SelectDialogue dialog

IMPLEMENT_DYNAMIC(TerrainEditor, CDialogEx)

//Message map.  Just like MFCMAIN.cpp.  This is where we catch button presses etc and point them to a handy dandy method.
BEGIN_MESSAGE_MAP(TerrainEditor, CDialogEx)
	ON_COMMAND(IDOK, &TerrainEditor::End)					//ok button
	ON_COMMAND(IDCANCEL, &TerrainEditor::End)
	ON_COMMAND(IDCLOSE, &TerrainEditor::End)
	ON_BN_CLICKED(IDOK, &TerrainEditor::OnBnClickedOk)
	ON_BN_CLICKED(IDC_APPLY, &TerrainEditor::OnBnClickedApply)
END_MESSAGE_MAP()

TerrainEditor::TerrainEditor(CWnd* pParent)			//constructor used in modeless
	: CDialogEx(IDD_TERRAIN, pParent), m_selectedType(0)
{
	m_selectedType = 0;
}

TerrainEditor::~TerrainEditor()
{
}

///pass through pointers to the data in the tool we want to manipulate
void TerrainEditor::SetObjectData(ToolMain* toolMain)
{
	m_ToolMain = toolMain;
}


void TerrainEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Radio(pDX, IDC_RAISE, m_selectedType);
}

void TerrainEditor::End()
{
	m_ToolMain->SetTerrainEditing(false);

	DestroyWindow();	//destory the window properly.  INcluding the links and pointers created.  THis is so the dialogue can start again. 
}

BOOL TerrainEditor::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//uncomment for modal only
/*	//roll through all the objects in the scene graph and put an entry for each in the listbox
	int numSceneObjects = m_sceneGraph->size();
	for (size_t i = 0; i < numSceneObjects; i++)
	{
		//easily possible to make the data string presented more complex. showing other columns.
		std::wstring listBoxEntry = std::to_wstring(m_sceneGraph->at(i).ID);
		m_listBox.AddString(listBoxEntry.c_str());
	}*/

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void TerrainEditor::PostNcDestroy()
{
}


void TerrainEditor::OnBnClickedOk()
{
	OnBnClickedApply();

	CDialogEx::OnOK();
}

void TerrainEditor::OnBnClickedApply()
{
	UpdateData();

	m_ToolMain->m_terrainMode = static_cast<ToolMain::TerrainMode>(m_selectedType);
}