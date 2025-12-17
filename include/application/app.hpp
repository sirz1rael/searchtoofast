#ifndef APP_HPP
#define APP_HPP

#include "application/window.hpp"
#include "glibmm/refptr.h"
#include "gtkmm/application.h"
#include "gtkmm/window.h"
#include <memory>
#include <string>

namespace searchtoofast::app {

class App {
private:
    std::shared_ptr<Gtk::Application> application_;
    std::shared_ptr<window::Window> main_window_;

    App() = default;
    ~App() = default;
public:
    void init(const std::string& app_id);
    int run();

    const Glib::RefPtr<Gtk::Application>& get_application() const {
        return application_;
    }

    // Singleton pattern
    static App& get_instance() {
        static App instance;
        return instance;
    }
};
}; // namespace searchtoofast::app
#endif // APP_HPP
