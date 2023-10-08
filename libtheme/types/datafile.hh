#ifndef DATAFILE_HH_
#define DATAFILE_HH_

#include <string>

#include "../luaw.hh"

struct DataFile {
    std::string format;
    std::string data;
};

template<> DataFile luaw_to(lua_State* L, int index);

#endif //DATAFILE_HH_
