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
        builder->add_from_file("/home/t0kkaaa/projects/searchtoofast/assets/main.ui");
        // Get the root window from the UI file
        window_ = builder->get_widget<Gtk::Window>("window");
        search_entry_ = builder->get_widget<Gtk::SearchEntry>("search_entry");
        results_list_ = builder->get_widget<Gtk::ListBox>("results_list");

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

    // Group results by file_path
    std::map<std::string, std::list<searchtoofast::SearchResult>> grouped_results;

    for (const auto& result : results) {
        grouped_results[result.file_path].push_back(result);
    }

    // Add grouped results to list
    for (const auto& [file_path, file_results] : grouped_results) {
        auto result_box = Gtk::make_managed<searchtoofast::app::essentials::Result>(
            file_path, file_results);
        results_list_->append(*result_box);
    }

    results_list_->show();
}

void Window::set_title(const Glib::ustring& title) {
    if (window_) {
        window_->set_title(title);
    }
}

void Window::set_size(const int& width, const int& height) {
    if (window_) {
        window_->set_default_size(width, height);
    }
}

void Window::show() {
    if (window_) {
        window_->present();
    }
}
} // namespace searchtoofast::app::window
