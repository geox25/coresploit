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

namespace boot::window {

    // Definition for PXL-Console Window
    struct Console {
        unordered_map<string, future<int>> futures;

        vector<string>      Items;
        ImGuiTextFilter     Filter;
        bool                AutoScroll;
        bool                ScrollToBottom;
        char                InputBuf[256]{};

        bool verbose = false;

        // Maps string cmd to its corresponding function for execution
        unordered_map<string, function<void(const vector<string>&)>> command_map;

        Console() {
            AutoScroll = true;
            ScrollToBottom = false;

            // Populate string to function of command_map
            PopulateCommandMap();
            makeServices();
            std::thread monitorThread(&boot::window::Console::monitorFutures, this);
            monitorThread.detach(); // detach so it doesn't result in terminate called without an active exception
        }
        ~Console() {
            // Make sure Items vector is empty
            Items.clear();
        }

        void PopulateCommandMap() {
            command_map["CLEAR"]    = [this](const vector<string>& cmd) { ClearLog(); };
            command_map["HELP"]     = [this](const vector<string>& cmd) { AddLog("Commands: CLEAR, HELP, RUN, STOP"); };
            command_map["RUN"]    = [this](const vector<string>& cmd) {
                if (cmd.size() < 2) {
                    return;
                }

                if (requestValidServiceID(cmd.at(1))) {
                    // If service is already running
                    if (!requestServiceStatus(cmd.at(1))) {
                        AddLog("#E [0] <boot.cpp> That service is already running! (Try running it again if you believe it has stopped)");
                        return;
                    }

                    AddLog("Starting service: " + cmd.at(1));
                    requestRunService(cmd.at(1));
                    futures.emplace(cmd.at(1), std::async(std::launch::async, [this, cmd] { return requestService(cmd.at(1))(cmd); }));
                } else {
                    AddLog("Could not locate service: " + cmd.at(1));
                }
            };
            command_map["STOP"]     = [this](const vector<string>& cmd) {
                if (cmd.size() < 2) {
                    return;
                }

                if (requestValidServiceID(cmd.at(1))) {
                    AddLog("Stopping service: " + cmd.at(1));
                    requestStopService(cmd.at(1));
                    futures.erase(cmd.at(1));
                    AddLog("#O [0] <boot.cpp> unordered_map<string, future<int>> futures length: " + std::to_string(futures.size()));
                } else {
                    AddLog("Could not locate service: " + cmd.at(1));
                }
            };
        }

        // Portable helper(s) stolen from imgui_demo.cpp
        static void Strtrim(char* s)    { char* str_end = s + strlen(s); while (str_end > s && str_end[-1] == ' ') str_end--; *str_end = 0; }

        void AddLog(const string& log) {
            // Add msg to log if it isn't other OR if verbose is enabled
            if (!log.starts_with("#O") || verbose)
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
            Filter.Draw(R"(Filter ("incl,-excl") ("error"))", 180);
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

                // Merge log messages from util_log_stack to main Items vector
                while (util_log_stack.size() != 0) {
                    if (!util_log_stack.front().starts_with("#O") || verbose)
                        Items.push_back(util_log_stack.front());
                    util_log_stack.pop();
                }

                for (const string& item : Items) {
                    const char* item_cstr = item.c_str();

                    if (!Filter.PassFilter(item_cstr))
                        continue;

                    ImVec4 msg_color = NORMAL_COLOR;

                    if (item.starts_with("#E")) {
                        msg_color = ERROR_COLOR;
                    } else if (item.starts_with("#W")) {
                        msg_color = WARNING_COLOR;
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

        // This function will be run in a separate thread
        [[noreturn]] void monitorFutures() {
            AddLog("#O [monitorFutures()] <boot.cpp> Futures are now being monitored every 5 seconds in a separate thread");
            while (true) {
                for (auto it = futures.begin(); it != futures.end(); ) {
                    if (it->second.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
                        string service_id = it->first;
                        requestStopService(service_id);
                        it = futures.erase(it);
                        AddLog("#O [monitorFutures()] <boot.cpp> [" + service_id + "] has been erased from futures");
                        AddLog("#O [monitorFutures()] <boot.cpp> Length of futures: " + std::to_string(futures.size()));
                    } else {
                        ++it;
                    }
                }

                // Sleep for a reasonable interval before checking again
                std::this_thread::sleep_for(std::chrono::milliseconds(250));
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