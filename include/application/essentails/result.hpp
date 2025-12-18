#ifndef RESULT_HPP
#define RESULT_HPP



#include "cairomm/fontoptions.h"
#include "gtkmm/enums.h"
#include "gtkmm/expander.h"
#include "gtkmm/listbox.h"
#include "models/search_result.hpp"
#include <gtkmm/label.h>
#include <gtkmm/combobox.h>
#include <vector>
namespace searchtoofast::app::essentials {

class ResultTitle : public Gtk::Label {
public:
    ResultTitle(const std::string& title) : Gtk::Label() {
        this->set_text(title);
        this->set_xalign(0.0);
        this->set_markup("<b>" + title + "</b>");
        this->property_valign().set_value(Gtk::Align::CENTER);
        this->property_height_request().set_value(48);
    }
    ~ResultTitle() = default;
};

class ResultContent : public Gtk::ListBox {
public:
    ResultContent(const std::vector<std::string>& entries) : Gtk::ListBox() {
        for (const auto& entry : entries) {
            auto label = Gtk::make_managed<Gtk::Label>();
            label->set_text(entry);
            label->set_xalign(0.0);
            label->set_valign(Gtk::Align::CENTER);
            this->append(*label);
        }
    }
    ~ResultContent() = default;
};

class Result : public Gtk::Expander {

public:
    // Nested Result class to represent individual search results
    // Displays file path and snippet
    // Expandable to show more details
    Result(const searchtoofast::SearchResult& search_result) : Gtk::Expander() {
        { // settings
            this->property_height_request().set_value(48);
        }

        // Set the title of the expander to the file path
        auto title = Gtk::make_managed<ResultTitle>(search_result.file_path);
        this->set_label_widget(*title);

        // Grouping similar files in one
        auto content = Gtk::make_managed<ResultContent>(search_result.line_content);
        this->set_child(*content);

        this->set_expanded(false); // Start collapsed
    }

};

} // namespace searchtoofast::app::essentials

#endif // RESULT_HPP
