#pragma once

#include "core.h"
#include <vector>

namespace lambda {
    // If OpenGL is given to the window as an API, it will own the OpenGL context itself, and will be responsible for swapping the buffers in IWindow::SwapOpenGLBuffers.
    // IWindow::SwapOpenGLBuffers will throw an error if the Window is not initialized with OpenGL
    class IWindow {
    public:
        static std::shared_ptr<IWindow> CreateWindow(u32 width, u32 height, const std::string& title, RenderingAPI api);

        virtual ~IWindow() = default;

        virtual void Init(u32 width, u32 height, const std::string& title, RenderingAPI api) = 0;
        virtual void Update() = 0;
        virtual bool GetClosingState() = 0;

        virtual std::vector<const char*> GetVulkanRequiredInstanceExtensions() = 0;

        virtual void SwapOpenGLBuffers() = 0;

        // Returns a VkSurfaceKHR object (casted to a void*)
        // Takes in a VkInstance object (casted to a void*)
        // We use this void* nature to keep the platform and API independence.
        virtual void* CreateVulkanSurface(void* instance) = 0;
    };
}