#include "SDL.h"
#include "SDL_image.h"

export module Window_module:Interface;

export class Window {
private:    // nested structures
    struct Point {
        int w = 0;
        int h = 0;

        Point operator+(const Point& other) { return { w + other.w, h + other.h }; };
    };

    struct ViewPort {
        int width = 0, height = 0;
        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;

        ViewPort() = default;
        ViewPort(int w, int h) : width(w), height(h) { }
    };

    static class UI {
    public:    // nested structures
        struct Zone {
            int min_w = 0;
            int min_h = 0;

            int w = 0;
            int h = 0;

            Point top_left;
        };

    public:     // fields
        Zone one, two, three, four;
        const double proportion_w = 0.6;
        const double proportion_h = 0.8;

        Point mid;

        int curr_w = 0;
        int curr_h = 0;

    public:     // constructors
        UI() = delete;
        UI(int w, int h) : curr_w(w), curr_h(h) { }

    public:     // methods
        void Update(const Window::ViewPort& vp);
    };

private:    // fields
    const int swidth = 256;
    const int height = 256;
    const int estfps = 50;
    const int msec_in_sec = 1000;

    SDL_Texture* tex = nullptr;
    ViewPort v;
    UI ui = UI(1024, 800);

public:     // constructors
    Window() = default;

public:     // interface methods
    void Create_window();
    void Loop();
    void Free();

private:    // utility methods

private:     // graphic methods
    void renderTriangle(SDL_Renderer* ren);
    void render_blank_map(SDL_Renderer* renderer, const UI::Zone& zone);
    void draw_rectangle(SDL_Renderer* renderer, const Point& tl, const Point& br);
};

void Window::renderTriangle(SDL_Renderer * ren) {
    int w, h;
    SDL_SetRenderDrawColor(ren, 0xff, 0xff, 0xff, 0xff);
    SDL_RenderClear(ren);

    SDL_GetRendererOutputSize(ren, &w, &h);
    SDL_Vertex vertex_1 = { {100, 100}, {0, 0, 0xff, 0xff} };
    SDL_Vertex vertex_2 = { {w - 100, 100}, {0xff, 0xff, 0xff, 0xff} };
    SDL_Vertex vertex_3 = { {100, h - 100}, {0xff, 0, 0, 0xff} };

    SDL_Vertex vertices[] = { vertex_1, vertex_2, vertex_3 };

    SDL_RenderGeometry(ren, NULL, vertices, 3, NULL, 0);
}

void Window::render_blank_map(SDL_Renderer* renderer, const UI::Zone& zone) {
    tex = IMG_LoadTexture(renderer, "E:\\cpp\\SatelliteSim\\images\\blank_world_map.jpg");

    int offset = 10;

    float proportion = 1920.0 / 967.0;
    float horizontal = (float)zone.w - (float)(offset * 2);

    float scale_factor = ((float)zone.h - offset * 2) / (horizontal / proportion);
    if (scale_factor < 1) {
        horizontal = (float)zone.w * scale_factor - (float)(offset * 2);
    }

    Point p = zone.top_left;

    // top-left, bottom-right
    Point tl = { p.w + offset, p.h + offset };
    Point br = { p.w + offset + (int)horizontal, p.h + offset + (int)(horizontal / proportion) };

    SDL_Rect dst = { tl.w, tl.h, (int)horizontal, (int)(horizontal / proportion) };
    SDL_RenderCopy(renderer, tex, NULL, &dst);

    draw_rectangle(renderer, tl, br);
}

void Window::draw_rectangle(SDL_Renderer* renderer, const Point& tl, const Point& br) {
    SDL_SetRenderDrawColor(v.renderer, 0, 0, 0, 255);
    SDL_RenderDrawLine(v.renderer, tl.w, tl.h, br.w, tl.h);
    SDL_RenderDrawLine(v.renderer, br.w, tl.h, br.w, br.h);
    SDL_RenderDrawLine(v.renderer, br.w, br.h, tl.w, br.h);
    SDL_RenderDrawLine(v.renderer, tl.w, br.h, tl.w, tl.h);
}

void Window::Create_window() {
    v = ViewPort(ui.curr_w, ui.curr_h);

    SDL_Init(SDL_INIT_VIDEO);
    v.window = SDL_CreateWindow("Satellite Simulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, v.width, v.height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    v.renderer = SDL_CreateRenderer(v.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

void Window::Loop() {
    for (;;)
    {
        Uint32 start, elapsed, estimated = msec_in_sec / estfps;
        SDL_Event event;
        start = SDL_GetTicks();

        int pending = SDL_PollEvent(&event);

        if (pending && event.type == SDL_QUIT) break;

        SDL_SetRenderDrawColor(v.renderer, 255, 255, 255, 255);
        SDL_RenderClear(v.renderer);

        ui.Update(v);

        render_blank_map(v.renderer, ui.one);

        elapsed = SDL_GetTicks() - start;

        SDL_RenderPresent(v.renderer);
        if (elapsed < estimated)
            SDL_Delay(estimated - elapsed);
    }
}

void Window::Free() {
    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(v.renderer);
    SDL_DestroyWindow(v.window);
    SDL_Quit();
}

void Window::UI::Update(const Window::ViewPort& vp) {
    SDL_GetWindowSize(vp.window, &curr_w, &curr_h);

    mid.w = static_cast<int>(curr_w * proportion_w);
    mid.h = static_cast<int>(curr_h * proportion_h);

    one.w = curr_w * proportion_w;
    one.h = curr_h * proportion_h;
    one.top_left = { 0, 0 };

    two.w = curr_w * (1.0 - proportion_w);
    two.h = curr_h * proportion_h;
    two.top_left = { mid.w, 0 };

    three.w = curr_w * (1.0 - proportion_w);
    three.h = curr_h * (1.0 - proportion_h);
    three.top_left = { mid.w, mid.h };

    four.w = curr_w * proportion_w;
    four.h = curr_h * (1.0 - proportion_h);
    four.top_left = { 0, mid.h };

    SDL_SetRenderDrawColor(vp.renderer, 0, 0, 0, 255);
    SDL_RenderDrawLine(vp.renderer, mid.w - 1, 0, mid.w - 1, curr_h);
    SDL_RenderDrawLine(vp.renderer, mid.w + 1, 0, mid.w + 1, curr_h);
    SDL_RenderDrawLine(vp.renderer, 0, mid.h - 1, curr_w, mid.h - 1);
    SDL_RenderDrawLine(vp.renderer, 0, mid.h + 1, curr_w, mid.h + 1);
}