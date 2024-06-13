export module Main_module;
import Window_module;
import Compute_module;

export class Controller {
public:     // constructors
    Controller() = default;

public:     // methods
    void Init();
};

void Controller::Init() {
    Window w;

    w.Create_window();
    w.Loop();
    w.Free();
}