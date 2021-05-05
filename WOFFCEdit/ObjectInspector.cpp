#include "stdafx.h"
#include "ObjectInspector.h"

// SelectDialogue dialog

IMPLEMENT_DYNAMIC(ObjectInspector, CDialogEx)

//Message map.  Just like MFCMAIN.cpp.  This is where we catch button presses etc and point them to a handy dandy method.
BEGIN_MESSAGE_MAP(ObjectInspector, CDialogEx)
	ON_COMMAND(IDOK, &ObjectInspector::End)					//ok button
	ON_COMMAND(IDCANCEL, &ObjectInspector::End)
	ON_COMMAND(IDCLOSE, &ObjectInspector::End)
	ON_BN_CLICKED(IDOK, &ObjectInspector::OnBnClickedOk)
	ON_BN_CLICKED(ID_APPLY, &ObjectInspector::OnBnClickedApply)
END_MESSAGE_MAP()


ObjectInspector::ObjectInspector(CWnd* pParent, std::vector<SceneObject>* SceneGraph)		//constructor used in modal
	: CDialogEx(IDD_OBJECTINSPECTOR, pParent), m_AxisToMove(0)
{
	m_sceneGraph = SceneGraph;
	m_AxisToMove = 0;
}

ObjectInspector::ObjectInspector(CWnd* pParent)			//constructor used in modeless
	: CDialogEx(IDD_OBJECTINSPECTOR, pParent), m_AxisToMove(0)
{
}

ObjectInspector::~ObjectInspector()
{
}

///pass through pointers to the data in the tool we want to manipulate
void ObjectInspector::SetObjectData(std::vector<SceneObject>* SceneGraph, int* selection, ToolMain* toolMain)
{
	m_sceneGraph = SceneGraph;
	m_currentSelection = selection;
	m_ToolMain = toolMain;

	SetIDText();
}


void ObjectInspector::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//IDs for text boxes
	DDX_Control(pDX, IDC_EDIT_X, newTransform[0]);
	DDX_Control(pDX, IDC_EDIT_Y, newTransform[1]);
	DDX_Control(pDX, IDC_EDIT_Z, newTransform[2]);
								 
	DDX_Control(pDX, IDC_EDIT_RX, newTransform[3]);
	DDX_Control(pDX, IDC_EDIT_RY, newTransform[4]);
	DDX_Control(pDX, IDC_EDIT_RZ, newTransform[5]);

	DDX_Control(pDX, IDC_EDIT_SX, newTransform[6]);
	DDX_Control(pDX, IDC_EDIT_SY, newTransform[7]);
	DDX_Control(pDX, IDC_EDIT_SZ, newTransform[8]);

	DDX_Radio(pDX, IDC_RADIONONE, m_AxisToMove);

	//output current ID value
	//DDX_Control(pDX, IDC_CURRENTVALS, m_currentValues);
}

void ObjectInspector::End()
{

	/*delete m_sceneGraph;
	delete m_currentSelection;
	delete m_ToolMain;

	m_sceneGraph = NULL;
	m_currentSelection = NULL;
	m_ToolMain = NULL;	*/

	DestroyWindow();	//destory the window properly.  INcluding the links and pointers created.  THis is so the dialogue can start again. 
}

void ObjectInspector::SetIDText()
{
	std::wstring message(L"Selected Object: " + std::to_wstring(*m_currentSelection));
	const wchar_t* szMessage = message.c_str();

	//m_currentValues.SetWindowTextW(szMessage);

	wchar_t buffer[256];
	wsprintfW(buffer, szMessage);
	SetDlgItemText(ID_SELECTEDID, buffer);
}

BOOL ObjectInspector::OnInitDialog()
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

void ObjectInspector::PostNcDestroy()
{
}


void ObjectInspector::OnBnClickedOk()
{
	OnBnClickedApply();

	CDialogEx::OnOK();
}

void ObjectInspector::OnBnClickedApply()
{
	UpdateData();

	m_ToolMain->m_movementAxis = static_cast<ToolMain::AxisToMove>(m_AxisToMove);

	SceneObject temp;
	bool firstMove = false;
	bool changeMade = false;
	
	if (m_ToolMain->m_sgFirstMoveFlags.at(*m_currentSelection))
	{
		firstMove = true;
		temp = m_sceneGraph->at(*m_currentSelection);	
	}	


	for (int i = 0; i < 9; i++)
	{
		CString stringVal;

		//get values from arrays
		newTransform[i].GetWindowTextW(stringVal);

		float floatVal;
		floatVal = _ttof(stringVal);

		if (floatVal)
		{
			switch (i)
			{
			case 0://position
				m_sceneGraph->at(*m_currentSelection).posX = floatVal;
				changeMade = true;
				break;
			case 1:
				m_sceneGraph->at(*m_currentSelection).posY = floatVal;
				changeMade = true;
				break;
			case 2:
				m_sceneGraph->at(*m_currentSelection).posZ = floatVal;
				changeMade = true;
				break;
			case 3://rotation
				m_sceneGraph->at(*m_currentSelection).rotX = floatVal;
				changeMade = true;
				break;
			case 4:
				m_sceneGraph->at(*m_currentSelection).rotY = floatVal;
				changeMade = true;
				break;
			case 5:
				m_sceneGraph->at(*m_currentSelection).rotZ = floatVal;
				changeMade = true;
				break;
			case 6://scale
				m_sceneGraph->at(*m_currentSelection).scaX = floatVal;
				changeMade = true;
				break;
			case 7:
				m_sceneGraph->at(*m_currentSelection).scaY = floatVal;
				changeMade = true;
				break;
			case 8:
				m_sceneGraph->at(*m_currentSelection).scaZ = floatVal;
				changeMade = true;
				break;
			default:
				break;
			}
		}
	

	}

	if (changeMade)
	{
		if (firstMove)
		{
			std::vector<SceneObject> tempObject;
			tempObject.push_back(temp);

			std::vector<int> tempIndex;
			tempIndex.push_back(*m_currentSelection);

			m_ToolMain->StoreChanges(tempObject, tempIndex, 2);
			m_ToolMain->m_sgFirstMoveFlags.at(*m_currentSelection) = false;

		}

		std::vector<SceneObject> tempObject;
		tempObject.push_back(m_sceneGraph->at(*m_currentSelection));

		std::vector<int> tempIndex;
		tempIndex.push_back(*m_currentSelection);

		m_ToolMain->StoreChanges(tempObject, tempIndex, 2);

		m_ToolMain->UpdateSceneObjects();
	}
	
}