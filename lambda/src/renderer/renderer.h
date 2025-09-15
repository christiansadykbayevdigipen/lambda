#pragma once

#include "core.h"
#include "window/window.h"

namespace lambda {
	using Mesh = u64;
	using ShaderUnit = u64;

	enum ShaderType {
		ShaderType_Vertex,
		ShaderType_Pixel
	};

	class IRenderer {
	public:
		static std::shared_ptr<IRenderer> CreateRenderer(std::shared_ptr<IWindow> window, RenderingAPI api);
		virtual ~IRenderer() = default;

		virtual void Init(std::shared_ptr<IWindow> window) = 0;
		virtual void PreRender() = 0;
		virtual void PostRender() = 0;

		virtual Mesh CreateMesh(std::vector<f32> vertexCoordinates) = 0;
		virtual ShaderUnit CreateShader(const std::string& vertexShader, const std::string& pixelShader) = 0;

		virtual void DestroyMesh(Mesh mesh) = 0;
		virtual void DestroyShader(ShaderUnit shader) = 0;

		virtual void Render(Mesh mesh, ShaderUnit shader) = 0;
	};
}