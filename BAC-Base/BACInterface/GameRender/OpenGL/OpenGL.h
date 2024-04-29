#pragma once
#include <stdio.h>
#include <iostream>


enum OpenGLVersion
{
	OpenGL1,
	OpenGL2,
	OpenGL3,
	OpenGL4,
};


class OpenGL
{
private:
	OpenGLVersion _current_version = OpenGLVersion::OpenGL1;

public:
	OpenGL();
	~OpenGL();

public:

};

