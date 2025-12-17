#include "application/window.hpp"

namespace searchtoofast::app::window {

Window::Window() {
}

Window::~Window() = default;

void Window::set_size(const size_t& width, const size_t& height) {
    width_ = width;
    height_ = height;
    Gtk::Window::set_default_size(static_cast<int>(width_), static_cast<int>(height_));
}

Gtk::Window& Window::get_class() {
    return *this;
}

}; // namespace searchtoofast::window
