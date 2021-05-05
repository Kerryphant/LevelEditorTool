#include "ToolMain.h"
#include "resource.h"
#include <vector>
#include <sstream>

//
//ToolMain Class
ToolMain::ToolMain()
{

	m_currentChunk = 0;		//default value
	m_selectedObject = 0;	//initial selection ID
	m_sceneGraph.clear();	//clear the vector for the scenegraph
	m_databaseConnection = NULL;

	//zero input commands
	m_toolInputCommands.forward		= false;
	m_toolInputCommands.back		= false;
	m_toolInputCommands.left		= false;
	m_toolInputCommands.right		= false;
	
	m_toolInputCommands.mouse_X = 0;
	m_toolInputCommands.mouse_Y = 0;
	m_toolInputCommands.mouse_LB_Down = false;

	m_selectMultiple = false;
	m_terrainSculpting = false;
	m_objectCreated = false;
	m_undoCompleted = false;
	m_redoCompleted = false;

	lastIntersection = DirectX::XMFLOAT4(0, 0, 0, 1);
	currentIntersection = DirectX::XMFLOAT4(0, 0, 0, 1);
}


ToolMain::~ToolMain()
{
	sqlite3_close(m_databaseConnection);		//close the database connection
}


int ToolMain::getCurrentSelectionID()
{

	return  m_selectedObject;
}

std::vector<int> ToolMain::getCurrentSelectionIDs()
{

	return m_selectedObjects;
}


void ToolMain::onActionInitialise(HWND handle, int width, int height)
{
	//window size, handle etc for directX
	m_width		= width;
	m_height	= height;
	
	m_d3dRenderer.Initialize(handle, m_width, m_height);

	//database connection establish
	int rc;
	rc = sqlite3_open_v2("database/test.db",&m_databaseConnection, SQLITE_OPEN_READWRITE, NULL);

	if (rc) 
	{
		TRACE("Can't open database");
		//if the database cant open. Perhaps a more catastrophic error would be better here
	}
	else 
	{
		TRACE("Opened database successfully");
	}

	onActionLoad();

	for (int i = 0; i < m_sceneGraph.size(); i++)
	{
		m_sgFirstMoveFlags.push_back(true);
	}
}

void ToolMain::onActionFocusCamera()
{
}

void ToolMain::onActionLoad()
{
	//load current chunk and objects into lists
	if (!m_sceneGraph.empty())		//is the vector empty
	{
		m_sceneGraph.clear();		//if not, empty it
	}

	//SQL
	int rc;
	char *sqlCommand;
	char *ErrMSG = 0;
	sqlite3_stmt *pResults;								//results of the query
	sqlite3_stmt *pResultsChunk;

	//OBJECTS IN THE WORLD
	//prepare SQL Text
	sqlCommand = "SELECT * from Objects";				//sql command which will return all records from the objects table.
	//Send Command and fill result object
	rc = sqlite3_prepare_v2(m_databaseConnection, sqlCommand, -1, &pResults, 0 );
	
	//loop for each row in results until there are no more rows.  ie for every row in the results. We create and object
	while (sqlite3_step(pResults) == SQLITE_ROW)
	{	
		SceneObject newSceneObject;
		newSceneObject.ID = sqlite3_column_int(pResults, 0);
		newSceneObject.chunk_ID = sqlite3_column_int(pResults, 1);
		newSceneObject.model_path		= reinterpret_cast<const char*>(sqlite3_column_text(pResults, 2));
		newSceneObject.tex_diffuse_path = reinterpret_cast<const char*>(sqlite3_column_text(pResults, 3));
		newSceneObject.posX = sqlite3_column_double(pResults, 4);
		newSceneObject.posY = sqlite3_column_double(pResults, 5);
		newSceneObject.posZ = sqlite3_column_double(pResults, 6);
		newSceneObject.rotX = sqlite3_column_double(pResults, 7);
		newSceneObject.rotY = sqlite3_column_double(pResults, 8);
		newSceneObject.rotZ = sqlite3_column_double(pResults, 9);
		newSceneObject.scaX = sqlite3_column_double(pResults, 10);
		newSceneObject.scaY = sqlite3_column_double(pResults, 11);
		newSceneObject.scaZ = sqlite3_column_double(pResults, 12);
		newSceneObject.render = sqlite3_column_int(pResults, 13);
		newSceneObject.collision = sqlite3_column_int(pResults, 14);
		newSceneObject.collision_mesh = reinterpret_cast<const char*>(sqlite3_column_text(pResults, 15));
		newSceneObject.collectable = sqlite3_column_int(pResults, 16);
		newSceneObject.destructable = sqlite3_column_int(pResults, 17);
		newSceneObject.health_amount = sqlite3_column_int(pResults, 18);
		newSceneObject.editor_render = sqlite3_column_int(pResults, 19);
		newSceneObject.editor_texture_vis = sqlite3_column_int(pResults, 20);
		newSceneObject.editor_normals_vis = sqlite3_column_int(pResults, 21);
		newSceneObject.editor_collision_vis = sqlite3_column_int(pResults, 22);
		newSceneObject.editor_pivot_vis = sqlite3_column_int(pResults, 23);
		newSceneObject.pivotX = sqlite3_column_double(pResults, 24);
		newSceneObject.pivotY = sqlite3_column_double(pResults, 25);
		newSceneObject.pivotZ = sqlite3_column_double(pResults, 26);
		newSceneObject.snapToGround = sqlite3_column_int(pResults, 27);
		newSceneObject.AINode = sqlite3_column_int(pResults, 28);
		newSceneObject.audio_path = reinterpret_cast<const char*>(sqlite3_column_text(pResults, 29));
		newSceneObject.volume = sqlite3_column_double(pResults, 30);
		newSceneObject.pitch = sqlite3_column_double(pResults, 31);
		newSceneObject.pan = sqlite3_column_int(pResults, 32);
		newSceneObject.one_shot = sqlite3_column_int(pResults, 33);
		newSceneObject.play_on_init = sqlite3_column_int(pResults, 34);
		newSceneObject.play_in_editor = sqlite3_column_int(pResults, 35);
		newSceneObject.min_dist = sqlite3_column_double(pResults, 36);
		newSceneObject.max_dist = sqlite3_column_double(pResults, 37);
		newSceneObject.camera = sqlite3_column_int(pResults, 38);
		newSceneObject.path_node = sqlite3_column_int(pResults, 39);
		newSceneObject.path_node_start = sqlite3_column_int(pResults, 40);
		newSceneObject.path_node_end = sqlite3_column_int(pResults, 41);
		newSceneObject.parent_id = sqlite3_column_int(pResults, 42);
		newSceneObject.editor_wireframe = sqlite3_column_int(pResults, 43);
		newSceneObject.name = reinterpret_cast<const char*>(sqlite3_column_text(pResults, 44));

		newSceneObject.light_type = sqlite3_column_int(pResults, 45);
		newSceneObject.light_diffuse_r = sqlite3_column_double(pResults, 46);
		newSceneObject.light_diffuse_g = sqlite3_column_double(pResults, 47);
		newSceneObject.light_diffuse_b = sqlite3_column_double(pResults, 48);
		newSceneObject.light_specular_r = sqlite3_column_double(pResults, 49);
		newSceneObject.light_specular_g = sqlite3_column_double(pResults, 50);
		newSceneObject.light_specular_b = sqlite3_column_double(pResults, 51);
		newSceneObject.light_spot_cutoff = sqlite3_column_double(pResults, 52);
		newSceneObject.light_constant = sqlite3_column_double(pResults, 53);
		newSceneObject.light_linear = sqlite3_column_double(pResults, 54);
		newSceneObject.light_quadratic = sqlite3_column_double(pResults, 55);
	

		//send completed object to scenegraph
		m_sceneGraph.push_back(newSceneObject);
	}

	//THE WORLD CHUNK
	//prepare SQL Text
	sqlCommand = "SELECT * from Chunks";				//sql command which will return all records from  chunks table. There is only one tho.
														//Send Command and fill result object
	rc = sqlite3_prepare_v2(m_databaseConnection, sqlCommand, -1, &pResultsChunk, 0);


	sqlite3_step(pResultsChunk);
	m_chunk.ID = sqlite3_column_int(pResultsChunk, 0);
	m_chunk.name = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 1));
	m_chunk.chunk_x_size_metres = sqlite3_column_int(pResultsChunk, 2);
	m_chunk.chunk_y_size_metres = sqlite3_column_int(pResultsChunk, 3);
	m_chunk.chunk_base_resolution = sqlite3_column_int(pResultsChunk, 4);
	m_chunk.heightmap_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 5));
	m_chunk.tex_diffuse_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 6));
	m_chunk.tex_splat_alpha_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 7));
	m_chunk.tex_splat_1_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 8));
	m_chunk.tex_splat_2_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 9));
	m_chunk.tex_splat_3_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 10));
	m_chunk.tex_splat_4_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 11));
	m_chunk.render_wireframe = sqlite3_column_int(pResultsChunk, 12);
	m_chunk.render_normals = sqlite3_column_int(pResultsChunk, 13);
	m_chunk.tex_diffuse_tiling = sqlite3_column_int(pResultsChunk, 14);
	m_chunk.tex_splat_1_tiling = sqlite3_column_int(pResultsChunk, 15);
	m_chunk.tex_splat_2_tiling = sqlite3_column_int(pResultsChunk, 16);
	m_chunk.tex_splat_3_tiling = sqlite3_column_int(pResultsChunk, 17);
	m_chunk.tex_splat_4_tiling = sqlite3_column_int(pResultsChunk, 18);


	//Process REsults into renderable
	m_d3dRenderer.BuildDisplayList(&m_sceneGraph);
	//build the renderable chunk 
	m_d3dRenderer.BuildDisplayChunk(&m_chunk);

}

void ToolMain::onActionSave()
{
	//SQL
	int rc;
	char *sqlCommand;
	char *ErrMSG = 0;
	sqlite3_stmt *pResults;								//results of the query
	

	//OBJECTS IN THE WORLD Delete them all
	//prepare SQL Text
	sqlCommand = "DELETE FROM Objects";	 //will delete the whole object table.   Slightly risky but hey.
	rc = sqlite3_prepare_v2(m_databaseConnection, sqlCommand, -1, &pResults, 0);
	sqlite3_step(pResults);

	//Populate with our new objects
	std::wstring sqlCommand2;
	int numObjects = m_sceneGraph.size();	//Loop thru the scengraph.

	for (int i = 0; i < numObjects; i++)
	{
		std::stringstream command;
		command << "INSERT INTO Objects " 
			<<"VALUES(" << m_sceneGraph.at(i).ID << ","
			<< m_sceneGraph.at(i).chunk_ID  << ","
			<< "'" << m_sceneGraph.at(i).model_path <<"'" << ","
			<< "'" << m_sceneGraph.at(i).tex_diffuse_path << "'" << ","
			<< m_sceneGraph.at(i).posX << ","
			<< m_sceneGraph.at(i).posY << ","
			<< m_sceneGraph.at(i).posZ << ","
			<< m_sceneGraph.at(i).rotX << ","
			<< m_sceneGraph.at(i).rotY << ","
			<< m_sceneGraph.at(i).rotZ << ","
			<< m_sceneGraph.at(i).scaX << ","
			<< m_sceneGraph.at(i).scaY << ","
			<< m_sceneGraph.at(i).scaZ << ","
			<< m_sceneGraph.at(i).render << ","
			<< m_sceneGraph.at(i).collision << ","
			<< "'" << m_sceneGraph.at(i).collision_mesh << "'" << ","
			<< m_sceneGraph.at(i).collectable << ","
			<< m_sceneGraph.at(i).destructable << ","
			<< m_sceneGraph.at(i).health_amount << ","
			<< m_sceneGraph.at(i).editor_render << ","
			<< m_sceneGraph.at(i).editor_texture_vis << ","
			<< m_sceneGraph.at(i).editor_normals_vis << ","
			<< m_sceneGraph.at(i).editor_collision_vis << ","
			<< m_sceneGraph.at(i).editor_pivot_vis << ","
			<< m_sceneGraph.at(i).pivotX << ","
			<< m_sceneGraph.at(i).pivotY << ","
			<< m_sceneGraph.at(i).pivotZ << ","
			<< m_sceneGraph.at(i).snapToGround << ","
			<< m_sceneGraph.at(i).AINode << ","
			<< "'" << m_sceneGraph.at(i).audio_path << "'" << ","
			<< m_sceneGraph.at(i).volume << ","
			<< m_sceneGraph.at(i).pitch << ","
			<< m_sceneGraph.at(i).pan << ","
			<< m_sceneGraph.at(i).one_shot << ","
			<< m_sceneGraph.at(i).play_on_init << ","
			<< m_sceneGraph.at(i).play_in_editor << ","
			<< m_sceneGraph.at(i).min_dist << ","
			<< m_sceneGraph.at(i).max_dist << ","
			<< m_sceneGraph.at(i).camera << ","
			<< m_sceneGraph.at(i).path_node << ","
			<< m_sceneGraph.at(i).path_node_start << ","
			<< m_sceneGraph.at(i).path_node_end << ","
			<< m_sceneGraph.at(i).parent_id << ","
			<< m_sceneGraph.at(i).editor_wireframe << ","
			<< "'" << m_sceneGraph.at(i).name << "'" << ","

			<< m_sceneGraph.at(i).light_type << ","
			<< m_sceneGraph.at(i).light_diffuse_r << ","
			<< m_sceneGraph.at(i).light_diffuse_g << ","
			<< m_sceneGraph.at(i).light_diffuse_b << ","
			<< m_sceneGraph.at(i).light_specular_r << ","
			<< m_sceneGraph.at(i).light_specular_g << ","
			<< m_sceneGraph.at(i).light_specular_b << ","
			<< m_sceneGraph.at(i).light_spot_cutoff << ","
			<< m_sceneGraph.at(i).light_constant << ","
			<< m_sceneGraph.at(i).light_linear << ","
			<< m_sceneGraph.at(i).light_quadratic

			<< ")";
		std::string sqlCommand2 = command.str();
		rc = sqlite3_prepare_v2(m_databaseConnection, sqlCommand2.c_str(), -1, &pResults, 0);
		sqlite3_step(pResults);	
	}
	MessageBox(NULL, L"Objects Saved", L"Notification", MB_OK);
}

void ToolMain::onActionSaveTerrain()
{
	m_d3dRenderer.SaveDisplayChunk(&m_chunk);
}

void ToolMain::onActionObjInspec()
{

}

void ToolMain::Tick(MSG *msg)
{	
	UpdateSceneObjects();
	
	HandleObjectSelection();

	HandleKeyboardShortcuts();

	HandleMouseDrag();

	//Renderer Update Call
	m_d3dRenderer.Tick(&m_toolInputCommands);
}

void ToolMain::UpdateInput(MSG * msg)
{
	switch (msg->message)
	{
		//Global inputs,  mouse position and keys etc
	case WM_KEYDOWN:
		m_keyArray[msg->wParam] = true;
		break;

	case WM_KEYUP:
		m_keyArray[msg->wParam] = false;
		break;

	case WM_MOUSEMOVE://mouse movement
		m_toolInputCommands.mouse_last_X = m_toolInputCommands.mouse_X;
		m_toolInputCommands.mouse_last_Y = m_toolInputCommands.mouse_Y;

		m_toolInputCommands.mouse_X = GET_X_LPARAM(msg->lParam);//set mouse pos x
		m_toolInputCommands.mouse_Y = GET_Y_LPARAM(msg->lParam);//set mouse pos y
		break;

	case WM_LBUTTONDOWN://mouse button down
		//set flag for the mouse button in inputcommands
		m_toolInputCommands.mouse_LB_Down = true;
		break;
	case WM_LBUTTONUP:
		m_toolInputCommands.mouse_LB_Down = false;
		break;
	case WM_RBUTTONDOWN://mouse button down
	//set flag for the mouse button in inputcommands
		m_toolInputCommands.mouse_RB_Down = true;
		break;
	case WM_RBUTTONUP:
		m_toolInputCommands.mouse_RB_Down = false;
		break;
	}
	
	//WASD movement
	if (m_keyArray['W'])
	{
		m_toolInputCommands.forward = true;
	}
	else m_toolInputCommands.forward = false;

	if (m_keyArray['S'])
	{
		m_toolInputCommands.back = true;
	}
	else m_toolInputCommands.back = false;

	if (m_keyArray['A'])
	{
		m_toolInputCommands.left = true;
	}
	else m_toolInputCommands.left = false;

	if (m_keyArray['D'])
	{
		m_toolInputCommands.right = true;
	}
	else m_toolInputCommands.right = false;
	
	//camera rotation
	if (m_keyArray['E'])
	{
		m_toolInputCommands.rotRight = true;
	}
	else m_toolInputCommands.rotRight = false;
	
	if (m_keyArray['Q'])
	{
		m_toolInputCommands.rotLeft = true;
	}
	else m_toolInputCommands.rotLeft = false;

	//shift key
	if (m_keyArray[16])
	{
		m_toolInputCommands.shift = true;
	}
	else m_toolInputCommands.shift = false;
	
	//control key
	if (m_keyArray[17])
	{
		m_toolInputCommands.ctrl = true;
	}
	else m_toolInputCommands.ctrl = false;

	if (m_keyArray['C'])
	{
		m_toolInputCommands.C = true;
	}
	else m_toolInputCommands.C = false;

	if (m_keyArray['Z'])
	{
		m_toolInputCommands.Z = true;
	}
	else m_toolInputCommands.Z = false;

	if (m_keyArray['Y'])
	{
		m_toolInputCommands.Y = true;
	}
	else m_toolInputCommands.Y = false;


	if (m_keyArray[17] && m_keyArray['Z'])
	{
		m_toolInputCommands.undo = true;
	}
	else
	{
		m_toolInputCommands.undo = false;
		m_undoCompleted = false;
	}

	if (m_keyArray[17] && m_keyArray['Y'])
	{
		m_toolInputCommands.redo = true;
	}
	else
	{
		m_toolInputCommands.redo = false;
		m_redoCompleted = false;
	}

	//New object create
	if (m_keyArray['N'])
	{
		m_toolInputCommands.newObject = true;
	}
	else
	{
		m_toolInputCommands.newObject = false;
		m_objectCreated = false;
	}

}

void ToolMain::UpdateSceneObjects()
{
	m_d3dRenderer.BuildDisplayList(&m_sceneGraph);
}

void ToolMain::HandleObjectSelection()
{
	if (!m_terrainSculpting)
	{
		if (m_dragging || m_toolInputCommands.ctrl)
		{
			return;
		}

		bool processedLClick = false;

		//multiple object selection
		if (m_toolInputCommands.mouse_LB_Down && m_toolInputCommands.shift)
		{
			if (!m_selectMultiple)
			{
				m_selectedObjects.clear();
			}

			int objectSelected = m_d3dRenderer.MousePicking();

			bool validID = true;
			if (objectSelected < 0)
			{
				validID = false;
			}
			else
			{
				for (int i = 0; i < m_selectedObjects.size(); i++)
				{
					if (m_selectedObjects[i] == objectSelected)
					{
						validID = false;
					}
				}
			}

			if (validID)
			{
				m_selectedObjects.push_back(objectSelected);
			}

			processedLClick = true;
			m_selectMultiple = true;
		}
		if (!m_toolInputCommands.shift && m_selectMultiple)
		{
			m_selectMultiple = false;
		}

		//single selection
		if (m_toolInputCommands.mouse_LB_Down && !processedLClick)
		{
			m_selectedObjects.clear();

			int objectSelected = m_d3dRenderer.MousePicking();
			if (objectSelected >= 0)
			{
				m_selectedObjects.push_back(m_d3dRenderer.MousePicking());
			}
		}

		if (m_selectedObjects.size() == 1)
		{
			m_selectedObject = m_selectedObjects[0];
		}
	}
	else
	{
		if (m_toolInputCommands.mouse_LB_Down)
		{
			m_d3dRenderer.SetLowerOrRaise(m_terrainMode);
			m_d3dRenderer.TerrainSculpt();
			m_toolInputCommands.mouse_LB_Down = false;
		}
	}
}

void ToolMain::HandleKeyboardShortcuts()
{
	if (m_toolInputCommands.undo)
	{
		UndoChange();

		m_toolInputCommands.Z = false;
		m_toolInputCommands.ctrl = false;
	}	
	
	if (m_toolInputCommands.redo)
	{
		RedoChange();

		m_toolInputCommands.Y = false;
		m_toolInputCommands.ctrl = false;
	}

	if (m_toolInputCommands.newObject)
	{
		CreateNewObject();
	}
}

void ToolMain::HandleMouseDrag()
{
	if (!m_dragging && m_toolInputCommands.mouse_LB_Down && m_toolInputCommands.ctrl && m_selectedObjects.size())
	{
		for (int i = 0; i < m_selectedObjects.size(); i++)
		{
			if (m_d3dRenderer.MousePicking() == m_selectedObjects[i])
			{
				m_dragging = true;

				std::vector<SceneObject> selectedObjects;
				for (int i = 0; i < m_selectedObjects.size(); i++)
				{
					selectedObjects.push_back(m_sceneGraph[m_selectedObjects[i]]);
				}
				undoRedoHandler.StoreChanges(selectedObjects, m_selectedObjects, 2);
				break;
			}
		}

		if (m_dragging)
		{
			lastIntersection = m_d3dRenderer.GetXZIntersection();
			currentIntersection = m_d3dRenderer.GetXZIntersection();
		}
	}

	if ((m_dragging && !m_toolInputCommands.mouse_LB_Down) || (m_dragging && !m_toolInputCommands.ctrl))
	{
		m_dragging = false;

		std::vector<SceneObject> selectedObjects;
		for (int i = 0; i < m_selectedObjects.size(); i++)
		{
			selectedObjects.push_back(m_sceneGraph[m_selectedObjects[i]]);
		}
		undoRedoHandler.StoreChanges(selectedObjects, m_selectedObjects, 2);

		lastIntersection.x = 0;
		lastIntersection.z = 0;

		currentIntersection.x = 0;
		currentIntersection.z = 0;
	}
	

	if (m_dragging && m_movementAxis == 1)
	{
		lastIntersection = currentIntersection;
		currentIntersection = m_d3dRenderer.GetXZIntersection();		

		DirectX::XMFLOAT4 intersectionDifference(0, 0, 0, 1);
		

		intersectionDifference.x = lastIntersection.x - currentIntersection.x;
		intersectionDifference.z = lastIntersection.z - currentIntersection.z;
		

		for (int i = 0; i < m_selectedObjects.size(); i++)
		{
			//calculate offset
			

			//set the position
			m_sceneGraph[m_selectedObjects[i]].posX -= intersectionDifference.x;
			m_sceneGraph[m_selectedObjects[i]].posZ -= intersectionDifference.z;
		}
	}
	
	//m_toolInputCommands.mouse_LB_Down = false;
	//m_toolInputCommands.ctrl = false;
	
}

void ToolMain::StoreChanges(std::vector<SceneObject> originalObjects, std::vector<int> objectIndex, int operationType, bool undoStack)
{
	undoRedoHandler.StoreChanges(originalObjects, objectIndex, operationType, undoStack);
}

void ToolMain::UndoChange()
{
	if (!m_undoCompleted)
	{
		undoRedoHandler.UndoChange(m_sceneGraph);
		m_undoCompleted = true;
	}
	
}

void ToolMain::RedoChange()
{
	if (!m_redoCompleted)
	{
		undoRedoHandler.RedoChange(m_sceneGraph);
		m_redoCompleted = true;
	}
}


void ToolMain::CreateNewObject()
{
	if(!m_objectCreated)
	{ 
		int largestID = 0;
		for (int i = 0; i < m_sceneGraph.size(); i++)
		{
			if (m_sceneGraph[i].ID > largestID)
			{
				largestID = m_sceneGraph[i].ID;
			}
		}

		SceneObject newSceneObject;
		newSceneObject.ID = largestID + 1;
		newSceneObject.chunk_ID = 0;
		newSceneObject.model_path = "database/data/placeholder.cmo";
		newSceneObject.tex_diffuse_path = "database/data/placeholder.dds";
		newSceneObject.posX = 0.0f;
		newSceneObject.posY = 0.0f;
		newSceneObject.posZ = 0.0f;
		newSceneObject.rotX = 0.0f;
		newSceneObject.rotY = 0.0f;
		newSceneObject.rotZ = 0.0f;
		newSceneObject.scaX = 1.0f;
		newSceneObject.scaY = 1.0f;
		newSceneObject.scaZ = 1.0f;
		newSceneObject.render = true;
		newSceneObject.collision = false;
		newSceneObject.collision_mesh = "";
		newSceneObject.collectable = false;
		newSceneObject.destructable = false;
		newSceneObject.health_amount = 0;
		newSceneObject.editor_render = true;
		newSceneObject.editor_texture_vis = true;
		newSceneObject.editor_normals_vis = false;
		newSceneObject.editor_collision_vis = false;
		newSceneObject.editor_pivot_vis = false;
		newSceneObject.pivotX = 0.0f;
		newSceneObject.pivotY = 0.0f;
		newSceneObject.pivotZ = 0.0f;
		newSceneObject.snapToGround = false;
		newSceneObject.AINode = false;
		newSceneObject.audio_path = "";
		newSceneObject.volume = 0.0f;
		newSceneObject.pitch = 0.0f;
		newSceneObject.pan = 0.0f;
		newSceneObject.one_shot = false;
		newSceneObject.play_on_init = false;
		newSceneObject.play_in_editor = false;
		newSceneObject.min_dist = 0;
		newSceneObject.max_dist = 0;
		newSceneObject.camera = false;
		newSceneObject.path_node = false;
		newSceneObject.path_node_start = false;
		newSceneObject.path_node_end = false;
		newSceneObject.parent_id = -1;
		newSceneObject.editor_wireframe = false;
		newSceneObject.name = "new Object";

		newSceneObject.light_type = 0;
		newSceneObject.light_diffuse_r = 1;
		newSceneObject.light_diffuse_g = 1;
		newSceneObject.light_diffuse_b = 1;
		newSceneObject.light_specular_r = 1;
		newSceneObject.light_specular_g = 1;
		newSceneObject.light_specular_b = 1;
		newSceneObject.light_spot_cutoff = 1;
		newSceneObject.light_constant = 1;
		newSceneObject.light_linear = 1;
		newSceneObject.light_quadratic = 1;


		//send completed object to scenegraph
		m_sceneGraph.push_back(newSceneObject);
		m_sgFirstMoveFlags.push_back(false);

		std::vector<SceneObject> tempNewObject;
		tempNewObject.push_back(newSceneObject);

		std::vector<int> tempIndex;
		tempIndex.push_back(m_sceneGraph.size() - 1);

		StoreChanges(tempNewObject, tempIndex, 1);

		m_objectCreated = true;
	}	
}

void ToolMain::DuplicateObject(SceneObject objectToCopy)
{
	SceneObject newObject;

	newObject = objectToCopy;
	
	int largestID = 0;
	for (int i = 0; i < m_sceneGraph.size(); i++)
	{
		if (m_sceneGraph[i].ID > largestID)
		{
			largestID = m_sceneGraph[i].ID;
		}
	}

	newObject.ID = largestID + 1;

	m_sceneGraph.push_back(newObject);
}

void ToolMain::DuplicateObject(int indexToObject)
{
	SceneObject newObject;

	newObject = m_sceneGraph[indexToObject];

	int largestID = 0;
	for (int i = 0; i < m_sceneGraph.size(); i++)
	{
		if (m_sceneGraph[i].ID > largestID)
		{
			largestID = m_sceneGraph[i].ID;
		}
	}

	newObject.ID = largestID + 1;

	m_sceneGraph.push_back(newObject);
}

void ToolMain::SetTerrainEditing(bool toggle)
{
	m_terrainSculpting = toggle;
}
