#pragma once

#include <afxwin.h> 
#include <afxext.h>
#include <afx.h>
#include "pch.h"
#include "Game.h"
#include "ToolMain.h"
#include "resource.h"
#include "MFCFrame.h"
#include "SelectDialogue.h"
#include "ObjectInspector.h"
#include "TerrainEditor.h"

class MFCMain : public CWinApp 
{
public:
	MFCMain();
	~MFCMain();
	BOOL InitInstance();
	int  Run();

private:

	CMyFrame * m_frame;	//handle to the frame where all our UI is
	HWND m_toolHandle;	//Handle to the MFC window
	ToolMain m_ToolSystem;	//Instance of Tool System that we interface to. 
	CRect WindowRECT;	//Window area rectangle. 
	SelectDialogue m_ToolSelectDialogue;			//for modeless dialogue, declare it here

	ObjectInspector m_ObjectInspector;
	TerrainEditor m_TerrainEditor;

	int m_width;		
	int m_height;
	
	bool ObjInspecCreated = false;

	//Interface funtions for menu and toolbar etc requires
	afx_msg void MenuFileQuit();
	afx_msg void MenuFileSaveTerrain();
	afx_msg void MenuEditSelect();
	afx_msg	void ToolBarButton1();
	afx_msg	void ObjectInspector();
	afx_msg	void Undo();
	afx_msg	void Redo();
	afx_msg	void TerrainEdit();

	DECLARE_MESSAGE_MAP()	// required macro for message map functionality  One per class
};
