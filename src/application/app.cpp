#include "application/app.hpp"
#include "application/config/config.hpp"
#include "application/window.hpp"
#include <gtkmm/application.h>
#include <memory>
#include <adwaita.h>

namespace searchtoofast::app {

void App::init(const std::string& app_id, const int& argc, char* argv[]) {
    adw_init();

    this->application_ = Gtk::Application::create(app_id);

    this->main_window_ = std::make_shared<window::Window>();

    config::Config& config = config::Config::get_instance();
    config.init("config.json", argv);

    // Apply loaded config to the main window
    { // Window settings
        auto& window_config = config.get_window_config();
        main_window_->set_title(window_config.window_title);
        main_window_->set_size(window_config.width, window_config.height);
    }
}

int App::run() {
    application_->signal_startup().connect([this]() {
        auto gtk_window = main_window_->get_window();
        if (gtk_window) {
            this->application_->add_window(*gtk_window);
            gtk_window->present();
        }
    });

    return application_->run();
}

} // namespace searchtoofast::app
