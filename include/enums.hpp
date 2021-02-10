#pragma once
#include "nlohmann/json.hpp"
#include <fstream>
#include <memory>
#include <string>
#include <vector>

using namespace nlohmann;

struct Value
{
    std::string name;
    float weight;
    bool select = false;
};

void to_json(json &j, const Value &v)
{
    j = json{{"name", v.name}, {"weight", v.weight}};
}
void from_json(const json &j, Value &v)
{
    v.name = j["name"];
    v.weight = j["weight"];
}

static std::vector<Value> values;
static std::vector<float> enumFloats{0, 7.5, 18.5, 33, 60, 90, 140};

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