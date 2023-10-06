#ifndef THEMEFILE_HH_
#define THEMEFILE_HH_

#include <optional>
#include <string>

class ThemeFile {
public:
    explicit ThemeFile(std::string const& filename);

    void mark_modification_time();
    bool is_modified() const;

    std::string const& full_path() const { return full_path_; }

private:
    std::string full_path_;
    time_t last_load_ = 0;

    static std::optional<std::string> find_file(std::string const& theme_file);

    static constexpr const char* theme_paths[] = {
            "../themes",
            "./themes",
    };
};


#endif //THEMEFILE_HH_
