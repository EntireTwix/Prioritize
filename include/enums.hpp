#pragma once
#include "nlohmann/json.hpp"
#include <fstream>
#include <memory>
#include <string>
#include <array>
#include <vector>

using namespace nlohmann;

struct Value
{
    std::string name;
    float weight;
    bool select = false;
};

void to_json(json &j, const Value &v) noexcept
{
    std::string temp_str = v.name;
    std::replace(temp_str.begin(), temp_str.end(), ' ', '_');
    j = json{{"name", temp_str}, {"weight", v.weight}};
}
void from_json(const json &j, Value &v)
{
    std::string temp_str = j["name"];
    v.name.resize(temp_str.size());
    for (uint_fast8_t i = 0; i < temp_str.size(); ++i)
    {
        if (temp_str[i] == '_')
        {
            v.name[i] = ' ';
        }
        else
        {
            v.name[i] = temp_str[i];
        }
    }
    v.weight = j["weight"];
}

static std::vector<Value> values;
static std::array<float, 7> enum_floats{0, 7.5, 18.5, 33, 60, 90, 140};
static std::array<std::pair<float, float>, 7> enum_colors;

inline void UpdateColors() noexcept
{
    for (uint_fast8_t i = 0; i < 7; ++i)
    {
        enum_colors[i] = {(enum_floats[i] / enum_floats[6]) * 2.5, 1 - (float)(enum_floats[i] / enum_floats[6])};
    }
}

//for gui usage
enum ValEnum
{
    VirtuallyNone,
    VeryLow,
    Low,
    Medium,
    High,
    VeryHigh,
    ExtremelyHigh
};