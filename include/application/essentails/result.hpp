#ifndef RESULT_HPP
#define RESULT_HPP

#include "gtkmm/enums.h"
#include "gtkmm/expander.h"
#include "models/search_result.hpp"
#include <gtkmm/label.h>
#include <gtkmm/box.h>
#include <list>

namespace searchtoofast::app::essentials {

class ResultTitle : public Gtk::Label {
public:
    ResultTitle(const std::string& title, size_t count) : Gtk::Label() {
        std::string text = title + " (" + std::to_string(count) + " occurrences)";
        this->set_text(text);
        this->set_xalign(0.0);
        this->set_markup("<b>" + text + "</b>");
        this->property_valign().set_value(Gtk::Align::CENTER);
        this->property_height_request().set_value(48);
    }
    ~ResultTitle() = default;
};

class ResultItem : public Gtk::Box {
public:
    ResultItem(const searchtoofast::SearchResult& result) : Gtk::Box(Gtk::Orientation::HORIZONTAL, 10) {
        this->set_margin_start(10);
        this->set_margin_end(10);
        this->set_margin_top(5);
        this->set_margin_bottom(5);

        // Line number
        auto line_label = Gtk::make_managed<Gtk::Label>(std::to_string(result.line_number));
        line_label->set_width_chars(4);
        line_label->set_xalign(1.0);
        line_label->add_css_class("dim-label");

        // TODO: Highlight researched word.
        // Line content
        std::string content;
        for (const auto& word : result.line_content) {
            if (word == result.word) {
                content += word + " ";
            } else {
                content += word + " ";
            }
        }
        if (!content.empty() && content.back() == '\n') {
            content.pop_back();
        }

        auto content_label = Gtk::make_managed<Gtk::Label>(content);
        content_label->set_xalign(0.0);
        content_label->set_wrap(true);
        content_label->set_wrap_mode(Pango::WrapMode::WORD_CHAR);

        this->append(*line_label);
        this->append(*content_label);
    }
    ~ResultItem() = default;
};

class ResultContent : public Gtk::Box {
public:
    ResultContent(const std::list<searchtoofast::SearchResult>& results) : Gtk::Box(Gtk::Orientation::VERTICAL, 5) {
        this->set_margin(10);

        for (const auto& result : results) {
            auto item = Gtk::make_managed<ResultItem>(result);
            this->append(*item);
        }
    }
    ~ResultContent() = default;
};

class Result : public Gtk::Expander {
public:
    Result(const std::string& file_path, const std::list<searchtoofast::SearchResult>& results) : Gtk::Expander() {
        // Settings
        this->property_height_request().set_value(48);
        this->set_margin_bottom(5);

        // Set the title with occurrence count
        auto title = Gtk::make_managed<ResultTitle>(file_path, results.size());
        this->set_label_widget(*title);

        // Create content with all results for this file
        auto content = Gtk::make_managed<ResultContent>(results);
        this->set_child(*content);

        this->set_expanded(false); // Start collapsed
    }
};

} // namespace searchtoofast::app::essentials

#endif // RESULT_HPP
