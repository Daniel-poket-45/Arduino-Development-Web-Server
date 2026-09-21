#include <iostream>
#include <string>
#include <cstdlib>
#include <SDL3/SDL.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl3.h>
#include <imgui/imgui_impl_sdlrenderer3.h>
#include <stdio.h>

//#

namespace adws{
    void imguiInitSystem();
    void imguiShutdownSystem();
}

int main(int argc, char* argv[]){
    if(!SDL_Init(SDL_INIT_VIDEO)){
        SDL_Log("Erro ao inicializar o SDL: %s", SDL_GetError());
        return 1;
    }

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    if(!SDL_CreateWindowAndRenderer("Arrois e fejaum da SDL3 + ImGui", 640, 480, SDL_WINDOW_RESIZABLE, &window, &renderer)){
        SDL_Log("Erro ao criar janela e renderizador: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    adws::imguiInitSystem();

    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);

    bool running = true;
    SDL_Event event;

    while(running){
        while(SDL_PollEvent(&event)){
            ImGui_ImplSDL3_ProcessEvent(&event);

            if(event.type == SDL_EVENT_QUIT){
                running = false;
            }
        }

        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        // Start the Dear ImGui frame


        
        ImGui::NewFrame();

        ImGui::Begin("Painel de Controle");
        ImGui::Text("Ola, mundo do SDL3 com ImGui e SDL_Renderer!");
        if (ImGui::Button("Clique em mim")) {
            SDL_Log("Botao clicado!");
        }
        ImGui::End();

        ImGui::Render();



        //render and clear the screen
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);

        SDL_RenderPresent(renderer);
    }

    adws::imguiShutdownSystem();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

namespace adws{
    inline void imguiInitSystem(){
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); //(void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        ImGui::StyleColorsDark();
    }

    inline void imguiShutdownSystem(){
        ImGui_ImplSDLRenderer3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
    }
}

