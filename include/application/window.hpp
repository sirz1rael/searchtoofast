#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <cstddef>

#include <gtkmm/window.h>

namespace searchtoofast::app::window {

class Window : public Gtk::Window {
private:
    size_t width_{};
    size_t height_{};

public:
    Window();
    ~Window() override;

    void set_size(const size_t& width, const size_t& height);
    Gtk::Window& get_class();
};
} // namespace searchtoofast::window

#endif // WINDOW_HPP
