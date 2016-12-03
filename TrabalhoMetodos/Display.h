#pragma once
#include<iostream>
#include<GL\glew.h>
#include<SDL\SDL.h>
#include"Command.h"

class Display
{
public:
	Display(int,int,std::string);
	~Display();
	Command Update();
	bool getIsClosed();
	void Clear(GLfloat, GLfloat, GLfloat, GLfloat);
	void drawSquare(GLfloat Posx, GLfloat Posy, GLfloat Tam);
	bool isClosed;
	SDL_Window* GetWindow() { return window; };
	GLuint GetProgram() { return Program; };
private:
	SDL_Window* window;
	SDL_Keycode PrevKey;
	GLuint Program;
	SDL_GLContext glContext;
	GLuint Shaders[2];
};

