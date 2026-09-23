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

// Estrutura para guardar os dados principais no loop do Emscripten
struct AppState {
    SDL_Window* window;
    SDL_GLContext gl_context;
    bool running;
};

void main_loop(void* arg){
    AppState* state = static_cast<AppState*>(arg);
    SDL_Event event;

    // Processa os eventos da fila do SDL
    while(SDL_PollEvent(&event)){
        ImGui_ImplSDL3_ProcessEvent(&event);
        if(event.type == SDL_EVENT_QUIT){
            state->running = false;
            emscripten_cancel_main_loop();
        }
    }

    // Início do frame do ImGui
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();


    ImVec2 currentDisplaySize = ImGui::GetIO().DisplaySize;
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(currentDisplaySize, ImGuiCond_Always);

    // Interface gráfica inicial
    ImGui::Begin("Editor de texto", nullptr, ImGuiWindowFlags_NoResize 
        | ImGuiWindowFlags_NoMove 
        | ImGuiWindowFlags_AlwaysAutoResize 
        | ImGuiWindowFlags_NoScrollbar
        | ImGuiWindowFlags_NoCollapse
        | ImGuiWindowFlags_NoSavedSettings
    );

    char textBuffer[1024] = "Teste para texto de 1 kilobyte (1024 bytes) de tamanho. Você pode digitar aqui e o texto será exibido no terminal quando você clicar no botão 'Salvar no TTY :)'.";

    if(ImGui::Button("Salvar no TTY :)")){
        std::cout << "Botao clicado! Exibindo o conteúdo no buffer: " << textBuffer << std::endl;
    }
    ImGui::SameLine();
    if(ImGui::Button("Botão de teste :)")){
        std::cout << "Botao de teste clicado!" << textBuffer << std::endl;
    }

    ImGui::InputTextMultiline(
        "##meu editor",
        textBuffer,
        sizeof(textBuffer),
        ImGui::GetIO().DisplaySize,
        ImGuiInputTextFlags_AllowTabInput
    );

    ImGui::End();

    // Renderização
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    SDL_GL_SwapWindow(state->window);
}

int main(int argc, char* argv[]){
    // Configurar o inicializador geral da biblioteca SDL
    if(!SDL_Init(SDL_INIT_VIDEO)){
        SDL_Log("Erro ao inicializar o SDL: %s", SDL_GetError());
        return 1;
    }

    // Configurar atributos para OpenGL ES 3.0 (WebGL 2)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    
    // Pega dinamicamente a largura e altura da aba do navegador via JS
    int w = EM_ASM_INT({ return window.innerWidth; });
    int h = EM_ASM_INT({ return window.innerHeight; });

    SDL_Window* window = SDL_CreateWindow("Arduino Development Web Server", w, h, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!window) {
        SDL_Log("Erro ao criar janela SDL: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    if (!gl_context) {
        SDL_Log("Erro ao criar contexto OpenGL: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    SDL_GL_MakeCurrent(window, gl_context);

    adws::imguiInitSystem();

    ImGui_ImplSDL3_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init("#version 300 es");

    AppState state = { window, gl_context, true };

    // Inicia o loop perpétuo gerenciado pelo navegador
    emscripten_set_main_loop_arg(main_loop, &state, 0, 1);

    // Nota: O código abaixo do emscripten_set_main_loop_arg geralmente 
    // não é alcançado em execução normal na web, mas mantemos para boas práticas:
    adws::imguiShutdownSystem();
    SDL_GL_DestroyContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}