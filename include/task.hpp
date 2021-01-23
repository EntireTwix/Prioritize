#pragma once
#include "enums.hpp"

struct Task
{
    std::string name = "nil";
    float score = 0;
    bool state;
    std::vector<int> task_values; //x being the value index, the int being the enum
};

static std::vector<Task> task_buffer;
static bool change_flag = true;

inline void UpdateScores()
{
    if (change_flag) //only updates when changes are made
    {
        //resize
        for (Task &t : task_buffer)
        {
            t.task_values.resize(values.size());
        }

        //sum each (if !state)
        //sort
        //softmax & percent
        change_flag = false;
    }
}