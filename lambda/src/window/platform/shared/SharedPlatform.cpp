#include "SharedPlatform.h"
#include "SDL3/SDL_error.h"
#include "SDL3/SDL_video.h"
#include "SDL3/SDL_vulkan.h"
#include "core.h"

#include <SDL3/SDL.h>
#include <cinttypes>

namespace lambda {
    
    void SharedWindow::Init(u32 width, u32 height, const std::string& title, RenderingAPI api) {
        LAMBDA_AUTOTRACE();
        m_CurrentClosingState = false;
        m_InitializedAPI = api;
        
        SDL_Init(SDL_INIT_VIDEO);
        
        unsigned long flag = SDL_WINDOW_RESIZABLE;
        
        switch(api) {
        case RenderingAPI_Vulkan:
            flag = flag | SDL_WINDOW_VULKAN;
            break;
        case RenderingAPI_OpenGL:
            flag = flag | SDL_WINDOW_OPENGL;
            break;
        default:
            LAMBDA_FATAL("lambda::SharedWindow::Init encountered an unrecoverable error. The rendering API given to the SharedWindow does not exist, thus, unsure of what API to give the window.");
            break;
        }
        
        m_Window = SDL_CreateWindow(title.c_str(), width, height, flag);
        
        if(m_Window == nullptr) {
            LAMBDA_FATAL("lambda::SharedWindow::Init has encountered an unrecoverable error. SDL_CreateWindow has returned a null pointer.");
            return;
        }

        if(api == RenderingAPI_OpenGL) {
            if(!SDL_GL_CreateContext(m_Window)) {
                LAMBDA_FATAL("lambda::SharedWindow::Init has encountered an unrecoverable error. SDL returned an error: ", SDL_GetError());
                return;
            }
        }
        
        SDL_ShowWindow(m_Window);
    }
    
    SharedWindow::~SharedWindow() {
        LAMBDA_AUTOTRACE();
        SDL_DestroyWindow(m_Window);
    }

    void SharedWindow::Update() {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_EVENT_QUIT) {
                m_CurrentClosingState = true;
            }
        }
    }

    bool SharedWindow::GetClosingState() {
        return m_CurrentClosingState;
    }

    std::vector<const char*> SharedWindow::GetVulkanRequiredInstanceExtensions() {
        LAMBDA_AUTOTRACE();
        std::vector<const char*> extensions;
        u32 length;
        const char* const* exts = SDL_Vulkan_GetInstanceExtensions(&length);

        for(u32 i = 0; i < length; i++) {
            extensions.push_back(exts[i]);
        }

        return extensions;
    }

    void* SharedWindow::CreateVulkanSurface(void* instance) {
        LAMBDA_AUTOTRACE();

        VkInstance vk_instance = reinterpret_cast<VkInstance>(instance);

        VkSurfaceKHR surface;

        if(!SDL_Vulkan_CreateSurface(m_Window, vk_instance, nullptr, &surface)) {
            LAMBDA_FATAL("lambda::SharedWindow::CreateVulkanSurface has encountered an unrecoverable error. SDL returned an error: ", SDL_GetError());
            return nullptr;
        }

        return reinterpret_cast<void*>(surface);
    }

    void SharedWindow::SwapOpenGLBuffers() {
        if(m_InitializedAPI != RenderingAPI_OpenGL) {
            LAMBDA_ERROR("lambda::SharedWindow::SwapOpenGLBuffers has encountered an error. The initialized API for SharedWindow is not OpenGL, but SwapOpenGLBuffers is being called. This is illegal.");
            return;
        }

        SDL_GL_SwapWindow(m_Window);
    }
}