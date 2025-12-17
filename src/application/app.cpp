#include "application/app.hpp"
#include "application/window.hpp"
#include <gtkmm/application.h>
#include <memory>

namespace searchtoofast::app {

void App::init(const std::string& app_id) {
    this->application_ = Gtk::Application::create(app_id);

    this->main_window_ = std::make_shared<window::Window>();

    { // Window settings
        main_window_->set_size(800, 600);
    }
}

int App::run() {
    application_->signal_startup().connect([this]() {
        this->application_->add_window(*main_window_);
        this->main_window_->present();
    });

    return application_->run();
}

} // namespace searchtoofast::app
