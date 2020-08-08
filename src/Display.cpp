#include "include/Display.h"
#include<iostream>

// cria e gerencia os eventos da janela onde sera desenhado o grafico de gantt

Display::Display(int width, int height, std::string title)
{
	// inicializa o SDL.
	SDL_Init(SDL_INIT_EVERYTHING);

	// configuracao da janela.
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// cria janela.
	window = SDL_CreateWindow(
		title.c_str(),            // titulo da janela.
		SDL_WINDOWPOS_CENTERED,   // (int) posicao da janela no eixo x.
		SDL_WINDOWPOS_CENTERED,   // (int) posicao da janela no eixo y.
		width,                    // (int) altura da janela.
		height,                   // (int) largura da janela.
		SDL_WINDOW_OPENGL);       // flag.

	// cria contexto openGL para a janela.
	SDL_GL_CreateContext(window);
	
	// inicializa o glew.
	GLenum status = glewInit();
	if (status != GLEW_OK)
	{
		std::cout << "failed to initialize glew\n";
		return;
	}

	// cria os shaders.
	Shaders[0] = glCreateShader(GL_VERTEX_SHADER);
	Shaders[1] = glCreateShader(GL_FRAGMENT_SHADER);

	std::string vertexShaderSource = "#version 300 es \n layout (location = 0) in vec4 vPosition; void main(){gl_Position = vPosition;} ";
	std::string fragmentShaderSource = "#version 300 es \n out highp vec4 fcolor; uniform highp vec4 ucolor; void main(){fcolor = ucolor;}";

	GLint LengthVS = vertexShaderSource.length();
	GLint LengthFS = fragmentShaderSource.length();

	const GLchar* CVertexShaderSource = vertexShaderSource.c_str();
	const GLchar* CFragmentShaderSource = fragmentShaderSource.c_str();

	glShaderSource(Shaders[0], 1, &CVertexShaderSource, &LengthVS);
	glShaderSource(Shaders[1], 1, &CFragmentShaderSource, &LengthFS);

	// compila o shader.
	glCompileShader(Shaders[0]);
	glCompileShader(Shaders[1]);

	// verificacao de erro.
	GLint VStatus = 0;
	GLint FStatus = 0;
	glGetShaderiv(Shaders[0], GL_COMPILE_STATUS, &VStatus);
	glGetShaderiv(Shaders[1], GL_COMPILE_STATUS, &FStatus);
	//std::cerr << "Vertex shader status: " << VStatus << std::endl;
	//std::cerr << "Fragment shader status: " << FStatus << std::endl;
	if (VStatus == GL_FALSE)
	{
		std::cerr << "Vertex Shader Compilation erro!\n";
		GLchar* infoLog = new GLchar[512];
		GLsizei* length;
		glGetShaderInfoLog(Shaders[0], 512 * sizeof(GLchar), length, infoLog);
		std::cout << infoLog << std::endl;
	}
	if (FStatus == GL_FALSE)
	{
		std::cerr << "fragment Shader Compilation erro!\n";
		GLchar* infoLog = new GLchar[512];
		GLsizei* length;
		glGetShaderInfoLog(Shaders[1], 512 * sizeof(GLchar), length, infoLog);
		std::cout << infoLog << std::endl;
	}
	// cria programa.
	Program = glCreateProgram();
	glAttachShader(Program, Shaders[0]);
	glAttachShader(Program, Shaders[1]);
	glLinkProgram(Program);

	// verifica erro.
	glGetProgramiv(Program, GL_LINK_STATUS, &VStatus);
	if (VStatus == GL_FALSE)
	{
		std::cerr << "Linking error\n";
	}

	glUseProgram(Program);

	
	isClosed = false;
	PrevKey = SDLK_ESCAPE;
}


Display::~Display()
{
	glDeleteShader(Shaders[0]);
	glDeleteShader(Shaders[1]);
	glDeleteProgram(Program);
	SDL_GL_DeleteContext(window);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

Command Display::Update()
{
	SDL_GL_SwapWindow(window);
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE)
		{
			std::cout << "Quit\n";
			isClosed = true;
			return Command::Nothing;
		}
		switch (event.key.keysym.sym)
		{
		default:
			return Command::Nothing;
			break;
		}
	}
	return Command::Nothing;
}

bool Display::getIsClosed()
{
	return isClosed;
}

void Display::Clear(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	glClearColor(red, green, blue, alpha);
	glClear(GL_COLOR_BUFFER_BIT);
}	


