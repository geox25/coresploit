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
#include "imgui.h"
#include "../svc/svc.hpp"
#include "../svc/example-svc.hpp"

using std::string, std::vector, std::unordered_map, std::function, std::istream_iterator, std::stringstream, std::transform, std::future, std::function;

namespace boot::window {

    // Definition for PXL-Console Window
    struct Console {
        vector<future<int>> services;

        vector<string>      Items;
        ImGuiTextFilter     Filter;
        bool                AutoScroll;
        bool                ScrollToBottom;
        char                InputBuf[256];

        // Maps string cmd to its corresponding function for execution
        unordered_map<string, function<void(const vector<string>&)>> command_map;

        Console() {
            AutoScroll = true;
            ScrollToBottom = false;

            // Populate string to function of command_map
            PopulateCommandMap();
            makeServices();
        }
        ~Console() {
            // Make sure Items vector is empty
            Items.clear();
            stop_service = true;
        }

        void PopulateCommandMap() {
            command_map["CLEAR"]    = [this](const vector<string>& cmd) { ClearLog(); };
            command_map["HELP"]     = [this](const vector<string>& cmd) { AddLog("Commands: CLEAR, HELP, START"); };
            command_map["START"]    = [this](const vector<string>& cmd) {
                if (cmd.size() < 2) {
                    return;
                }

                // TODO: Use identifier for each service to track when it should start/stop.
                // TODO: This could be done with a map of ID to function
                // TODO: When running START cmd, if match of ID in map is found, call function and map ID to atomic<bool>
                // If start cmd wants to start example service
                /*if (cmd.at(1) == "example") {
                    AddLog("Starting service: " + cmd.at(1));
                    stop_service = false;
                    services.push_back(std::async(std::launch::async, exampleService));
                }*/

                if (service_id_entry.contains(cmd.at(1)) && service_id_status.contains(cmd.at(1))) {
                    AddLog("Starting service: " + cmd.at(1));
                    service_id_status.at(cmd.at(1)) = false;
                    services.push_back(std::async(std::launch::async, service_id_entry.at(cmd.at(1))));
                } else {
                    AddLog("Could not locate service: " + cmd.at(1));
                }
            };
            command_map["STOP"]     = [this](const vector<string>& cmd) {
                if (cmd.size() < 2) {
                    return;
                }

                if (service_id_entry.contains(cmd.at(1)) && service_id_status.contains(cmd.at(1))) {
                    AddLog("Stopping service: " + cmd.at(1));
                    service_id_status.at(cmd.at(1)) = true;
                    // TODO: Remove future from services vector
                } else {
                    AddLog("Could not locate service: " + cmd.at(1));
                }
            };
        }

        // Portable helper(s) stolen from imgui_demo.cpp
        static void Strtrim(char* s)    { char* str_end = s + strlen(s); while (str_end > s && str_end[-1] == ' ') str_end--; *str_end = 0; }

        void AddLog(const string& log) {
            Items.push_back(log);
        }

        void ClearLog() {
            Items.clear();
        }

        void CopyItemsToClipboard() {
            string clipboardBuffer;
            for (const string& item : Items) {
                clipboardBuffer.append(item + '\n');
            }
            ImGui::SetClipboardText(clipboardBuffer.c_str());
            std::cout << clipboardBuffer << std::endl;
        }

        void Draw(const string& title, bool& show_console) {
            ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
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
                bool example = false;
                ImGui::Checkbox("Example", &example);
                ImGui::EndPopup();
            }

            // Options button that opens the popup menu
            if (ImGui::Button("Options"))
                ImGui::OpenPopup("Options");
            ImGui::SameLine();
            Filter.Draw(R"(Filter ("incl,-excl") ("error"))", 180);
            ImGui::Separator();

            // Reserve enough left-over height for 1 separator + 1 input text
            const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
            if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar)) {
                // Insert log messages from util_loq_stack queue to the main Items vector
                while (util_log_stack.size() != 0) {
                    Items.push_back(util_log_stack.front());
                    util_log_stack.pop();
                }

                for (const string& item : Items) {
                    const char* item_cstr = item.c_str();

                    if (!Filter.PassFilter(item_cstr))
                        continue;

                    // TODO: Add support here for colored output

                    ImGui::TextUnformatted(item_cstr);
                }

                // Stolen from imgui_demo.cpp
                // Keep up at the bottom of the scroll region if we were already at the bottom at the beginning of the frame.
                // Using a scrollbar or mouse-wheel will take away from the bottom edge.
                if (ScrollToBottom || (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
                    ImGui::SetScrollHereY(1.0f);
                ScrollToBottom = false;
            }
            ImGui::EndChild();
            ImGui::Separator();

            bool reclaim_focus = false;
            // Flags to configure behavior of input text
            ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
            if (ImGui::InputText("Input", InputBuf, IM_ARRAYSIZE(InputBuf), input_text_flags, &TextEditCallbackStub, (void*)this)) {
                char* s = InputBuf;
                Strtrim(s);
                if (s[0])
                    ExecCommand(string(s));
                strcpy(s, "");
                reclaim_focus = true;
            }

            // Stolen from imgui_demo.cpp
            // Autofocus on window apparition
            ImGui::SetItemDefaultFocus();
            if (reclaim_focus)
                ImGui::SetKeyboardFocusHere(-1); // Autofocus previous widget

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
            return console->TextEditCallback(data);
        }

        int TextEditCallback(ImGuiInputTextCallbackData* data) {
            return 0;
        }
    };

    void ShowConsole(bool& show_console) {
        static Console console;
        console.Draw("PXL Console", show_console);
    }
}