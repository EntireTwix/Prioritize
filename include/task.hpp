#pragma once
#include <iostream>
#include <algorithm>
#include "enums.hpp"

struct Task
{
    char name[128] = "nil";
    float score = 0;
    bool state = false;
    bool select = false;
    std::vector<int> task_values; //x being the value index, the int being the enum

    Task() = default;
    bool operator<(const Task &t) const
    {
        return this->score > t.score;
    }
};

void to_json(json &j, const Task &t)
{
    std::string temp_str = t.name;
    std::replace(temp_str.begin(), temp_str.end(), ' ', '_');
    j = json{{"name", temp_str}, {"score", t.score}, {"state", t.state}, {"task_value", json(t.task_values)}};
}
void from_json(const json &j, Task &t)
{
    std::string temp_str = j["name"];
    std::replace(temp_str.begin(), temp_str.end(), '_', ' ');
    if (temp_str.size() > 128)
    {
        temp_str.resize(128);
    }
    std::move(temp_str.begin(), temp_str.end(), t.name);
    t.score = j["score"];
    t.state = j["state"];
    const auto temp = j["task_value"];
    t.task_values.resize(temp.size());
    for (size_t i = 0; i < temp.size(); ++i)
    {
        t.task_values[i] = (int)temp[i];
    }
}

static std::vector<Task> task_buffer;
static bool change_flag = true;

inline void UpdateScores()
{
    if (change_flag) //only updates when changes are made
    {
        float sum, total_sum = 0;
        for (Task &t : task_buffer)
        {
            sum = 0;

            //resize
            t.task_values.resize(values.size()); //so that newly added values dont throw with old tasks

            //sum each
            if (!t.state)
            {
                for (int i = 0; i < values.size(); ++i)
                {
                    sum += enumFloats[t.task_values[i]] * values[i].weight; //assigned enum * weight
                }
                t.score = sum;
                total_sum += sum; //for softmax
            }
            else
            {
                t.score = 0;
            }
        }

        //sort
        std::sort(task_buffer.begin(), task_buffer.end());

        //softmax
        for (Task &t : task_buffer)
        {
            if (t.score) //preventing -MAX_INT bug
            {
                t.score /= total_sum;
            }
        }

        //back to no change
        change_flag = false;
    }
}