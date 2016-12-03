#pragma once
#include<GL\glew.h>

class glRectangle
{
public:
	glRectangle(GLfloat, GLfloat, GLfloat, GLfloat);
	~glRectangle();
	void DrawRectangle();
	void ChangeRectanglePos(GLfloat, GLfloat);
	void ChangeRectangleSize(GLfloat, GLfloat);
	void ChangeSizeRedc(GLfloat);
	GLfloat GetXPos() { return XPos; };
	GLfloat GetYPos() { return YPos; };
	void DesenhaContorno();
private:
	GLuint VAO;
	GLuint Buffer;
	GLuint VAOContorno;
	GLuint BufferContorno;
	GLfloat SizeRedc = 0.0f;
	GLfloat* vertices;
	GLfloat* verticesContorno;
	GLfloat XPos;
	GLfloat YPos;
	GLfloat XSize;
	GLfloat YSize;
};

