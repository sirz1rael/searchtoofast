#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "searchtoofast.hpp"
#include <gtkmm/window.h>
#include <gtkmm/builder.h>
#include <gtkmm/searchentry.h>
#include <gtkmm/button.h>
#include <gtkmm/menubutton.h>
#include <gtkmm/listbox.h>
#include <gtkmm/expander.h>
#include <gtkmm/label.h>
#include <gtkmm/combobox.h>
namespace searchtoofast::app::window {

class Window {
private:
    Gtk::Window* window_{nullptr};
    Gtk::SearchEntry* search_entry_{nullptr};
    Gtk::ListBox* results_list_{nullptr};




    searchtoofast::SearchTooFast search_engine;

    void on_search_clicked();  // Kept for compatibility
    void on_search_changed();
    void on_close_clicked();
    void perform_search(const Glib::ustring& query);
    void update_results_list(const std::list<searchtoofast::SearchResult>& results);

public:
    Window();
    ~Window() = default;

    Gtk::Window* get_window() const { return window_; }
    void set_title(const Glib::ustring& title);
    void set_size(const int& width, const int& height);
    void show();
};

} // namespace searchtoofast::app::window

#endif // WINDOW_HPP
