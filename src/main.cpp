#include <iostream>
#include <string>
#include <cstdlib>
#include <SDL3/SDL.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl3.h>
#include <imgui/imgui_impl_opengl3.h>
#include <stdio.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

namespace adws{
    void imguiInitSystem();
    void imguiShutdownSystem();
}

// Estrutura para guardar os dados principais no loop do Emscripten
struct AppState {
    SDL_Window* window;
    SDL_GLContext gl_context;
    bool running;
};

void main_loop(void* arg) {
    AppState* state = static_cast<AppState*>(arg);
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL3_ProcessEvent(&event);
        if (event.type == SDL_EVENT_QUIT) {
            state->running = false;
            #ifdef __EMSCRIPTEN__
            emscripten_cancel_main_loop();
            #endif
        }
    }

    // Início do frame ImGui
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Painel de Controle");
    ImGui::Text("Ola, mundo do SDL3 com ImGui e WebGL!");
    if(ImGui::Button("Clique em mim")){
        std::cout << "Botao clicado no navegador!" << std::endl;
    }
    ImGui::End();

    ImGui::Render();

    // Renderização com OpenGL/WebGL
    //glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
    //glClearColor(0.12f, 0.12f, 0.12f, 1.00f);
    //glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    SDL_GL_SwapWindow(state->window);
}

int main(int argc, char* argv[]){
    if(!SDL_Init(SDL_INIT_VIDEO)){
        SDL_Log("Erro ao inicializar o SDL: %s", SDL_GetError());
        return 1;
    }

    // Configurar atributos para OpenGL ES 3.0 (WebGL 2)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    
    //variável + inicialização da largura e altura da janela para um fallback
    int w = 800;
    int h = 600;

    #ifdef __EMSCRIPTEN__
    // {gerado com IA} Pega dinamicamente a largura e altura da aba do navegador via JS
    w = EM_ASM_INT({
        return window.innerWidth;
    });
    h = EM_ASM_INT({
        return window.innerHeight;
    });
    #endif

    SDL_Window* window = SDL_CreateWindow("Arrois e fejaum da SDL3 + ImGui (Web)", w, h, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
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

    // Configuração do loop principal para Emscripten ou execução normal
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(main_loop, &state, 0, 1);
#else
    while (state.running) {
        main_loop(&state);
    }
#endif

    adws::imguiShutdownSystem();

    SDL_GL_DestroyContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

namespace adws{
    inline void imguiInitSystem(){
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        ImGui::StyleColorsDark();
    }

    inline void imguiShutdownSystem(){
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
    }
}