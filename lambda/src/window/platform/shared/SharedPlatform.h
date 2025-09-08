#pragma once

#include "../../window.h"

struct SDL_Window;

namespace lambda {
    class SharedWindow : public IWindow {
    public:
        ~SharedWindow();

        virtual void Init(u32 width, u32 height, const std::string& title, RenderingAPI api) override;
        virtual void Update() override;
        virtual bool GetClosingState() override;

        virtual std::vector<const char*> GetVulkanRequiredInstanceExtensions() override;

        virtual void* CreateVulkanSurface(void* instance) override;
    private:
        SDL_Window* m_Window;
        bool m_CurrentClosingState;
    };
}