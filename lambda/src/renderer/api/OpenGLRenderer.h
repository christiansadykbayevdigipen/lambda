#pragma once

#include "core.h"
#include "renderer/renderer.h"

#include <optional>
#include <unordered_map>

#include "opengl/VertexArray.h"
#include "opengl/ShaderProgram.h"

namespace lambda {
	struct OpenGLMeshData {
		std::shared_ptr<VertexArray> VertexArrayObject;
		u32 VertexCount;
	};

	struct OpenGLShaderData {
		std::shared_ptr<ShaderProgram> MainShaderProgram;
	};

	class OpenGLRenderer : public IRenderer {
	public:
		virtual void Init(std::shared_ptr<IWindow> window) override;
		virtual void PreRender() override;
		virtual void PostRender() override;

		~OpenGLRenderer();

		virtual Mesh CreateMesh(std::vector<f32> vertexCoordinates) override;
		virtual ShaderUnit CreateShader(const std::string& vertexShader, const std::string& pixelShader) override;

		virtual void DestroyMesh(Mesh mesh) override;
		virtual void DestroyShader(ShaderUnit shader) override;

		virtual void Render(Mesh mesh, ShaderUnit shader) override;
	private:
		u32 m_CurrentMeshIndex, m_CurrentShaderIndex;

		std::shared_ptr<IWindow> m_Window;
		std::unordered_map<Mesh, OpenGLMeshData> m_MeshDatas;
		std::unordered_map<ShaderUnit, OpenGLShaderData> m_ShaderDatas;
	};
}