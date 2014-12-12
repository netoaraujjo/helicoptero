#include "PewGL.h"

bool PewGL::loadObj(const char * filename, vector<glm::vec3>& vertices, vector<glm::vec2>& uvs,	vector<glm::vec3>& normals)
{
	printf("Loading mesh \"%s\" ...", filename);

	vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	vector<glm::vec3> temp_vertices;
	vector<glm::vec2> temp_uvs;
	vector<glm::vec3> temp_normals;


	FILE * file = fopen(filename, "r");
	if( file == NULL ){
		printf("Error while opening the file !");
		return false;
	}

	while(true){
		char line[128];
		// read the first word of the line
		int res = fscanf(file, "%s", line);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse line
		if (strcmp(line, "v") == 0){
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
			//printf("vertex %f %f %f", vertex.x, vertex.y, vertex.z);
		}else if(strcmp(line, "vt") == 0){
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y );
			temp_uvs.push_back(uv);
			//printf("uv %f %f", uv.x, uv.y);
		}else if(strcmp(line, "vn") == 0){
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
			//printf("normal %f %f %f", normal.x, normal.y, normal.z);
		}else if(strcmp(line, "f") == 0){
			//string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
                        &vertexIndex[0], &uvIndex[0], &normalIndex[0],
                        &vertexIndex[1], &uvIndex[1], &normalIndex[1],
                        &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9){
				printf("File can't be read by our simple parser :-( Try exporting with other options");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}else{
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}
	}

	// For each vertex of each triangle
	for(unsigned int i = 0; i < vertexIndices.size(); i++){

		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[vertexIndex-1];
		glm::vec2 uv = temp_uvs[uvIndex-1];
		glm::vec3 normal = temp_normals[normalIndex-1];

		// Put the attributes in buffers
		vertices.push_back(vertex);
		uvs.push_back(uv);
		normals.push_back(normal);

	}
    printf("%d vertices, %d uvs, %d normals, %d faces carregadas...",
             (int)vertices.size(), (int)uvs.size(), (int)normals.size(), (int)vertices.size()/3);
	printf("Done...");
	return true;
}

GLuint PewGL::loadTexture(const char *filename)
{
    printf("Carregando Texture \"%s\"", filename);
    return loadTGATexture(filename);
}

GLuint PewGL::loadShaders(const char* vertexFile, const char* fragmentFile)
{
	// Create the shaders
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	string vertexShaderCode;
	ifstream vertexShaderStream(vertexFile, ios::in);
	if(vertexShaderStream.is_open()){
		string Line = "";
		while(getline(vertexShaderStream, Line))
			vertexShaderCode += "\n" + Line;
		vertexShaderStream.close();
	}

	// Read the Fragment Shader code from the file
	string fragmentShaderCode;
	ifstream fragmentShaderStream(fragmentFile, ios::in);
	if(fragmentShaderStream.is_open()){
		string Line = "";
		while(getline(fragmentShaderStream, Line))
			fragmentShaderCode += "\n" + Line;
		fragmentShaderStream.close();
	}

	GLint result = GL_FALSE;
	int infoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s", vertexFile);
	char const* vertexSourcePointer = vertexShaderCode.c_str();
	glShaderSource(vertexShaderID, 1, &vertexSourcePointer , NULL);
	glCompileShader(vertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	vector<char> vertexShaderErrorMessage(infoLogLength);
	glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL, &vertexShaderErrorMessage[0]);
	printf("%s", &vertexShaderErrorMessage[0]);

	// Compile Fragment Shader
	printf("Compiling shader : %s", fragmentFile);
	char const* fragmentSourcePointer = fragmentShaderCode.c_str();
	glShaderSource(fragmentShaderID, 1, &fragmentSourcePointer , NULL);
	glCompileShader(fragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	vector<char> fragmentShaderErrorMessage(infoLogLength);
	glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL, &fragmentShaderErrorMessage[0]);
	printf("%s", &fragmentShaderErrorMessage[0]);

	// Link the program
	printf("Linking program");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, vertexShaderID);
	glAttachShader(ProgramID, fragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &infoLogLength);
	vector<char> programErrorMessage(max(infoLogLength, int(1)));
	glGetProgramInfoLog(ProgramID, infoLogLength, NULL, &programErrorMessage[0]);
	printf("%s", &programErrorMessage[0]);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	return ProgramID;
}

// Ultra Fast Square Calc
float ivrsqrt(float number)
{
        long i;
        float x2, y;
        const float threehalfs = 1.5F;

        x2 = number * 0.5F;
        y  = number;
        i  = * (long *) &y;                       // evil floating point bit level hacking
        i  = 0x5f3759df - (i >> 1);               // what the fuck?
        y  = * (float *) &i;
        y  = y * (threehalfs - (x2 * y * y));   // 1st iteration
//      y  = y * (threehalfs - (x2 * y * y));   // 2nd iteration, this can be removed

        return y;
}

bool PewGL::colisionDetection(double xA, double yA, double zA, double radiousA,
                              double xB, double yB, double zB, double radiousB)
{
    double powVal = pow(xA - xB, 2) + pow(yA - yB, 2) + pow(zA - zB, 2);
    //printf("Pow Sum = %f", powVal);
    if(powVal == INFINITY)
        return false;
    double distance = 1/ivrsqrt(powVal);
    #ifdef SHOW_COLISION_BOUNDS
    glDisable(GL_TEXTURE_2D);
    if(distance < radiousA + radiousB){
        glPushMatrix();
            glUseProgram(0);
            glColor4d(1.0, 0.0, 0.0, 1.0);
            glTranslated(xA, yA, zA);
            glutWireSphere(radiousA + 1, 8, 8);
        glPopMatrix();
        glPushMatrix();
            glUseProgram(0);
            glColor4d(1.0, 0.0, 0.0, 1.0);
            glTranslated(xB, yB, zB);
            glutWireSphere(radiousB + 1, 8, 8);
            glColor4d(1.0, 1.0, 1.0, 1.0);
        glPopMatrix();
    //}
    } else {
        glPushMatrix();
            glUseProgram(0);
            glColor4d(0.0, 1.0, 0.0, 1.0);
            glTranslated(xA, yA, zA);
            glutWireSphere(radiousA, 8, 8);
        glPopMatrix();
        glPushMatrix();
            glColor4d(0.0, 1.0, 0.0, 1.0);
            glTranslated(xB, yB, zB);
            glutWireSphere(radiousB, 8, 8);
            glColor4d(1.0, 1.0, 1.0, 1.0);
        glPopMatrix();
    }
    glEnable(GL_TEXTURE_2D);
    #endif
    return distance < radiousA + radiousB;
}
