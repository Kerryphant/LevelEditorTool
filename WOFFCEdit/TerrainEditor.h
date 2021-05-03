#pragma once
#include "afxdialogex.h"
#include "resource.h"
#include "afxwin.h"
#include <vector>
#include "ToolMain.h"

class TerrainEditor : public CDialogEx
{

	DECLARE_DYNAMIC(TerrainEditor)

public:
	TerrainEditor(CWnd* pParent = NULL);
	virtual ~TerrainEditor();
	void SetObjectData(ToolMain* toolMain);	//passing in pointers to the data the class will operate on.

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void End();		//kill the dialogue
	afx_msg void Select();	//Item has been selected

	ToolMain* m_ToolMain;

	int m_selectedType;

	DECLARE_MESSAGE_MAP()
public:
	// Control variable for more efficient access of the listbox
	virtual BOOL OnInitDialog() override;
	virtual void PostNcDestroy();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedApply();
	afx_msg void OnLbnSelchangeList1();
};