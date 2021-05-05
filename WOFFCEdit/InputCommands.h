#pragma once

struct InputCommands
{
	bool forward;
	bool back;
	bool right;
	bool left;
	bool rotRight;
	bool rotLeft;

	bool mouse_LB_Down;
	bool mouse_RB_Down;
	
	int mouse_X;
	int mouse_Y;
	int mouse_last_X;
	int mouse_last_Y;


	bool shift;
	bool ctrl;

	bool undo;
	bool redo;

	bool C;
	bool Z;
	bool Y;

	bool newObject;
};
