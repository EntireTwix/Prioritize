#pragma once
#include <memory>
#include <string>
#include <vector>

struct Value
{
    std::string name;
    float weight = 1;
};

static std::vector<Value> values;
static std::vector<float> enumFloats{0, 1.66, 3.33, 5, 6, 8.33, 10};

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