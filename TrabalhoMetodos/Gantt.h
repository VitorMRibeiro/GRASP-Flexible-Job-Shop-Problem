#pragma once
#include<GL\glew.h>
#include<SDL\SDL.h>
#include"Display.h"
#include"glRectangle.h"
#include<vector>

struct GanttElement
{
	int beginning;
	int size;
	int line;
	int category;
};
typedef struct GanttElement GanttElement;

struct GanttParameters
{
	int total_Units;
	int line_Quantity;
	int category_Quantity;
	std::vector<GanttElement> Elements;
};
typedef struct GanttParameters GanttParameters;

void Ganttmain(GanttParameters);

void Move(Command command, glRectangle ** rectangles, int QT_OP);
