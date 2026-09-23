#include <ADWS/adws.hpp>
#include <iostream>
#include <string>
#include <cstdlib>
#include <stdio.h>
#include <SDL3/SDL.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl3.h>
#include <imgui/imgui_impl_opengl3.h>
#include <emscripten.h>
#include <ADWS/adws.hpp>

namespace adws{
    // Imgui initialization wrapper function
    void imguiInitSystem(){
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        ImGui::StyleColorsDark();
    }

    // Imgui shutdown wrapper function
    void imguiShutdownSystem(){
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
    }
}