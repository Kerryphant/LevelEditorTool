#include "UndoRedo.h"

UndoRedo::UndoRedo()
{
	wasUndoLastOper = false;
	undoStackPointer = -2;
}

void UndoRedo::StoreChanges(std::vector<SceneObject> originalObjects, std::vector<int> objectIndexes, int operationType, bool undoStack)
{

	//if (wasUndoLastOper)
	//{
		int size = m_redoObjectStack.size();
		for (int i = 0; i < size; i++)
		{
			m_redoObjectStack.pop();
		}
		//if (size) { undoStackPointer++; }
	//}

	ObjectChange newChange;
	for (int i = 0; i < originalObjects.size(); i++)
	{
		newChange.s_originalObjects.push_back(originalObjects[i]);
		newChange.s_indexesSceneGraph.push_back(objectIndexes[i]);
	}
	

	switch (operationType)
	{
	case 0:
		newChange.s_operation = ObjectChange::OperationType::deletion;
		break;
	case 1:
		newChange.s_operation = ObjectChange::OperationType::creation;
		break;
	case 2:
		newChange.s_operation = ObjectChange::OperationType::change;
		break;
	default:
		break;
	}

	undoStackPointer++;

	if (undoStack)
	{
		if (undoStackPointer + 1 >= m_undoObjectStack.size())
		{
			m_undoObjectStack.push_back(newChange);
		}
		else
		{
			m_undoObjectStack[undoStackPointer + 1] = newChange;
		}
	}
	else
	{
		m_redoObjectStack.push(newChange);
	}

}

void UndoRedo::UndoChange(std::vector<SceneObject>& m_sceneGraph_)
{
	if (undoStackPointer > -1)
	{
		ObjectChange change = m_undoObjectStack[undoStackPointer];

		switch (change.s_operation)
		{
		case ObjectChange::OperationType::deletion:

			break;
		case ObjectChange::OperationType::creation:
			m_sceneGraph_.pop_back();
			break;
		case ObjectChange::OperationType::change:
			//revert the object to its original state
			for (int i = 0; i < change.s_originalObjects.size(); i++)
			{
				m_sceneGraph_[change.s_indexesSceneGraph[i]] = change.s_originalObjects[i];
			}


			break;
		default:
			break;
		}

		m_redoObjectStack.push(m_undoObjectStack[undoStackPointer + 1]);
		//m_undoObjectStack.pop();
		undoStackPointer--;

		wasUndoLastOper = true;
	}

}

void UndoRedo::RedoChange(std::vector<SceneObject>& m_sceneGraph_)
{
	if (!m_redoObjectStack.empty())
	{
		ObjectChange change = m_redoObjectStack.top();

		switch (change.s_operation)
		{
		case ObjectChange::OperationType::deletion:

			break;
		case ObjectChange::OperationType::creation:
			for (int i = 0; i < change.s_originalObjects.size(); i++)
			{
				m_sceneGraph_.push_back(change.s_originalObjects[i]);
			}			
			break;
		case ObjectChange::OperationType::change:
			//revert the object to its original state
			for (int i = 0; i < change.s_originalObjects.size(); i++)
			{
				m_sceneGraph_[change.s_indexesSceneGraph[i]] = change.s_originalObjects[i];
			}

			break;
		default:
			break;
		}

		undoStackPointer++;
		//m_undoObjectStack.push(change);
		m_undoObjectStack[undoStackPointer + 1] = change;

		

		m_redoObjectStack.pop();

		wasUndoLastOper = false;
	}
}