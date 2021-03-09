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
    j = json{{"name", v.name}, {"weight", v.weight}};
}
void from_json(const json &j, Value &v)
{
    v.name = j["name"];
    v.weight = j["weight"];
}

static std::vector<Value> values;
static std::array<float, 7> enum_floats{0, 7.5, 18.5, 33, 60, 90, 140};
static std::array<std::pair<float, float>, 7> enum_colors;

inline void UpdateColors() noexcept
{

    for (uint_fast8_t i = 0; i < 7; ++i)
    {
        enum_colors[i] = {(enum_floats[i] / 140) * 2.5, 1 - (float)(enum_floats[i] / 140)};
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