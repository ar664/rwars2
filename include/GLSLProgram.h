#ifndef	__GLSLPROGRAM_H
#define __GLSLPROGRAM_H

class GLSLProgram
{
public:
	GLSLProgram();
	~GLSLProgram();

	void CompileShaders(const char* vertexShaderFilePath,const char* fragmentShaderFilePath);
	void LinkShaders();

private:

};


#endif