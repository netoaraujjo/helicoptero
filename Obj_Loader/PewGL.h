#ifndef PEWGL_H
#define PEWGL_H

//#define GLEW_STATIC
#include <GL/glew.h>
//#define FREEGLUT_STATIC
#include <GL/freeglut.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>

#include "Tga.h"

//#define SHOW_COLISION_BOUNDS

using namespace std;

namespace PewGL
{
    bool loadObj(const char* filename, vector<glm::vec3>& vertices, vector<glm::vec2>& uvs, vector<glm::vec3>& normals);
    GLuint loadTexture(const char* filename);
    GLuint loadShaders(const char* vertexFile,const char* fragmentFile);
    bool colisionDetection(double xA, double yA, double zA, double radiousA,
                            double xB, double yB, double zB, double radiousB);
};

#endif // PEWGL_H
