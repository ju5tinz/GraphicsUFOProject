#ifndef _MAIN_H_
#define _MAIN_H_

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>
#include "Window.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#endif
