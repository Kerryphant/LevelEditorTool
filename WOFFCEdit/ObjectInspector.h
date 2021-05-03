#pragma once
#include "afxdialogex.h"
#include "resource.h"
#include "afxwin.h"
#include "SceneObject.h"
#include <vector>
#include "ToolMain.h"

class ObjectInspector : public CDialogEx
{

	DECLARE_DYNAMIC(ObjectInspector)

public:
	ObjectInspector(CWnd* pParent, std::vector<SceneObject>* SceneGraph);   // modal // takes in out scenegraph in the constructor
	ObjectInspector(CWnd* pParent = NULL);
	virtual ~ObjectInspector();
	void SetObjectData(std::vector<SceneObject>* SceneGraph, int* Selection, ToolMain* toolMain);	//passing in pointers to the data the class will operate on.

	void SetIDText();
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void End();		//kill the dialogue
	afx_msg void Select();	//Item has been selected

	std::vector<SceneObject>* m_sceneGraph;
	int* m_currentSelection;

	CEdit newTransform[9];

	int m_AxisToMove;

	ToolMain* m_ToolMain;

	DECLARE_MESSAGE_MAP()
public:
	// Control variable for more efficient access of the listbox
	virtual BOOL OnInitDialog() override;
	virtual void PostNcDestroy();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedApply();
	afx_msg void OnLbnSelchangeList1();
};