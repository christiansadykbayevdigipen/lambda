#include "application/application.h"
#include "core.h"
#include "lambda.h"
#include "window/window.h"

class Sandbox : public lambda::Application {
private:
    static constexpr lambda::ApplicationSettings AppSettings = {1280, 720, "Sandbox", lambda::RenderingAPI_OpenGL};
    lambda::Mesh m_Mesh;
    lambda::ShaderUnit m_Shader;
public:
    Sandbox() : lambda::Application(AppSettings) {
        std::vector<f32> vertices = 
        {
                -0.5f, -0.5f, 0.0f,
                0.5f, -0.5f, 0.0f,
                0.0f,  0.5f, 0.0f
        };

        m_Mesh = m_Renderer->CreateMesh(vertices);
        m_Shader = m_Renderer->CreateShader("res/common/2D_Basic_Unlighted/vertex.glsl", "res/common/2D_Basic_Unlighted/pixel.glsl");
    }

    virtual void OnRender() {
        m_Renderer->Render(m_Mesh, m_Shader);
    }
};

int main() {
    Sandbox s;
    s.Run();
}