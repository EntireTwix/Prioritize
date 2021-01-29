#include <fstream>
#include "task.hpp"
#include "enums.hpp"
#include "imgui.h"
#include "backends/imgui_impl_glut.h"
#include "backends/imgui_impl_opengl2.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <GL/freeglut.h>
#elif __linux__
#include <GL/freeglut.h>
#else
#error "Unsupported Compiler"
#endif

#ifdef _MSC_VER
#pragma warning(disable : 4505) // unreferenced local function has been removed
#endif

template <typename T>
bool Save(const std::string &location, const std::vector<T> &dest)
{
    std::ofstream output(location);
    if (output.is_open())
    {
        output << (json(dest).dump());
    }
    else
    {
        return false;
    }
    output.close();
    return true;
}

template <typename T>
bool Load(const std::string &location, std::vector<T> &dest)
{
    std::ifstream input(location);
    json temp;
    std::string final_str, temp_str;
    if (input.is_open())
    {
        //entirely for old versions that werent one line
        while (input >> temp_str)
        {
            final_str += temp_str;
        }
        temp = json::parse(final_str);
        input.close();
    }
    else
    {
        return false;
    }

    dest.resize(temp.size());
    for (size_t i = 0; i < temp.size(); ++i)
    {
        dest[i] = (T)temp[i];
    }
    return true;
}

//static bool show_demo_window = true;

static const ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
static constexpr ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar;
static constexpr ImGuiWindowFlags default_flags = ImGuiWindowFlags_NoCollapse;
static constexpr ImGuiTableFlags table_settings = ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_BordersOuterV | ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_BordersOuterH;
static bool save_active = false;
static bool open_active = false;
static bool task_active = false;
static bool val_active = false;

void my_display_code()
{
    //dashboard
    ImGui::Begin("##Dashboard", nullptr, window_flags);
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open"))
            {
                open_active = true;
            }
            if (ImGui::MenuItem("Save"))
            {
                save_active = true;
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Tasks"))
            {
                task_active = true;
            }
            if (ImGui::MenuItem("Values"))
            {
                val_active = true;
            }
            if (ImGui::MenuItem("Enums"))
            {
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
    if (ImGui::BeginTable("##table1", 2, table_settings))
    {
        ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Score", ImGuiTableColumnFlags_WidthFixed);
        UpdateScores();
        for (Task &t : task_buffer)
        {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            if (ImGui::Checkbox(t.name.c_str(), &t.state)) //if there is a change
            {
                change_flag = true;
            }
            ImGui::TableSetColumnIndex(1);
            if (!t.state)
            {
                ImGui::TextColored(ImVec4(t.score, (1 - t.score), 0.0f, 1.0f), "%d%%", (int)(t.score * 100));
            }
            else
            {
                ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Done!");
            }
        }
        ImGui::EndTable();
    }
    ImGui::End();

    //saving
    if (save_active)
    {
        static char save_locationbff[128] = "";
        static bool error_flag = false;

        ImGui::Begin("Save Window", &save_active, default_flags);
        ImGui::InputTextWithHint("folder path", "ex: \"homework\"", save_locationbff, IM_ARRAYSIZE(save_locationbff));
        if (ImGui::Button("Save"))
        {
            if (Save(std::string(save_locationbff) + "/tasks.json", task_buffer), Save(std::string(save_locationbff) + "/values.json", values))
            {
                save_active = false;
            }
            else
            {
                error_flag = true;
            }
        }
        if (error_flag)
        {
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "cannot find directory");
        }
        ImGui::End();
    }

    //opening
    if (open_active)
    {
        static char open_locationbff[128] = "";
        static bool error_flag = false;

        ImGui::Begin("Open Window", &open_active, default_flags); //reuses flags
        ImGui::InputTextWithHint("folder path", "ex: \"homework\"", open_locationbff, IM_ARRAYSIZE(open_locationbff));
        if (ImGui::Button("Open"))
        {
            if (Load(std::string(open_locationbff) + "/values.json", values), Load(std::string(open_locationbff) + "/tasks.json", task_buffer))
            {
                open_active = false;
            }
            else
            {
                error_flag = true;
            }
        }
        if (error_flag)
        {
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "cannot find directory");
        }
        ImGui::End();
    }

    //edit task
    if (task_active)
    {
        static const char *elems_names[7] = {"VirtuallyNone",
                                             "VeryLow",
                                             "Low",
                                             "Medium",
                                             "High",
                                             "VeryHigh",
                                             "ExtremelyHigh"};
        ImGui::Begin("Editing Tasks", &task_active, default_flags);
        static char temp[128] = "";
        ImGui::InputTextWithHint("##task_name", "enter task name", temp, IM_ARRAYSIZE(temp));
        ImGui::SameLine();
        if (ImGui::Button("Add"))
        {
            task_buffer.push_back({temp});
            change_flag = true;
        }
        // static float r, g;
        // ImGui::SliderFloat("red", &r, 0, 5);
        // ImGui::SliderFloat("green", &g, 0, 5);

        if (ImGui::BeginTable("##table1", 1 + values.size(), table_settings))
        {
            ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
            for (const Value &v : values)
            {
                ImGui::TableSetupColumn(v.name.c_str(), ImGuiTableColumnFlags_WidthFixed, 150);
            }
            ImGui::TableHeadersRow();
            UpdateScores();
            for (Task &t : task_buffer)
            {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Checkbox(t.name.c_str(), &t.select);
                for (int i = 0; i < values.size(); ++i)
                {
                    ImGui::TableSetColumnIndex(i + 1);
                    ImGui::SliderInt(("##" + std::to_string(i) + ':' + t.name).c_str(), &t.task_values[i], 0, 6, elems_names[t.task_values[i]]);
                    ImGui::SameLine();
                    ImGui::ColorButton(("##" + std::to_string(i) + ':' + t.name).c_str(), ImVec4((float)(enumFloats[t.task_values[i]] / 140) * 2.5, 1 - (float)(enumFloats[t.task_values[i]] / 140), 0.0f, 1.0f), 0, ImVec2(35, 25));
                }
            }
            ImGui::EndTable();
        }
        if (ImGui::Button("Update"))
        {
            change_flag = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("Delete Selected"))
        {
            for (int i = task_buffer.size() - 1; i >= 0; --i)
            {
                if (task_buffer[i].select)
                {
                    task_buffer.erase(task_buffer.begin() + i);
                    //std::cout << "erasing " << i << '\n'
                    //          << "size is now " << task_buffer.size() << '\n';
                    change_flag = true;
                }
            }
        }
        ImGui::End();
    }

    //edit values
    if (val_active)
    {
        ImGui::Begin("Edit Values", &val_active, default_flags);

        static char temp[128] = "";
        ImGui::InputTextWithHint("##value_name", "enter value name", temp, IM_ARRAYSIZE(temp));

        static float temp_weight = 0;
        ImGui::SliderFloat("weight", &temp_weight, 0, 2, "%.2f");
        if (ImGui::Button("Add"))
        {
            values.push_back({temp, temp_weight});
            change_flag = true;
        }
        ImGui::NewLine();

        for (int i = 0; i < values.size(); ++i)
        {
            ImGui::Checkbox(("##" + std::to_string(i)).c_str(), &values[i].select);
            ImGui::SameLine();
            if (ImGui::SliderFloat(values[i].name.c_str(), &values[i].weight, 0, 2, "%.2f"))
            {
                change_flag = true;
            }
        }
        if (ImGui::Button("Delete Selected"))
        {
            for (int i = values.size() - 1; i >= 0; --i)
            {
                if (values[i].select)
                {
                    values.erase(values.begin() + i);
                    for (Task &t : task_buffer)
                    {
                        t.task_values.erase(t.task_values.begin() + i); //removing refs
                    }
                    change_flag = true;
                }
            }
        }
        ImGui::End();
    }

    // if (show_demo_window)
    //     ImGui::ShowDemoWindow(&show_demo_window);
}

void glut_display_func()
{

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGLUT_NewFrame();

    my_display_code();

    // Rendering
    ImGui::Render();
    ImGuiIO &io = ImGui::GetIO();
    glViewport(0, 0, (GLsizei)io.DisplaySize.x, (GLsizei)io.DisplaySize.y);
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    //glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound, but prefer using the GL3+ code.
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

    glutSwapBuffers();
    glutPostRedisplay();
}

// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.

int main(int argc, char **argv)
{

    // values.push_back({"Urgency", 1});
    // values.push_back({"Time", 1});
    // values.push_back({"Points", 1});
    // Create GLUT window
    glutInit(&argc, argv);
#ifdef __FREEGLUT_EXT_H__
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
#endif
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_MULTISAMPLE);
    glutInitWindowSize(1280, 720);
    glutCreateWindow("PrioritizationGUI");

    // Setup GLUT display function
    // We will also call ImGui_ImplGLUT_InstallFuncs() to get all the other functions installed for us,
    // otherwise it is possible to install our own functions and call the imgui_impl_glut.h functions ourselves.
    glutDisplayFunc(glut_display_func);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGLUT_Init();
    ImGui_ImplGLUT_InstallFuncs();
    ImGui_ImplOpenGL2_Init();

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    glutMainLoop();

    // Cleanup
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGLUT_Shutdown();
    ImGui::DestroyContext();

    return 0;
}
//0->2 float slider property weight;
//0->25 int slider per value;