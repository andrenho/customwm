#ifndef DATAFILE_HH_
#define DATAFILE_HH_

#include <string>

#include "../luaw.hh"
#include "types.hh"

#include <unordered_map>

struct DataFile {
    std::string format;
    std::string data;
    std::unordered_map<std::string, Rectangle> slices;
};

template<> DataFile luaw_to(lua_State* L, int index);

#endif //DATAFILE_HH_
