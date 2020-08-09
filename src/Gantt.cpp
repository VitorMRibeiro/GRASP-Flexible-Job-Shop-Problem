#include<iostream>
#include"include/Gantt.h"
#include"ctime"

// desenha um grafico de gantt usando os retangulos implementados em 'glRectangle.cpp'

#define MOVE 0.02f

struct color{
	GLfloat red;
	GLfloat green;
	GLfloat blue;
	GLfloat alpha;
};
typedef struct color color;

void Ganttmain(GanttParameters parameters){
	GLfloat UNIT_SCALE = 2.0f / parameters.total_Units;
	GLfloat LINE_SCALE = 1.5f / parameters.line_Quantity;
	GLfloat RDC_SCALE = LINE_SCALE / 6.0f;
	Display display(900, 500, "Gantt Chart");
	color *Job_Color = new color[parameters.category_Quantity];
	bool *Job_Op_Have_Color = new bool[parameters.Elements.size()];
	glRectangle** rectangles = new glRectangle*[parameters.Elements.size()];
	Command command = Command::Nothing;

	for (unsigned int i = 0; i < parameters.Elements.size(); i++){
		Job_Op_Have_Color[i] = false;
	}

	srand((unsigned int)time(NULL));


	for (unsigned int i = 0; i < parameters.Elements.size(); i++){

		GLfloat YPos = 1.0f - ((GLfloat)parameters.Elements[i].line * LINE_SCALE + RDC_SCALE );
		GLfloat XPos = -1.0f + ((GLfloat)parameters.Elements[i].beginning * UNIT_SCALE);
		GLfloat XSize = (GLfloat)parameters.Elements[i].size * UNIT_SCALE;
		GLfloat YSize = LINE_SCALE - RDC_SCALE;
		rectangles[i] = new glRectangle(XPos, YPos, XSize, YSize);

		if (!Job_Op_Have_Color[parameters.Elements[i].category]){
			Job_Color[parameters.Elements[i].category].red = (GLfloat) ( rand() % 10) / 10.0f;
			Job_Color[parameters.Elements[i].category].green = (GLfloat) (rand() % 10) / 10.0f;
			Job_Color[parameters.Elements[i].category].blue = (GLfloat) (rand() % 10) / 10.0f;
			Job_Color[parameters.Elements[i].category].alpha = 1.0f;
			Job_Op_Have_Color[parameters.Elements[i].category] = true;
		}
	}

	while (!display.getIsClosed()){
		display.Clear(0.9f, 0.9f, 1.0f, 1.0f);
		for (unsigned int i = 0; i < parameters.Elements.size(); i++){
			// send the job color to the fragment shader.
			GLuint program = display.GetProgram();
			GLint uColorLocation = glGetUniformLocation(program, "ucolor");
			glUniform4f(uColorLocation, Job_Color[parameters.Elements[i].category].red,
									    Job_Color[parameters.Elements[i].category].blue,
										Job_Color[parameters.Elements[i].category].green,
										Job_Color[parameters.Elements[i].category].alpha);

			rectangles[i]->DrawRectangle();
			
			// send the color black to the fragment shader.
			uColorLocation = glGetUniformLocation(program, "ucolor");
			glUniform4f(uColorLocation, 0.0f, 0.0f, 0.0f, 0.0f);

			rectangles[i]->DesenhaContorno();
		}
		command = display.Update();
		Move(command, rectangles, parameters.Elements.size());
	}
	return;
}

void CreateScale(unsigned int Units_total, GLfloat unit_scale, GLuint* VAO, GLuint* Buffer){
	constexpr GLfloat SCALE_Y_POS = -1.9f;
	GLfloat* vertices = new GLfloat[Units_total * 8];
	for (unsigned int i = 0; i < Units_total * 8; i++){
		GLfloat Xpos = i * unit_scale;
		vertices[i] = Xpos;
		vertices[i++] = SCALE_Y_POS;

		vertices[i++] = Xpos + unit_scale;
		vertices[i++] = SCALE_Y_POS;

		vertices[i++] = Xpos + unit_scale;
		vertices[i++] = SCALE_Y_POS;

		vertices[i++] = Xpos + unit_scale;
		vertices[i++] = SCALE_Y_POS + 0.05f;
	}
	glGenBuffers(1, Buffer);
	glBindBuffer(GL_ARRAY_BUFFER, *Buffer);
	glBufferData(GL_ARRAY_BUFFER, Units_total * 8 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, VAO);
	glBindVertexArray(*VAO);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, *Buffer);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Move(Command command, glRectangle** rectangles, int QT_OP){

	switch (command){
	case Command::Nothing:
		break;
	case Command::Left:
		for (int i = 0; i < QT_OP; i++){
			rectangles[i]->ChangeRectanglePos(rectangles[i]->GetXPos() + MOVE, rectangles[i]->GetYPos());
		}
		break;
	case Command::Right:
		for (int i = 0; i < QT_OP; i++){
			rectangles[i]->ChangeRectanglePos(rectangles[i]->GetXPos() - MOVE, rectangles[i]->GetYPos());
		}
		break;
	case Command::Down:
		for (int i = 0; i < QT_OP; i++){
			rectangles[i]->ChangeRectanglePos(rectangles[i]->GetXPos(), rectangles[i]->GetYPos() + MOVE);
		}
		break;
	case Command::Up:
		for (int i = 0; i < QT_OP; i++){
			rectangles[i]->ChangeRectanglePos(rectangles[i]->GetXPos(), rectangles[i]->GetYPos() - MOVE);
		}
		break;
	case Command::Space:
		break;
	default:
		break;
	}
	return;
}
