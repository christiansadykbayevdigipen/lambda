#include "window.h"

#if defined(LAMBDA_PLATFORM_WINDOWS) || defined(LAMBDA_PLATFORM_LINUX) || defined(LAMBDA_PLATFORM_APPLE)
#include "platform/shared/SharedPlatform.h"
#endif

namespace lambda {
    std::shared_ptr<IWindow> IWindow::CreateWindow(u32 width, u32 height, const std::string& title, RenderingAPI api) {
        LAMBDA_AUTOTRACE();
        
        std::shared_ptr<IWindow> window = nullptr;

        #if defined(LAMBDA_PLATFORM_WINDOWS) || defined(LAMBDA_PLATFORM_LINUX) || defined(LAMBDA_PLATFORM_APPLE)
        window = std::make_shared<SharedWindow>();
        #else
        LAMBDA_FATAL("lambda::IWindow::CreateWindow has encountered an unrecoverable error. The platform the window is being initialized under is not supported!");
        return nullptr;
        #endif

        window->Init(width, height, title, api);

        return window;
    }
}