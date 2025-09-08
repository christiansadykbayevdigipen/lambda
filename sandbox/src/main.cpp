#include "application/application.h"
#include "core.h"
#include "lambda.h"
#include "window/window.h"

class Sandbox : public lambda::Application {
private:
    static constexpr lambda::ApplicationSettings AppSettings = {1280, 720, "Sandbox", lambda::RenderingAPI_Vulkan};
public:
    Sandbox() : lambda::Application(AppSettings) {
        
    }
};

int main() {
    Sandbox s;
    s.Run();
}