#include "include/glRectangle.h"
#include <iostream>

// abstração do OpenGL para desenhar retangulos na tela  

glRectangle::glRectangle(GLfloat xPos, GLfloat yPos, GLfloat xSize, GLfloat ySize){
	XPos = xPos;
	YPos = yPos;
	XSize = xSize;
	YSize = ySize;

	vertices = new GLfloat[12];
	vertices[0] = XPos + SizeRedc;
	vertices[1] = YPos - SizeRedc;
	vertices[2] = XPos + XSize - SizeRedc;
	vertices[3] = YPos - SizeRedc;
	vertices[4] = XPos + SizeRedc;
	vertices[5] = YPos - YSize + SizeRedc;
	vertices[6] = XPos + XSize - SizeRedc;
	vertices[7] = YPos - SizeRedc;
	vertices[8] = XPos + SizeRedc;
	vertices[9] = YPos - YSize + SizeRedc;
	vertices[10] = XPos + XSize - SizeRedc;
	vertices[11] = YPos - YSize + SizeRedc;

	// buffers and VAOs.
	glGenBuffers(1, &Buffer);
	glBindBuffer(GL_ARRAY_BUFFER, Buffer);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, Buffer);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	verticesContorno = new GLfloat[16];

	// Contorno buffers and VAOs.
	glGenBuffers(1, &BufferContorno);
	glBindBuffer(GL_ARRAY_BUFFER, BufferContorno);
	glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(GLfloat), verticesContorno, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAOContorno);
	glBindVertexArray(VAOContorno);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, BufferContorno);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//debug.
	GLenum Status;
	Status = glGetError();
	switch (Status){
	case GL_NO_ERROR:
		break;
	case GL_INVALID_ENUM:
		std::cout << "Invalid enum\n";
		break;
	case GL_INVALID_VALUE:
		std::cout << "Invalid value\n";
		break;
	case GL_INVALID_OPERATION:
		std::cout << "Invalid operetion\n";
		break;
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		std::cout << "Frame buffer error\n";
		break;
	case GL_OUT_OF_MEMORY:
		std::cout << "Out of memory";
		break;
	default:
		std::cout << "Unknow erro\n";
		break;
	}
}


glRectangle::~glRectangle(){
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &Buffer);
}

void glRectangle::DrawRectangle(){
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	return;
}



void glRectangle::ChangeRectanglePos(GLfloat NewXPos, GLfloat NewYPos){
	XPos = NewXPos;
	YPos = NewYPos;
	vertices[0] = XPos + SizeRedc;
	vertices[1] = YPos - SizeRedc;
	vertices[2] = XPos + XSize - SizeRedc;
	vertices[3] = YPos - SizeRedc;
	vertices[4] = XPos + SizeRedc;
	vertices[5] = YPos - YSize + SizeRedc;
	vertices[6] = XPos + XSize - SizeRedc;
	vertices[7] = YPos - SizeRedc;
	vertices[8] = XPos + SizeRedc;
	vertices[9] = YPos - YSize + SizeRedc;
	vertices[10] = XPos + XSize - SizeRedc;
	vertices[11] = YPos - YSize + SizeRedc;

	glBindBuffer(GL_ARRAY_BUFFER, Buffer);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return;
}

void glRectangle::ChangeRectangleSize(GLfloat, GLfloat){
	return;
}

void glRectangle::ChangeSizeRedc(GLfloat newValue){
	SizeRedc = newValue;
	vertices[0] = XPos + SizeRedc;
	vertices[1] = YPos - SizeRedc;
	vertices[2] = XPos + XSize - SizeRedc;
	vertices[3] = YPos - SizeRedc;
	vertices[4] = XPos + SizeRedc;
	vertices[5] = YPos - YSize + SizeRedc;
	vertices[6] = XPos + XSize - SizeRedc;
	vertices[7] = YPos - SizeRedc;
	vertices[8] = XPos + SizeRedc;
	vertices[9] = YPos - YSize + SizeRedc;
	vertices[10] = XPos + XSize - SizeRedc;
	vertices[11] = YPos - YSize + SizeRedc;

	glBindBuffer(GL_ARRAY_BUFFER, Buffer);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void glRectangle::DesenhaContorno(){
	verticesContorno[0] = vertices[0];
	verticesContorno[1] = vertices[1];
	verticesContorno[2] = vertices[2];
	verticesContorno[3] = vertices[3];

	verticesContorno[4] = vertices[2];
	verticesContorno[5] = vertices[3];
	verticesContorno[6] = vertices[10];
	verticesContorno[7] = vertices[11];

	verticesContorno[8] = vertices[10];
	verticesContorno[9] = vertices[11];
	verticesContorno[10] = vertices[4];
	verticesContorno[11] = vertices[5];

	verticesContorno[12] = vertices[0];
	verticesContorno[13] = vertices[1];
	verticesContorno[14] = vertices[4];
	verticesContorno[15] = vertices[5];

	glBindBuffer(GL_ARRAY_BUFFER, BufferContorno);
	glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(GLfloat), verticesContorno, GL_STATIC_DRAW);
	glBindVertexArray(VAOContorno);
	glDrawArrays(GL_LINES, 0, 8);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return;
}
