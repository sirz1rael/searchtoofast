#include "application/app.hpp"

int main(int argc, char* argv[]) {
    searchtoofast::app::App& app = searchtoofast::app::App::get_instance();
    app.init("com.example.searchtoofast");

    auto status = app.run();
}
