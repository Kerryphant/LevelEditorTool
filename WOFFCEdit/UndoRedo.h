#pragma once
#include "SceneObject.h"
#include <vector>
#include <stack>

struct ObjectChange
{
	enum OperationType
	{
		deletion = 0,
		creation = 1,
		change = 2
	};

	OperationType s_operation;
	std::vector<int> s_indexesSceneGraph;
	std::vector<SceneObject> s_originalObjects;
};

class UndoRedo
{
public:

	UndoRedo();

	void	StoreChanges(std::vector<SceneObject> originalObjects, std::vector<int> objectIndex, int operationType, bool undoStack = true);
	void	UndoChange(std::vector<SceneObject>& m_sceneGraph_);
	void	RedoChange(std::vector<SceneObject>& m_sceneGraph_);

private:

	bool wasUndoLastOper;

	std::stack<ObjectChange> m_undoObjectStack;
	std::stack<ObjectChange> m_redoObjectStack;

};