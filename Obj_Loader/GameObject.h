#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

//#define GLEW_STATIC
#include <GL/glew.h>
//#define FREEGLUT_STATIC
#include <GL/freeglut.h>
#include <vector>
#include <glm/glm.hpp>

#include "PewGL.h"

#define COMPATIBILITY_MODE

using namespace std;

class GameObject
{
    public:
        GLuint vertexArrayId, vertexBuffer, uvBuffer, normalsBuffer;
        GLuint textureId;
        GLuint programId;
        vector<glm::vec3> vertices;
        vector<glm::vec2> uvs;
        vector<glm::vec3> normals;
        glm::vec3 centre;

        GameObject();
        virtual ~GameObject();

        void load(const char * meshFile, const char * textureFile, GLuint programId);
        void render();
        void oldRender();
    protected:
    private:
};

#endif // GAME_OBJECT_H
