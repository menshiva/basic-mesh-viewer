#include "presenter/Presenter.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

constexpr static inline int         WIN_WIDTH = 1280;
constexpr static inline int         WIN_HEIGHT = 720;
constexpr static inline const char *WIN_TITLE = "Basic Mesh Viewer";
constexpr static inline bool        WIN_FULLSCREEN = true;

constexpr static inline ImVec4      BACKGROUND_COLOR = ImVec4(0.36f, 0.6f, 0.65f, 1.0f);

static void OnPresenterCreate(const char *glslVersion, const Presenter &presenter) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    auto &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavNoCaptureKeyboard;

    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    ImGui_ImplGlfw_InitForOpenGL(presenter.getWindow(), true);
    ImGui_ImplOpenGL3_Init(glslVersion);

#ifdef __EMSCRIPTEN__
    // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
    io.IniFilename = nullptr;
#endif
}

static void OnPresenterTick(const Presenter &presenter) {
    auto &io = ImGui::GetIO();

    static bool showDemoWindow = false;
    static bool showAnotherWindow = false;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (showDemoWindow)
        ImGui::ShowDemoWindow(&showDemoWindow);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text."); // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &showDemoWindow); // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &showAnotherWindow);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f); // Edit 1 float using a slider from 0.0f to 1.0f
        // ImGui::ColorEdit3("clear color", (float*) &BACKGROUND_COLOR); // Edit 3 floats representing a color

        if (ImGui::Button("Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
    }

    // 3. Show another simple window.
    if (showAnotherWindow) {
        ImGui::Begin("Another Window", &showAnotherWindow); // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            showAnotherWindow = false;
        ImGui::End();
    }

    ImGui::Render();
    presenter.clear((GLclampf*) &BACKGROUND_COLOR);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

static void OnPresenterDestroy(const Presenter&) {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

int main() {
    Presenter()
        .withOnCreateCallback(OnPresenterCreate)
        .withOnTickCallback(OnPresenterTick)
        .withOnDestroyCallback(OnPresenterDestroy)
        .initGlfwCreateWindowAndLoop(WIN_WIDTH, WIN_HEIGHT, WIN_TITLE, WIN_FULLSCREEN);
    return 0;
}
