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

const auto &PracticallyNone = ValueEnum.at("Practically None");
const auto &VeryLow = ValueEnum.at("Very Low");
const auto &Low = ValueEnum.at("Low");
const auto &Medium = ValueEnum.at("Medium");
const auto &High = ValueEnum.at("High");
const auto &VeryHigh = ValueEnum.at("Very High");
const auto &ExtremelyHigh = ValueEnum.at("Extremely High");