#include "application/config/config.hpp"
#include "utils/json_helper.hpp"
#include <filesystem>
#include <fstream>
#include <boost/json.hpp>
#include <iostream>
#include <string>



namespace searchtoofast::app::config {

void Config::init(const fs::path& path, char* argv[]) {
    this->application_name_ = argv[0];

    set_config_file_path(path);
    create_config_file_if_not_exists();
    load_config_file();
}

void Config::set_config_file_path(const fs::path& path) {
    // Determine the directory containing the running executable and place the
    // config file next to it. If argv[0] is a relative or absolute path,
    // use its parent directory; otherwise fall back to the current working dir.
    fs::path application_location = fs::absolute(std::string(application_name_));
    fs::path application_dir = application_location.has_filename()
        ? application_location.parent_path()
        : application_location;

    if (application_dir.empty()) {
        application_dir = fs::current_path();
    }

    config_file_path_ = application_dir / path;
}

void Config::create_config_file_if_not_exists() {
    // Ensure the parent directory exists, then create the file with default
    // contents if it does not already exist.
    if (fs::exists(config_file_path_)) {
        return;
    }

    fs::path parent = config_file_path_.parent_path();
    if (!parent.empty() && !fs::exists(parent)) {
        std::error_code ec;
        fs::create_directories(parent, ec);
        // If directory creation failed, bail out silently; caller can handle later.
        if (ec) {
            return;
        }
    }

    std::ofstream file(config_file_path_);
    if (!file.is_open()) {
        return;
    }

    boost::json::object defaults {
        {"window", {
            {"window_title", "SearchTooFast"},
            {"width", 800},
            {"height", 600}
        }},
        {"app", {
            {"application_id", "com.t0kkaaa.searchtoofast"},
            {"is_first_run", true}
        }},
        {"search", {
            {"max_results_to_display", 100}
        }}
    };

    // Write pretty-printed JSON with indentation
    utils::JsonHelper::write_json_pretty(file, boost::json::value(defaults), 0);
    file << std::endl;

    file.close();
}

void Config::load_config_file() {
    // Implementation to load config file
    std::ifstream file(config_file_path_);
    std::cout << "Loading config file from: " << config_file_path_ << std::endl;
    if (!file.is_open()) {
        std::cerr << "Failed to open config file: " << config_file_path_ << std::endl;
        return;
    }

    boost::json::error_code ec;
    boost::json::value jv = utils::JsonHelper::read_json(file, ec);
    if (ec) {
        std::cerr << "Failed to parse config file: " << ec.message() << std::endl;
        return;
    }

    if (!jv.is_object()) {
        std::cerr << "Config file does not contain a JSON object." << std::endl;
        return;
    }

    auto config_obj = jv.as_object();

    // Load window settings
    if (config_obj.contains("window") && config_obj["window"].is_object()) {
        auto window_obj = config_obj["window"].as_object();
        if (window_obj.contains("window_title") && window_obj["window_title"].is_string())
            window_config_.window_title = window_obj["window_title"].as_string().c_str();
        if (window_obj.contains("width") && window_obj["width"].is_int64())
            window_config_.width = static_cast<int>(window_obj["width"].as_int64());
        if (window_obj.contains("height") && window_obj["height"].is_int64())
            window_config_.height = static_cast<int>(window_obj["height"].as_int64());
    }

    // Load app settings
    if (config_obj.contains("app") && config_obj["app"].is_object()) {
        auto app_obj = config_obj["app"].as_object();
        if (app_obj.contains("application_id") && app_obj["application_id"].is_string())
            app_config_.application_id = app_obj["application_id"].as_string().c_str();
        if (app_obj.contains("is_first_run") && app_obj["is_first_run"].is_bool())
            app_config_.is_first_run = app_obj["is_first_run"].as_bool();
    }

    // Load search settings
    if (config_obj.contains("search") && config_obj["search"].is_object()) {
        auto search_obj = config_obj["search"].as_object();
        if (search_obj.contains("max_results_to_display") && search_obj["max_results_to_display"].is_int64())
            search_config_.max_results_to_display = static_cast<int>(search_obj["max_results_to_display"].as_int64());
    }

    file.close();

    std::cout << "Config loaded: " << std::endl;
    std::cout << " Window title: " << window_config_.window_title << std::endl;
    std::cout << "  Window width: " << window_config_.width << std::endl;
    std::cout << "  Window height: " << window_config_.height << std::endl;
    std::cout << "  Application ID: " << app_config_.application_id << std::endl;
    std::cout << "  Is first run: " << (app_config_.is_first_run ? "true" : "false") << std::endl;
    std::cout << "  Max results to display: " << search_config_.max_results_to_display << std::endl;
}

void Config::save_config_file() {
    // Implementation to save config file

    boost::json::object config_obj;
    // Window settings
    boost::json::object window_obj;
    window_obj["window_title"] = window_config_.window_title;
    window_obj["width"] = window_config_.width;
    window_obj["height"] = window_config_.height;
    config_obj["window"] = window_obj;
    // App settings
    boost::json::object app_obj;
    app_obj["application_id"] = app_config_.application_id;
    app_obj["is_first_run"] = app_config_.is_first_run;
    config_obj["app"] = app_obj;
    // Search settings
    boost::json::object search_obj;
    search_obj["max_results_to_display"] = search_config_.max_results_to_display;
    config_obj["search"] = search_obj;
    std::ofstream file(config_file_path_);
    if (!file.is_open()) {
        std::cerr << "Failed to open config file for writing: " << config_file_path_ << std::endl;
        return;
    }

    utils::JsonHelper::write_json_pretty(file, boost::json::value(config_obj), 0);
    file << std::endl;
    file.close();
}

}; // namespace searchtoofast::app::config
