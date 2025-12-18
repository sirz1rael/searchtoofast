#include "application/window.hpp"
#include "searchtoofast.hpp"
#include <glibmm/error.h>
#include <glib.h>
#include <gtkmm/label.h>
#include "application/essentails/result.hpp"

namespace searchtoofast::app::window {

Window::Window() : search_engine("/home/t0kkaaa/projects") {
    auto builder = Gtk::Builder::create();

    try {
        // Load UI file
        builder->add_from_file("/home/t0kkaaa/projects/searchtoofast/assets/MainWindow/main.ui");

        // Get the root window from the UI file
        auto window = builder->get_widget<Gtk::Window>("window");
        auto search_entry = builder->get_widget<Gtk::SearchEntry>("search_entry");
        auto results_list = builder->get_widget<Gtk::ListBox>("results_list");

        window_ = window;
        search_entry_ = search_entry;
        results_list_ = results_list;

        if (!window_) {
            g_warning("Failed to load window from UI file");
            return;
        }

        // Connect signals
        if (search_entry_) {
            search_entry_->signal_search_changed().connect(
                [this]() { on_search_changed(); }
            );
        }

        // Connect UI signal handlers
        builder->set_translation_domain("searchtoofast");

        // Index the project source code directory
        search_engine.index_directory("/home/t0kkaaa/projects");


    } catch (const Glib::Error& ex) {
        g_warning("Error loading UI file: %s", ex.what());
    }
}

void Window::on_search_changed() {
    if (search_entry_) {
        Glib::ustring text = search_entry_->get_text();
        if (text.length() >= 3) {
            g_message("Live search for: %s", text.c_str());
            perform_search(text);
        }
    }
}

void Window::perform_search(const Glib::ustring& query) {
    // Implement your search logic here
    g_message("Performing search for: %s", query.c_str());
    auto results = this->search_engine.search(std::string(query));
    g_message("Found %lu results", results.size());
    update_results_list(results);
}

void Window::update_results_list(const std::list<searchtoofast::SearchResult>& results) {
    if (!results_list_) {
        g_warning("results_list_ is null");
        return;
    }

    g_message("Updating results list with %lu items", results.size());

    // Clear existing items
    while (auto child = results_list_->get_first_child()) {
        results_list_->remove(*child);
    }

    // Add new results
    for (const auto& result : results) {
        g_message("Adding result: %s (line %d)", result.file_path.c_str(), result.line_number);

        auto result_box = Gtk::make_managed<searchtoofast::app::essentials::Result>(result);
        results_list_->append(*result_box);
    }

    results_list_->show();
}

void Window::set_title(const Glib::ustring& title) {
    if (window_) {
        window_->set_title(title);
    }
}

void Window::show() {
    if (window_) {
        window_->present();
    }
}

void Window::on_close_clicked() {
    if (window_) {
        window_->close();
    }
}

} // namespace searchtoofast::app::window
