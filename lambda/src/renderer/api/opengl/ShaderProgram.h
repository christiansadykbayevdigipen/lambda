#pragma once

#include "core.h"

#include "renderer/renderer.h"

namespace lambda {
	class ShaderProgram {
	public:
		ShaderProgram();
		~ShaderProgram();
		ShaderProgram(const ShaderProgram& other) = delete;
		ShaderProgram& operator=(const ShaderProgram& other) = delete;

		void AttachShader(const std::string& shaderFilepath, ShaderType type);
		void LinkShader();

		void Use();
		void Unuse();
	private:
		u32 m_ShaderProgramID;

		std::vector<u32> m_LinkedShaderIDs;
	};
}