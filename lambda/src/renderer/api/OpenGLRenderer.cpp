#include "OpenGLRenderer.h"
#include "renderer/renderer.h"

#define GLEW_STATIC
#include <GL/glew.h>

namespace lambda {
	void OpenGLRenderer::Init(std::shared_ptr<IWindow> window) {
		LAMBDA_AUTOTRACE();

		m_Window = window;
		m_CurrentMeshIndex = m_CurrentShaderIndex = 0;

		if(glewInit() != GLEW_OK) {
			LAMBDA_FATAL("lambda::OpenGLRenderer::Init has encountered an unrecoverable error. glewInit() has not returned GLEW_OK.");
			return;
		}
	}

	void OpenGLRenderer::PreRender() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRenderer::PostRender() {
		m_Window->SwapOpenGLBuffers();
	}

	OpenGLRenderer::~OpenGLRenderer() {
		LAMBDA_AUTOTRACE();
	}

	Mesh OpenGLRenderer::CreateMesh(std::vector<f32> vertexCoordinates) {
		Mesh mesh = m_CurrentMeshIndex;

		// Initialization goes here
		OpenGLMeshData data = {};

		data.VertexCount = vertexCoordinates.size()/3;

		std::vector<VertexArrayIndex> vertexArrayIndices = {
			{3, 0, 0}
		};

		std::shared_ptr<VertexBuffer> buffer = std::make_shared<VertexBuffer>(vertexCoordinates);

		data.VertexArrayObject = std::make_shared<VertexArray>();

		data.VertexArrayObject->AddVertexBuffer(buffer, vertexArrayIndices);
		
		m_MeshDatas.insert(std::make_pair(mesh, data));

		m_CurrentMeshIndex++;
	
		return mesh;
	}

	ShaderUnit OpenGLRenderer::CreateShader(const std::string& vertexShader, const std::string& pixelShader) {
		ShaderUnit shader = m_CurrentShaderIndex;

		// Initialization goes here
		OpenGLShaderData data = {};
		data.MainShaderProgram = std::make_shared<ShaderProgram>();
		data.MainShaderProgram->AttachShader(vertexShader, ShaderType_Vertex);
		data.MainShaderProgram->AttachShader(pixelShader, ShaderType_Pixel);
		data.MainShaderProgram->LinkShader();

		m_ShaderDatas.insert(std::make_pair(shader, data));

		m_CurrentShaderIndex++;
	
		return shader;
	}

	void OpenGLRenderer::DestroyMesh(Mesh mesh) {
		m_MeshDatas.erase(mesh);
	}

	void OpenGLRenderer::DestroyShader(ShaderUnit shader) {
		m_ShaderDatas.erase(shader);
	}

	void OpenGLRenderer::Render(Mesh mesh, ShaderUnit shader) {
		const OpenGLMeshData& meshData = m_MeshDatas.at(mesh);
		const OpenGLShaderData& shaderData = m_ShaderDatas.at(shader); 

		shaderData.MainShaderProgram->Use();

		meshData.VertexArrayObject->Draw(meshData.VertexCount);
		
		shaderData.MainShaderProgram->Unuse();
	}
}