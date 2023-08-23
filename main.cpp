#include <fstream>
#include "file_io.hpp"
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
#elif __FreeBSD__
#include <GL/glut.h>
#else
#error "Unsupported Compiler"
#endif

#ifdef _MSC_VER
#pragma warning(disable : 4505) // unreferenced local function has been removed
#endif

static const ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
static constexpr ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar;
static constexpr ImGuiWindowFlags default_flags = ImGuiWindowFlags_NoCollapse;
static constexpr ImGuiTableFlags table_settings = ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_BordersOuterV | ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_BordersOuterH;
static bool open_active = false;
static bool save_active = false;
static bool task_active = false;
static bool val_active = false;
static bool enums_active = false;
static char last_open[128] = "";
static const char *elems_names[] = {"VirtuallyNone",
                                    "VeryLow",
                                    "Low",
                                    "Medium",
                                    "High",
                                    "VeryHigh",
                                    "ExtremelyHigh"};

inline void AutoSave() noexcept
{
    if (strcmp(last_open, ""))
    {
        Save(std::string(last_open) + "/tasks.json", task_buffer) && Save(std::string(last_open) + "/values.json", values) && Save(std::string(last_open) + "/enums.json", enum_floats);
    }
}

inline void my_display_code() noexcept
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
            if (ImGui::MenuItem("Save a COPY"))
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
                enums_active = true;
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
            if (ImGui::Checkbox(t.name, &t.state)) //if there is a change
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

    // //saving
    if (save_active)
    {
        static char save_locationbff[128] = "";
        static bool error_flag = false;

        ImGui::Begin("Save Window", &save_active, default_flags);
        ImGui::InputTextWithHint("folder path", "ex: \"homework\"", save_locationbff, IM_ARRAYSIZE(save_locationbff));
        if (ImGui::Button("Copy"))
        {
            if (Save(std::string(save_locationbff) + "/tasks.json", task_buffer) && Save(std::string(save_locationbff) + "/values.json", values) && Save(std::string(save_locationbff) + "/enums.json", enum_floats))
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
            if (task_buffer.size() || values.size())
            {
                AutoSave();
            }
            if (Load(std::string(open_locationbff) + "/values.json", values) && Load(std::string(open_locationbff) + "/tasks.json", task_buffer) && Load(std::string(open_locationbff) + "/enums.json", enum_floats))
            {
                open_active = false;
                change_flag = true;
                std::copy(&open_locationbff[0], &open_locationbff[127], &last_open[0]);
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
        ImGui::Begin("Editing Tasks", &task_active, default_flags);
        static char temp[128] = "";
        ImGui::InputTextWithHint("##task_name", "enter task name", temp, IM_ARRAYSIZE(temp));
        ImGui::SameLine();
        if (ImGui::Button("Add"))
        {
            static Task t;
            std::copy(&temp[0], &temp[127], &t.name[0]);
            task_buffer.push_back({t});
            change_flag = true;
        }

        if (ImGui::BeginTable("##table1", 1 + values.size(), table_settings))
        {
            ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
            for (const Value &v : values)
            {
                ImGui::TableSetupColumn(v.name.c_str(), ImGuiTableColumnFlags_WidthFixed, 150);
            }
            ImGui::TableHeadersRow();
            UpdateScores();
            for (size_t j = 0; j < task_buffer.size(); ++j)
            {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Checkbox((std::string("##") + task_buffer[j].name).c_str(), &task_buffer[j].select);
                ImGui::SameLine();
                ImGui::InputText(("##name of " + std::to_string(j)).c_str(), task_buffer[j].name, 128);
                for (int i = 0; i < values.size(); ++i)
                {
                    ImGui::TableSetColumnIndex(i + 1);
                    ImGui::SliderInt(("##" + std::to_string(i) + ':' + std::to_string(j)).c_str(), &task_buffer[j].task_values[i], 0, 6, elems_names[task_buffer[j].task_values[i]]);
                    ImGui::SameLine();
                    ImGui::ColorButton(("##" + std::to_string(i) + ':' + std::to_string(j)).c_str(), ImVec4(enum_colors[task_buffer[j].task_values[i]].first, enum_colors[task_buffer[j].task_values[i]].second, 0.0f, 1.0f), 0, ImVec2(35, 25));
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
            ImGui::Text(values[i].name.c_str());
            ImGui::Checkbox((std::string("##") + std::to_string(i)).c_str(), &values[i].select);
            ImGui::SameLine();
            if (ImGui::SliderFloat(("##S" + values[i].name).c_str(), &values[i].weight, 0, 2, "%.2f"))
            {
                change_flag = true;
            }
            ImGui::SameLine();
            if (ImGui::InputFloat(("##I" + values[i].name).c_str(), &values[i].weight, 0, 2, "%.2f"))
            {
                change_flag = true;
            }
            ImGui::NewLine();
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

    if (enums_active)
    {
        ImGui::Begin("Edit Enums", &enums_active, default_flags);
        ImGui::PlotLines("##Enums Ploted", enum_floats.data(), enum_floats.size());
        for (int i = 0; i < IM_ARRAYSIZE(elems_names); ++i)
        {
            if (ImGui::VSliderFloat((std::string("##") + std::to_string(i)).c_str(), ImVec2(21, 256), &enum_floats[i], 0, 256))
            {
                UpdateColors();
                change_flag = true;
            }
            ImGui::SameLine();
        }
        ImGui::End();
    }
}

inline void glut_display_func() noexcept
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

int main(int argc, char **argv)
{
    UpdateColors();
    glutInit(&argc, argv);
#ifdef __FREEGLUT_EXT_H__
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
#endif
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_MULTISAMPLE);
    glutInitWindowSize(1280, 720);
    glutCreateWindow("PrioritizationGUI");
    glutDisplayFunc(glut_display_func);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    ImGui::StyleColorsDark();

    ImGui_ImplGLUT_Init();
    ImGui_ImplGLUT_InstallFuncs();
    ImGui_ImplOpenGL2_Init();

    glutMainLoop();

    // Cleanup
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGLUT_Shutdown();
    ImGui::DestroyContext();

    // Saving
    AutoSave();

    return 0;
}