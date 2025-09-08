#pragma once

#include "core.h"
#include "window/window.h"

namespace lambda {
	class IRenderer {
	public:
		static std::shared_ptr<IRenderer> CreateRenderer(std::shared_ptr<IWindow> window, RenderingAPI api);
		virtual ~IRenderer() = default;

		virtual void Init(std::shared_ptr<IWindow> window) = 0;
		virtual void PreRender() = 0;
		virtual void PostRender() = 0;
	};
}