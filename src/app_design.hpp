#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <cstdlib>
#include <stdio.h>

static void glfw_error_callback(int code, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", code, description);
}

template <typename Derived>
class App
{
public:
    App()
    {
        // Setup
        glfwSetErrorCallback(glfw_error_callback);
        if (!glfwInit())
            std::exit(1);

        const char* glsl_version = nullptr;
#if defined(__APPLE__)
        // GL 3.2 + generally GLSL 150
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Required on Mac
#else
        // GL 3.0 + generally GLSL 130
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+
        // only glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // 3.0+ only
#endif

        // Create window with graphics context
        float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor()); // Valid on GLFW 3.3+ only
        window = glfwCreateWindow((int)(1280 * main_scale), (int)(800 * main_scale), "Dear ImGui", nullptr, nullptr);
        glfwMakeContextCurrent(window);
        if (window == nullptr)
            std::exit(1);

        int version = gladLoadGL(glfwGetProcAddress);
        if (version == 0)
            std::exit(1);

        glfwSwapInterval(1); // Enable vsync

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        // ImGui::StyleColorsLight();

        // Setup scaling
        ImGuiStyle& style = ImGui::GetStyle();
        style.ScaleAllSizes(main_scale); // Bake a fixed style scale. (until we have a solution for
                                         // dynamic style scaling, changing this requires resetting
                                         // Style + calling this again)
        style.FontScaleDpi = main_scale; // Set initial font scale. (in docking branch: using
                                         // io.ConfigDpiScaleFonts=true automatically overrides this
                                         // for every window depending on the current monitor)

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);

        // Load Fonts
        // - If fonts are not explicitly loaded, Dear ImGui will select an embedded
        // font: either AddFontDefaultVector() or AddFontDefaultBitmap().
        //   This selection is based on (style.FontSizeBase * style.FontScaleMain *
        //   style.FontScaleDpi) reaching a small threshold.
        // - You can load multiple fonts and use ImGui::PushFont()/PopFont() to
        // select them.
        // - If a file cannot be loaded, AddFont functions will return a nullptr.
        // Please handle those errors in your code (e.g. use an assertion, display
        // an error and quit).
        // - Read 'docs/FONTS.md' for more instructions and details.
        // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use
        // FreeType for higher quality font rendering.
        // - Remember that in C/C++ if you want to include a backslash \ in a string
        // literal you need to write a double backslash \\ !
        // - Our Emscripten build process allows embedding fonts to be accessible at
        // runtime from the "fonts/" folder. See Makefile.emscripten for details.
        // style.FontSizeBase = 20.0f;
        // io.Fonts->AddFontDefaultVector();
        // io.Fonts->AddFontDefaultBitmap();
        // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf");
        // io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf");
        // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf");
        // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf");
        // ImFont* font =
        // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf");
        // IM_ASSERT(font != nullptr);
    }

    ~App()
    {
        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void Run()
    {
        StartUp();

        while (!glfwWindowShouldClose(window))
        {
            // Poll and handle events (inputs, window resize, etc.)
            // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear
            // imgui wants to use your inputs.
            // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main
            // application, or clear/overwrite your copy of the mouse data.
            // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your
            // main application, or clear/overwrite your copy of the keyboard data. Generally you
            // may always pass all inputs to dear imgui, and hide them from your application based
            // on those two flags.
            glfwPollEvents();
            if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
            {
                ImGui_ImplGlfw_Sleep(10);
                continue;
            }

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            Update();

            // Rendering
            ImGui::Render();
            int display_w, display_h;
            glfwGetFramebufferSize(window, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);
            glClearColor(clear_color.x * clear_color.w,
                         clear_color.y * clear_color.w,
                         clear_color.z * clear_color.w,
                         clear_color.w);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
        }
    }

    void Update()
    {
        static_cast<Derived*>(this)->Update();
    }

    void StartUp()
    {
        static_cast<Derived*>(this)->StartUp();
    }
protected:
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
private:
    GLFWwindow* window;
};
