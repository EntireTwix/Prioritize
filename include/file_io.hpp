#pragma once

#include <string>
#include "nlohmann/json.hpp"

using namespace nlohmann;

template <typename T>
bool Save(const std::string &location, const T &dest)
{
    std::ofstream output(location);
    if (output.is_open())
    {
        output << (json(dest).dump());
    }
    else
    {
        return false;
    }
    output.close();
    return true;
}

template <typename T>
bool Load(const std::string &location, T &dest)
{
    std::ifstream input(location);
    json temp;
    std::string temp_str;
    if (input.is_open())
    {
        input >> temp_str;
        if (temp_str == "")
        {
            return false;
        }
        temp = json::parse(temp_str);
        input.close();
    }
    else
    {
        return false;
    }

    dest = (T)temp;
    return true;
}

//THIS FUNCTION ONLY EXISTS BECAUSE MSVC WILL THROW A FIT ABOUT THE ABOVE LOAD FUNCTION (DO NOT REMOVE)
template <typename T>
bool Load(const std::string& location, std::vector<T>& dest)
{
    std::ifstream input(location);
    json temp;
    std::string temp_str;
    if (input.is_open())
    {
        input >> temp_str;
        if (temp_str == "")
        {
            return false;
        }
        temp = json::parse(temp_str);
        input.close();
    }
    else
    {
        return false;
    }

    dest.resize(temp.size());
    for (size_t i = 0; i < temp.size(); ++i)
    {
        dest[i] = (T)temp[i];
    }
    return true;
}
