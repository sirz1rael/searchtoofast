#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <cstddef>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

namespace searchtoofast::app::config {

class Config {
private:

struct WindowConfig {
    std::string window_title = "SearchTooFast";
    size_t width = 800;
    size_t height = 600;
};

struct AppConfig {
    std::string application_id = "com.t0kkaaa.searchtoofast";
    bool is_first_run {true};
};

struct SearchConfig {
    size_t max_results_to_display {100};
};

WindowConfig window_config_;
AppConfig app_config_;
SearchConfig search_config_;

fs::path config_file_path_;
std::string application_name_;

Config() = default;
~Config() = default;

public:

void set_config_file_path(const fs::path& path);

void init(const fs::path& path, char* argv[]);

void create_config_file_if_not_exists();
void load_config_file();
void save_config_file();

const WindowConfig get_window_config() const {
    return this->window_config_;
}

const AppConfig get_app_config() const {
    return this->app_config_;
}

const SearchConfig get_search_config() const {
    return this->search_config_;
}

// Singleton pattern
static Config& get_instance() {
    static Config instance;
    return instance;
}

};

}; // namespace searchtoofast::app::config

#endif // CONFIG_HPP
