#include "GameObject.h"

GameObject::GameObject()
{

}

GameObject::~GameObject()
{
//    glDeleteBuffers(1, &vertexBuffer);
//	glDeleteBuffers(1, &uvBuffer);
//	glDeleteTextures(1, &textureId);
}

void GameObject::load(const char * meshFile, const char * textureFile, GLuint programId)
{
    // Load mesh data
    PewGL::loadObj(meshFile, vertices, uvs, normals);

	glGenVertexArrays(1, &vertexArrayId);
	glBindVertexArray(vertexArrayId);

    // Load it into a VBO
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

    glGenBuffers(1, &normalsBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

	glBindVertexArray(0);

    // Load TGA texture
    textureId = PewGL::loadTexture(textureFile);
    if (!textureId)
        printf("Erro ao carregar Textura");

    // Set Program
    this->programId = programId;

    /*for (register int i = 0; i < (int)vertices.size(); i+=3)
    {
        centre.x += vertices[i].x;
        centre.y += vertices[i].y;
        centre.z += vertices[i].z;
    }

    centre.x /= vertices.size();
    centre.y /= vertices.size();
    centre.z /= vertices.size();*/
}

void GameObject::oldRender()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);

    for (register int i = 0; i < (int)vertices.size(); i+=3)
    {
        glBegin(GL_TRIANGLES);
        for (register int j = 0; j < 3; j++){
            glTexCoord2f(uvs[i+j].x, uvs[i+j].y);
            glNormal3f(normals[i+j].x, normals[i+j].y, normals[i+j].z);
            glVertex3f(vertices[i+j].x, vertices[i+j].y, vertices[i+j].z);
        }
        glEnd();
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}

void GameObject::render()
{
    #ifdef COMPATIBILITY_MODE
        oldRender();
        return;
    #endif

    glUseProgram(programId);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);

    GLuint TextureID  = glGetUniformLocation(programId, "myTextureSampler");
    glUniform1i(TextureID, 0);

	glBindVertexArray(vertexArrayId);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(
        0,                  // attribute
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    // 2nd attribute buffer : UVs
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glVertexAttribPointer(
        1,                                // attribute
        2,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );

    // 3rd attribute buffer : Normals
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
    glVertexAttribPointer(
        2,                                // attribute
        3,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );

    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);
}
