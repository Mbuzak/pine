#ifndef __SHADER_STUFF
#define __SHADER_STUFF

// References:
// https://www.opengl.org/sdk/docs/tutorials/ClockworkCoders/loading.php

#include <fstream>

// --------------------------------------------------------------
// NOWE: Zmienne kontrolujace polozenie i orientacje kamery
// --------------------------------------------------------------

// Zmienne do kontroli obrotu kamery
GLfloat CameraRotate_x = 0.0f;
GLfloat CameraRotate_y = 0.0f;

// Zmienne do kontroli pozycji kamery
GLfloat CameraTranslate_x = 0.0f;
GLfloat CameraTranslate_y = 0.0f;
GLfloat CameraTranslate_z = -2.0f;

// Zmienne do kontroli stanu myszy
int _mouse_buttonState = GLUT_UP;
int _mouse_buttonX;
int _mouse_buttonY;

// --------------------------------------------------------------
// Funkcja zwraca macierz widoku dla kamery
// --------------------------------------------------------------
glm::mat4 UpdateViewMatrix()
{
	glm::mat4 matView = glm::mat4x4( 1.0 );
	matView = glm::translate( matView, glm::vec3( CameraTranslate_x, CameraTranslate_y, CameraTranslate_z) );
	matView = glm::rotate( matView, CameraRotate_x, glm::vec3( 1.0f, 0.0f, 0.0f ) );
	matView = glm::rotate( matView, CameraRotate_y, glm::vec3( 0.0f, 1.0f, 0.0f ) );

	return matView;
}




// --------------------------------------------------------------
// Funkcja wywolywana przy nacisnieciu tzw. specjalnych
// klawiszy (klawiszy spoza tablicy ASCII)
// --------------------------------------------------------------
void SpecialKeys( int key, int x, int y )
{
    switch( key )
    {

        case GLUT_KEY_LEFT:
            CameraTranslate_x -= 0.2f;
            break;

        case GLUT_KEY_RIGHT:
            CameraTranslate_x += 0.2f;
            break;

		 case GLUT_KEY_UP:
            CameraTranslate_y += 0.2f;
            break;

        case GLUT_KEY_DOWN:
            CameraTranslate_y -= 0.2f;
            break;

    }

    glutPostRedisplay();
}



// --------------------------------------------------------------
// Funkcja zwrotna wywolywana podczas nacisnieciu klawisza myszy
// --------------------------------------------------------------
void MouseButton( int button, int state, int x, int y )
{
    if( button == GLUT_LEFT_BUTTON )
    {

        _mouse_buttonState = state;


        if( state == GLUT_DOWN )
        {
            _mouse_buttonX = x;
            _mouse_buttonY = y;
        }
    }
}

// --------------------------------------------------------------
// Funkcja zwrotna wywolywana podczas ruchu kursorem myszy
// --------------------------------------------------------------
void MouseMotion( int x, int y )
{
    if( _mouse_buttonState == GLUT_DOWN )
    {
        CameraRotate_y += 2*(x - _mouse_buttonX)/(float)glutGet( GLUT_WINDOW_WIDTH );
        _mouse_buttonX = x;
        CameraRotate_x -= 2*(_mouse_buttonY - y)/(float)glutGet( GLUT_WINDOW_HEIGHT );
        _mouse_buttonY = y;
        glutPostRedisplay();
    }
}

// --------------------------------------------------------------
// Funkcja wywolywana podczas ruchu rolki myszy
// --------------------------------------------------------------
void MouseWheel(int button, int dir, int x, int y)
{
    if (dir > 0)
    {
        // Zoom in
        CameraTranslate_z += 0.5f;
    }
    else
    {
        // Zoom out
        CameraTranslate_z -= 0.5f;
    }

    glutPostRedisplay();
}

// --------------------------------------------------------------
// Funkcje zwiazane z wczytywaniem i kompilacja plikow shaderow
// --------------------------------------------------------------

// --------------------------------------------------------------
unsigned long getFileLength(std::ifstream& file)
{
	if(!file.good()) return 0;

	unsigned long pos=file.tellg();
	file.seekg(0,std::ios::end);
	unsigned long len = file.tellg();
	file.seekg(std::ios::beg);

	return len;
}

// --------------------------------------------------------------
GLchar * LoadShaderFile(const char* filename)
{
	std::ifstream file;
	file.open(filename, std::ios::in); // opens as ASCII!
	if(!file) { printf("Can't open file %s !\n", filename); exit(1); }

	unsigned long len = getFileLength(file);

	if (len==0) { printf("File %s is empty!\n", filename); exit(1); };   // Error: Empty File

	GLchar *ShaderSource = new GLchar[len+1];
	if (ShaderSource == NULL) { printf("Can't reserve memory %d \n", len+1); exit(1); }   // can't reserve memory

	// len isn't always strlen cause some characters are stripped in ascii read...
	// it is important to 0-terminate the real length later, len is just max possible value...
	ShaderSource[len] = 0;

	unsigned int i=0;
	while (file.good())
	{
		ShaderSource[i] = file.get();
		if (!file.eof())
		i++;
	}

	ShaderSource[i] = 0;
	file.close();
	return ShaderSource;
}

// --------------------------------------------------------------
void CheckForErrors_Shader(GLuint shader)
{
	GLint status;
	glGetShaderiv( shader, GL_COMPILE_STATUS, &status );
	if( status != GL_TRUE )
	{
		printf("Blad!\n");
		GLint logLength;
		glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logLength );
		char *log = new char[logLength];
		glGetShaderInfoLog( shader, logLength, NULL, log );
		printf("LOG: %s\n", log);
		delete[] log;
	}
}

// --------------------------------------------------------------
void CheckForErrors_Program(GLuint program, GLenum mode)
{
	GLint status;
	glGetProgramiv( program, mode, &status );
	if( status != GL_TRUE )
	{
		switch (mode)
		{
			case GL_LINK_STATUS:
				printf("Blad konsolidacji programu!\n");
				break;
			case GL_VALIDATE_STATUS:
				printf("Blad walidacji programu!\n");
				break;
			default:
				printf("Inny blad programu!\n");

		}
		GLint logLength;
		glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logLength );
		char *log = new char[logLength];
	    glGetProgramInfoLog( program, logLength, NULL, log );
		printf("LOG: %s\n", log);
		delete[] log;
	}
}

// --------------------------------------------------------------
GLuint LoadShader( GLuint MODE, const char *filename  )
{
	// utworzenie obiektu shadera
	GLuint shader = glCreateShader( MODE );

	// Wczytanie kodu shadera z pliku
	GLchar *code = LoadShaderFile(filename);

	glShaderSource( shader, 1, &code, NULL );
	glCompileShader( shader );
	CheckForErrors_Shader(shader);

	delete[] code;
	return shader;
}

// --------------------------------------------------------------
void LinkAndValidateProgram(GLuint program)
{
    // Konsolidacja programu
    glLinkProgram( program );
	CheckForErrors_Program(program, GL_LINK_STATUS);

	// Walidacja programu
    glValidateProgram( program );
    CheckForErrors_Program(program, GL_VALIDATE_STATUS);
}

#endif

