#include "application/app.hpp"

int main(int argc, char* argv[]) {
    searchtoofast::app::App& app = searchtoofast::app::App::get_instance();
    app.init("com.t0kkaaa.searchtoofast", argc, argv);

    auto status = app.run();
}
