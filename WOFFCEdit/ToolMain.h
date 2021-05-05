#pragma once

#include <afxext.h>
#include "pch.h"
#include "Game.h"
#include "sqlite3.h"
#include "SceneObject.h"
#include "InputCommands.h"
#include <vector>
#include "UndoRedo.h"

class ToolMain
{
public: //methods
	ToolMain();
	~ToolMain();

	enum AxisToMove
	{
		none = 0,
		zx = 1,
		y = 2
	};

	enum TerrainMode
	{
		raise = 0,
		lower = 1
	};

	//onAction - These are the interface to MFC
	int		getCurrentSelectionID();										//returns the selection number of currently selected object so that It can be displayed.
	std::vector<int> getCurrentSelectionIDs();										
	
	void	onActionInitialise(HWND handle, int width, int height);			//Passes through handle and hieght and width and initialises DirectX renderer and SQL LITE
	void	onActionFocusCamera();
	void	onActionLoad();													//load the current chunk
	afx_msg	void	onActionSave();											//save the current chunk
	afx_msg void	onActionSaveTerrain();									//save chunk geometry
	afx_msg void	onActionObjInspec();									//open object inspector

	void	Tick(MSG *msg);
	void	UpdateInput(MSG *msg);

	void	UpdateSceneObjects();
	void	HandleObjectSelection();
	void	HandleKeyboardShortcuts();

	void	HandleMouseDrag();

	void	StoreChanges(std::vector<SceneObject> originalObjects, std::vector<int> objectIndex, int operationType, bool undoStack = true);
	void	UndoChange();
	void	RedoChange();

	void	CreateNewObject();
	void	DuplicateObject(SceneObject objectToCopy);
	void	DuplicateObject(int indexToObject);
	void	DeleteObject(int objectIndex);

	void	SetTerrainEditing(bool toggle);

public:	//variables
	std::vector<SceneObject>    m_sceneGraph;	//our scenegraph storing all the objects in the current chunk
	std::vector<bool>			m_sgFirstMoveFlags;	//
	ChunkObject					m_chunk;		//our landscape chunk
	int							m_selectedObject; //ID of current Selection
	std::vector<int>			m_selectedObjects;

	AxisToMove					m_movementAxis;
	TerrainMode					m_terrainMode;


private:	//methods
	void	onContentAdded();


		
private:	//variables
	HWND	m_toolHandle;		//Handle to the  window
	Game	m_d3dRenderer;		//Instance of D3D rendering system for our tool
	InputCommands m_toolInputCommands;		//input commands that we want to use and possibly pass over to the renderer
	CRect	WindowRECT;		//Window area rectangle. 
	char	m_prevKeyArray[256];
	char	m_keyArray[256];
	sqlite3 *m_databaseConnection;	//sqldatabase handle

	int m_width;		//dimensions passed to directX
	int m_height;
	int m_currentChunk;			//the current chunk of thedatabase that we are operating on.  Dictates loading and saving. 
	
	bool m_selectMultiple;
	bool m_ctrlHeld;
	
	bool m_terrainSculpting;

	bool m_objectCreated;
	bool m_dragging;
	bool m_undoCompleted;
	bool m_redoCompleted;

	DirectX::XMFLOAT4 lastIntersection;
	DirectX::XMFLOAT4 currentIntersection;

	UndoRedo undoRedoHandler;
};
