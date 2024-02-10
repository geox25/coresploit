//
// Created by geo on 1/27/24.
//
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <iostream>
#include <iterator>
#include <sstream>
#include <future>
#include <fstream>
#include <chrono>
#include "imgui.h"
#include "../svc/svc.hpp"
#include "config.hpp"
#include "msg.hpp"

using std::string, std::vector, std::unordered_map, std::function, std::istream_iterator, std::stringstream, std::transform, std::future, std::function;

namespace boot::routine {
    static vector<string> Items;
    static bool verbose = true;

    void AddLog(const string& log) {
        // Add msg to log if it isn't other OR if verbose is enabled
        if (!log.starts_with("#O") || verbose)
            Items.push_back(log);
    }

    static void ClearLog() {
        Items.clear();
    }

    static void CopyItemsToClipboard() {
        string clipboardBuffer;
        for (const string& item : Items) {
            clipboardBuffer.append(item + '\n');
        }
        ImGui::SetClipboardText(clipboardBuffer.c_str());
        std::cout << clipboardBuffer << std::endl;
    }

    void runRoutineAsProgram(const vector<string>& cmd) {
        // If PROGRAM is already running,
        // then automatically stop it first.
        if ((cmd.at(1).find(".svc") == std::string::npos) && !requestRoutineStatus(cmd.at(1))) {
            requestStopRoutine(cmd.at(1));
            AddLog("#O [0] <boot.cpp>: Automatically stopped (program behavior): " + cmd.at(1));
        }

        // Now the program can be started again
        AddLog("#O [0] <boot.cpp>: Starting: " + cmd.at(1));
        requestRunRoutine(cmd);
    }

    void runRoutineAsService(const vector<string>& cmd) {
        // If SERVICE is already running,
        // then log an O-type message and return.
        if ((cmd.at(1).find(".prg") == std::string::npos) && !requestRoutineStatus(cmd.at(1))) {
            AddLog("#E [0] <boot.cpp>: That service is already running! (Try running it again if you believe it has stopped)");
            return;
        }

        // Otherwise, start the service
        AddLog("#O [0] <boot.cpp>: Starting: " + cmd.at(1));
        requestRunRoutine(cmd);
    }
}

using boot::routine::AddLog, boot::routine::ClearLog, boot::routine::CopyItemsToClipboard, boot::routine::runRoutineAsProgram, boot::routine::runRoutineAsService, boot::routine::verbose;

namespace boot::window {

    // Definition for PXL-Console Window
    struct Console {

        ImGuiTextFilter     filter;
        bool                auto_scroll;
        bool                scroll_to_bottom;
        char                input_buf[256]{};

        // Maps string cmd to its corresponding function for execution
        unordered_map<string, function<void(const vector<string>&)>> command_map;

        Console() {
            auto_scroll = true;
            scroll_to_bottom = false;

            // Populate string to function of command_map
            PopulateCommandMap();
            makeRoutines();
            start_monitor_futures();
        }
        ~Console() {
            // Make sure Items vector is empty
            routine::Items.clear();
        }

        void PopulateCommandMap() {
            command_map["CLEAR"]    = [this](const vector<string>& cmd) { ClearLog(); };
            command_map["HELP"]     = [this](const vector<string>& cmd) { AddLog("Commands: CLEAR, HELP, RUN, STOP"); };
            command_map["RUN"]    = [this](const vector<string>& cmd) {
                if (cmd.size() < 2) {
                    return;
                }

                if (requestValidRoutineID(cmd.at(1))) {
                    // If SERVICE is already running
                    if (!requestRoutineStatus(cmd.at(1))) {
                        AddLog("#E [0] <boot.cpp>: That service is already running! (Try running it again if you believe it has stopped)");
                        return;
                    }

                    // If PROGRAM is already running
                    if ((cmd.at(1).find(".svc") == std::string::npos) && !requestRoutineStatus(cmd.at(1))) {
                        requestStopRoutine(cmd.at(1));
                        AddLog("#O [0] <boot.cpp>: Automatically stopped (program behavior): " + cmd.at(1));
                    }

                    AddLog("#O [0] <boot.cpp>: Starting: " + cmd.at(1));
                    requestRunRoutine(cmd);
                } else {
                    AddLog("#E [0] <boot.cpp>: Could not locate service: " + cmd.at(1));
                }
            };
            command_map["STOP"]     = [this](const vector<string>& cmd) {
                if (cmd.size() < 2) {
                    return;
                }

                if (requestValidRoutineID(cmd.at(1))) {
                    AddLog("Stopping service: " + cmd.at(1));
                    requestStopRoutine(cmd.at(1));
                } else {
                    AddLog("#E [0] <boot.cpp>: Could not locate service: " + cmd.at(1));
                }
            };
            command_map["ACTIVE"] = [this](const vector<string>& cmd) {
                show_active_services();
            };

            /*
             * Command Map Bindings
             * (Binds a program or service to a command for ease of use)
             */
            command_map["USTAT"] = [this](const vector<string>& cmd) {
                const vector<string> args = {"run", cmd.at(0)};
                runRoutineAsProgram(args);
            };
        }

        // Portable helper(s) stolen from imgui_demo.cpp
        static void Strtrim(char* s)    { char* str_end = s + strlen(s); while (str_end > s && str_end[-1] == ' ') str_end--; *str_end = 0; }

        void Draw(const string& title, bool& show_console) {
            ImGui::SetNextWindowSize(DEFAULT_WIN_CONSOLE_SIZE);

            if (!ImGui::Begin(title.c_str(), &show_console)) {
                ImGui::End();
                return;
            }

            if (ImGui::BeginPopupContextItem()) {
                if (ImGui::MenuItem("Close Console"))
                    show_console = false;
                ImGui::EndPopup();
            }

            ImGui::TextWrapped("Enter 'HELP' for help.");

            if (ImGui::SmallButton("Clear"))        { ClearLog(); }
            ImGui::SameLine();
            if (ImGui::SmallButton("Copy"))         { CopyItemsToClipboard(); }

            ImGui::Separator();

            // Options popup menu with example checkbox
            if (ImGui::BeginPopup("Options")) {
                ImGui::Checkbox("Verbose", &verbose);
                ImGui::EndPopup();
            }

            // Options button that opens the popup menu
            if (ImGui::Button("Options"))
                ImGui::OpenPopup("Options");
            ImGui::SameLine();
            filter.Draw(R"(Filter ("incl,-excl") ("error"))", 180);
            ImGui::Separator();

            // DEBUG CODE FOR SHOWING WINDOW SIZE
            /*
            ImVec2 size = ImGui::GetWindowSize();
            string winSize = std::to_string(size.x) + "," + std::to_string(size.y);
            Items.push_back(winSize);
             */

            // Reserve enough left-over height for 1 separator + 1 input text
            const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
            if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar)) {

                // Merge log messages from log_system to main Items vector (priority over util)
                while (log_system.size() != 0) {
                    if (!log_system.front().starts_with("#O") || verbose)
                        routine::Items.push_back(log_system.front());
                    log_system.pop();
                }

                // Merge log messages from log_util to main Items vector
                while (log_util.size() != 0) {
                    if (!log_util.front().starts_with("#O") || verbose)
                        routine::Items.push_back(log_util.front());
                    log_util.pop();
                }

                for (const string& item : routine::Items) {
                    if (!filter.PassFilter(item.c_str()))
                        continue;

                    ImVec4 msg_color = NORMAL_COLOR;

                    string modified_item;
                    int pos = 0;
                    for (const char& ch : item) {
                        if (ch == '\n') {
                            pos = 0;
                        }
                        if (pos == 150) {
                            modified_item.append("\n");
                            pos = 0;
                        }
                        modified_item += ch;
                        pos++;
                    }
                    const char* item_cstr = modified_item.c_str();

                    if (item.starts_with("#E")) {
                        msg_color = ERROR_COLOR;
                    } else if (item.starts_with("#W")) {
                        msg_color = WARNING_COLOR;
                    } else if (item.starts_with("#Security")) {
                        msg_color = SECURITY_WARNING_COLOR;
                    } else if (item.starts_with("#S")) {
                        msg_color = SUCCESS_COLOR;
                    } else if (item.starts_with("#O")) {
                        msg_color = OTHER_COLOR;
                    }

                    ImGui::TextColored(msg_color, "%s", item_cstr);
                }

                // Stolen from imgui_demo.cpp
                // Keep up at the bottom of the scroll region if we were already at the bottom at the beginning of the frame.
                // Using a scrollbar or mouse-wheel will take away from the bottom edge.
                if (scroll_to_bottom || (auto_scroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
                    ImGui::SetScrollHereY(1.0f);
                scroll_to_bottom = false;
            }
            ImGui::EndChild();
            ImGui::Separator();

            // Flags to configure behavior of input text
            ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
            if (ImGui::InputText("Input", input_buf, IM_ARRAYSIZE(input_buf), input_text_flags, &TextEditCallbackStub, (void*)this)) {
                char* s = input_buf;
                Strtrim(s);
                if (s[0])
                    ExecCommand(string(s));
                strcpy(s, "");
                // Fixes losing focus after enter (maintain focus until clicked off)
                ImGui::SetKeyboardFocusHere(-1);
            }

            ImGui::SameLine();
            if (ImGui::Button("INJECT")) {
                // TODO: Injection Functionality
                // TODO: (init.svc, inject.svc, etc)
            }

            ImGui::End();
        }

        void ExecCommand(const string& command) {
            // Do Stuff with command
            std::cout << "Received Command: " << command << std::endl;

            // Create vector of words of cmd
            stringstream ss(command);
            istream_iterator<string> begin(ss);
            istream_iterator<string> end;
            vector<string> cmd_words(begin, end);

            string cmd_arg0 = cmd_words.at(0);
            transform(cmd_arg0.begin(), cmd_arg0.end(), cmd_arg0.begin(), ::toupper);
            auto result = command_map.find(cmd_arg0);
            // If cmd key (arg 0) is found in command_map then call it
            if (result != command_map.end()) {
                result->second(cmd_words);
            }
        }

        static int TextEditCallbackStub(ImGuiInputTextCallbackData* data) {
            auto* console = (Console*)data->UserData;
            return boot::window::Console::TextEditCallback(data);
        }

        static int TextEditCallback(ImGuiInputTextCallbackData* data) {
            return 0;
        }
    };

    void ShowConsole(bool& show_console) {
        static Console console;
        console.Draw("PXL Console", show_console);
    }
}