#include "ShaderProgram.h"
#include "renderer/renderer.h"

#define GLEW_STATIC
#include <GL/glew.h>

#include <fstream>

namespace lambda {
	ShaderProgram::ShaderProgram() {
		m_ShaderProgramID = glCreateProgram();
	}

	ShaderProgram::~ShaderProgram() {
		glDeleteProgram(m_ShaderProgramID);
	}

	void ShaderProgram::AttachShader(const std::string& shaderFilepath, ShaderType type) {
		u32 openGLShaderType = -1;

		switch(type) {
		case ShaderType_Vertex:
			openGLShaderType = GL_VERTEX_SHADER;
			break;
		case ShaderType_Pixel:
			openGLShaderType = GL_FRAGMENT_SHADER;
			break;
		default:
			LAMBDA_FATAL("lambda::ShaderProgram::AttachShader has encountered an unrecoverable error. ShaderType is neither pixel or vertex.");
			return;
			break;
		}

		u32 shader = glCreateShader(openGLShaderType);

		std::ifstream file(shaderFilepath);
		if(!file.is_open()) {
			LAMBDA_FATAL("lambda::ShaderProgram::AttachShader has encountered an unrecoverable error. The filename given does not exist.");
			return;
		}

		std::string line;

		std::string fullShaderSource;

		while(std::getline(file, line)) {
			fullShaderSource += line + "\n";
		}

		const char* shaderSource = fullShaderSource.c_str();

		glShaderSource(shader, 1, &shaderSource, nullptr);

		glCompileShader(shader);

		s32 compiledStatus = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiledStatus);
		if(compiledStatus != GL_TRUE) {
			s32 logLength = 0;
			char message[1024];
			glGetShaderInfoLog(shader, 1024, &logLength, message);
			
			LAMBDA_ERROR("lambda::ShaderProgram::AttachShader has encountered an error. The shader failed to compile. The compilation error message is: ", message);
			return;
		}

		glAttachShader(m_ShaderProgramID, shader);
		m_LinkedShaderIDs.push_back(shader);
	}

	void ShaderProgram::LinkShader() {
		glLinkProgram(m_ShaderProgramID);

		s32 linkedStatus = 0;
		glGetProgramiv(m_ShaderProgramID, GL_LINK_STATUS, &linkedStatus);
		if(linkedStatus != GL_TRUE) {
			s32 logLength = 0;
			char message[1024];
			glGetProgramInfoLog(m_ShaderProgramID, 1024, &logLength, message);
			
			LAMBDA_ERROR("lambda::ShaderProgram::LinkShader has encountered an error. The shader program failed to link. The linking error message is: ", message);
			return;
		}

		for(const u32 id : m_LinkedShaderIDs) {
			glDeleteShader(id);
		}
	}

	void ShaderProgram::Use() {
		glUseProgram(m_ShaderProgramID);
	}

	void ShaderProgram::Unuse() {
		glUseProgram(0);
	}
}