#include "themefile.hh"

#include <sys/stat.h>

ThemeFile::ThemeFile(const std::string &theme_name)
{
    auto ofile = find_file(theme_name);
    if (!ofile) {
        fprintf(stderr, "File for theme '%s' was not found.\n", theme_name.c_str());
        exit(EXIT_FAILURE);
    }

    full_path_ = *ofile;
}

std::optional<std::string> ThemeFile::find_file(std::string const& theme_file)
{
    struct stat buffer {};
    for (const char* path : theme_paths) {
        std::string full_path = std::string(path) + "/" + theme_file + ".customvm_theme";
        if (stat(full_path.c_str(), &buffer) == 0)
            return full_path;
    }

    return {};
}


void ThemeFile::mark_modification_time()
{
    struct stat buffer {};
    if (stat(full_path().c_str(), &buffer) == 0)
        last_load_ = buffer.st_mtime;
}

bool ThemeFile::is_modified() const
{
    struct stat buffer {};
    if (stat(full_path().c_str(), &buffer) == 0)
        return buffer.st_mtime > last_load_;
    return false;
}
