#include "gradeBook.h"
#include "gradeManager.h"
#include "statistics.h"
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include <algorithm>

void strToBuffer(const std::string& src, char* dst, int size) {
    strncpy_s(dst, size, src.c_str(), size - 1);
}

int main(int argc, char* argv[]) {

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Grade Book System",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1000, 650, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,
        SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsLight();
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    GradeBook  gradeBook;
    GradeManager gradeManager;
    Statistics   stats;

    // --- Input buffers ---
    char idBuf[32]   = "";
    char nameBuf[64] = "";
    char deptBuf[64] = "";
    char subjectBuf[64] = "";
    char gradeBuf[16]   = "";
    char searchIdBuf[32] = "";
    char searchNameBuf[64] = "";
    char searchDeptBuf[64] = "";
    string lastSearchId = "";
    string lastSearchName = "";
    string lastSearchDept = "";
    bool searchIdClicked = false;
    bool searchNameClicked = false;
    bool searchDeptClicked = false;

    std::string statusMsg = ""; // feedback message shown to user

    // Main Loop
    bool running = true;
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            ImGui_ImplSDL2_ProcessEvent(&e);
            if (e.type == SDL_QUIT) running = false;
        }

        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // full screen window
        ImGui::SetNextWindowPos({0, 0});
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        ImGui::Begin("Grade Book", nullptr,
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

        ImGui::Text("GRADE BOOK SYSTEM");
        ImGui::Separator();

        // tab bar
        if (ImGui::BeginTabBar("Tabs")) {

            // Add Student tab
            if (ImGui::BeginTabItem("Add Student")) {
                ImGui::InputText("Student ID",   idBuf,   sizeof(idBuf));
                ImGui::InputText("Name",         nameBuf, sizeof(nameBuf));
                ImGui::InputText("Department",   deptBuf, sizeof(deptBuf));

                if (ImGui::Button("Add Student")) {
                    if (gradeBook.isExist(idBuf)) {
                        statusMsg = "Error: Student ID already exists!";
                    } else {
                        gradeBook.addStudent(idBuf, nameBuf, deptBuf);
                        statusMsg = "Student added successfully!";
                        idBuf[0] = nameBuf[0] = deptBuf[0] = '\0'; // clear fields
                    }
                }
                ImGui::EndTabItem();
            }

            // Add / Update Grade tab
            if (ImGui::BeginTabItem("Grades")) {
                ImGui::InputText("Student ID##g", idBuf, sizeof(idBuf));
                ImGui::InputText("Subject",       subjectBuf, sizeof(subjectBuf));
                ImGui::InputText("Grade (0-100)", gradeBuf,   sizeof(gradeBuf));

                if (ImGui::Button("Add Grade")) {
                    if (!gradeBook.isExist(idBuf)) {
                        statusMsg = "Error: Student not found!";
                    } else {
                        double g = atof(gradeBuf);
                        Student& s = gradeBook.getStudent(idBuf);
                        gradeManager.addGrade(s, subjectBuf, g);
                        statusMsg = "Grade added!";
                    }
                }
                ImGui::SameLine();
                if (ImGui::Button("Update Grade")) {
                    if (!gradeBook.isExist(idBuf)) {
                        statusMsg = "Error: Student not found!";
                    } else {
                        double g = atof(gradeBuf);
                        Student& s = gradeBook.getStudent(idBuf);
                        gradeManager.updateGrade(s, subjectBuf, g);
                        statusMsg = "Grade updated!";
                    }
                }
                ImGui::SameLine();
                if (ImGui::Button("Remove Subject")) {
                    if (!gradeBook.isExist(idBuf)) {
                        statusMsg = "Error: Student not found!";
                    } else {
                        Student& s = gradeBook.getStudent(idBuf);
                        gradeManager.removeSubject(s, subjectBuf);
                        statusMsg = "Subject removed!";
                    }
                }
                ImGui::EndTabItem();
            }

            // Search / View Student tab
            if (ImGui::BeginTabItem("Search")) {

            // Search by ID
            ImGui::InputText("Search by ID", searchIdBuf, sizeof(searchIdBuf));
            if (ImGui::Button("Search##id")) {
                lastSearchId = searchIdBuf;
            }
            if (!lastSearchId.empty()) {
                if (gradeBook.isExist(lastSearchId)) {
                    Student& s = gradeBook.getStudent(lastSearchId);
                    ImGui::Separator();
                    ImGui::Text("ID: %s", s.id.c_str());
                    ImGui::Text("Name: %s", s.name.c_str());
                    ImGui::Text("Department: %s", s.department.c_str());
                    ImGui::Text("GPA: %.2f  (%s)", s.getGPA(), s.gpaToLetter().c_str());
                    ImGui::Separator();
                    ImGui::Text("Subjects:");
                    for (auto& subj : s.subjects) {
                        ImGui::Text("  %-20s  %.1f  (%s)",
                            subj.name.c_str(), subj.grade,
                            subj.gradeToLetter().c_str());
                    }
                } else {
                    ImGui::Text("Student not found!");
                }
            }

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            // Search by Name
            ImGui::InputText("Search by Name", searchNameBuf, sizeof(searchNameBuf));
            if (ImGui::Button("Search##name")) {
                lastSearchName = searchNameBuf;
            }
            if (!lastSearchName.empty()) {
                auto results = stats.searchByName(gradeBook.getAllStudents(), lastSearchName);
                if (results.empty()) {
                    ImGui::Text("No students found with this name!");
                } else {
                    for (auto& s : results) {
                        ImGui::Text("  %s | %s | %s | GPA: %.2f",
                            s.id.c_str(), s.name.c_str(),
                            s.department.c_str(), s.getGPA());
                    }
                }
            }

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            // Search by Department
            ImGui::InputText("Search by Department", searchDeptBuf, sizeof(searchDeptBuf));
            if (ImGui::Button("Search##dept")) {
                lastSearchDept = searchDeptBuf;
            }
            if (!lastSearchDept.empty()) {
                auto results = stats.searchByDepartment(gradeBook.getAllStudents(), lastSearchDept);
                if (results.empty()) {
                    ImGui::Text("No students found in this department!");
                } else {
                    for (auto& s : results) {
                        ImGui::Text("  %s | %s | %s | GPA: %.2f",
                            s.id.c_str(), s.name.c_str(),
                            s.department.c_str(), s.getGPA());
                    }
                }
            }

            ImGui::EndTabItem();
        }

            // All Students tab
            // --- Load from file ---
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            static char fileBuf[256] = "students.csv";
            ImGui::InputText("File Path", fileBuf, sizeof(fileBuf));
            if (ImGui::Button("Load from File")) {
                bool success = stats.loadFromFile(fileBuf, gradeBook, gradeManager);
                if (success)
                    statusMsg = "File loaded successfully!";
                else
                    statusMsg = "Error: Could not open file!";
            }
            ImGui::SameLine();
            if (ImGui::Button("Save to File")) {
                stats.saveToFile(fileBuf, gradeBook);
                statusMsg = "File saved successfully!";
            }
            if (ImGui::BeginTabItem("All Students")) {
                static int sortMode = 0;
                ImGui::RadioButton("Sort by GPA",  &sortMode, 0); ImGui::SameLine();
                ImGui::RadioButton("Sort by Name", &sortMode, 1); ImGui::SameLine();
                ImGui::RadioButton("Sort by ID",   &sortMode, 2);

                auto allStudents = gradeBook.getAllStudents();

                // Sort
                if (sortMode == 0)
                    sort(allStudents.begin(), allStudents.end(),
                        [](Student& a, Student& b){ return a.getGPA() > b.getGPA(); });
                else if (sortMode == 1)
                    sort(allStudents.begin(), allStudents.end(),
                        [](Student& a, Student& b){ return a.name < b.name; });
                else
                    sort(allStudents.begin(), allStudents.end(),
                        [](Student& a, Student& b){ return a.id < b.id; });

                // Table
                if (ImGui::BeginTable("StudentsTable", 5,
                    ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg |
                    ImGuiTableFlags_ScrollY, {0, 400})) {

                    ImGui::TableSetupColumn("ID");
                    ImGui::TableSetupColumn("Name");
                    ImGui::TableSetupColumn("Department");
                    ImGui::TableSetupColumn("GPA");
                    ImGui::TableSetupColumn("Grade");
                    ImGui::TableHeadersRow();

                    for (auto& s : allStudents) {
                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0); ImGui::Text("%s", s.id.c_str());
                        ImGui::TableSetColumnIndex(1); ImGui::Text("%s", s.name.c_str());
                        ImGui::TableSetColumnIndex(2); ImGui::Text("%s", s.department.c_str());
                        ImGui::TableSetColumnIndex(3); ImGui::Text("%.2f", s.getGPA());
                        ImGui::TableSetColumnIndex(4); ImGui::Text("%s", s.gpaToLetter().c_str());
                    }
                    ImGui::EndTable();
                }

                // Delete
                ImGui::InputText("Delete by ID", idBuf, sizeof(idBuf));
                if (ImGui::Button("Delete Student")) {
                    gradeBook.deleteStudent(idBuf);
                    statusMsg = "Student deleted.";
                }
                ImGui::EndTabItem();
            }

            // Statistics tab
            if (ImGui::BeginTabItem("Statistics")) {
                auto allStudents = gradeBook.getAllStudents();
                double avg = stats.getClassAverage(allStudents);
                Student hi = stats.getHigestStudent(allStudents);
                Student lo = stats.getLowestStudent(allStudents);
                auto bands = stats.getGradeBandCount(allStudents);

                ImGui::Text("Total Students : %d", gradeBook.size());
                ImGui::Text("Class Average GPA : %.2f", avg);
                ImGui::Text("Highest GPA : %s (%.2f)", hi.name.c_str(), hi.getGPA());
                ImGui::Text("Lowest  GPA : %s (%.2f)", lo.name.c_str(), lo.getGPA());
                ImGui::Separator();
                ImGui::Text("Grade Distribution:");
                for (auto& [letter, count] : bands)
                    ImGui::Text("  %s : %d students", letter.c_str(), count);

                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

        // Status bar at bottom
        ImGui::Separator();
        ImGui::TextColored({0.4f, 1.0f, 0.4f, 1.0f}, "%s", statusMsg.c_str());

        ImGui::End();

        ImGui::Render();
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);
    }

    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}