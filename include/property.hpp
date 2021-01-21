#pragma once
#include <unordered_map>
#include <string>

std::unordered_map<std::string, float> properties;

std::unordered_map<std::string, float> ValueEnum{
    {"Practically None", 0},
    {"Very Low", 1.66},
    {"Low", 3.33},
    {"Medium", 5},
    {"High", 6},
    {"Very High", 8.33},
    {"Extremely High", 10}};

auto &PracticallyNone = ValueEnum.at("Practically None");
auto &VeryLow = ValueEnum.at("Very Low");
auto &Low = ValueEnum.at("Low");
auto &Medium = ValueEnum.at("Medium");
auto &High = ValueEnum.at("High");
auto &VeryHigh = ValueEnum.at("Very High");
auto &ExtremelyHigh = ValueEnum.at("Extremely High");