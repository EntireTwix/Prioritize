#pragma once
#include "enums.hpp"

struct Task
{
    std::string name = "nil";
    float score = 0;
    std::vector<int> task_values; //x being the value index, the int being the enum
};

static std::vector<Task> task_buffer;
void ResizeTaskVals()
{
    for (Task &t : task_buffer)
    {
        t.task_values.resize(values.size());
    }
}
void UpdateScores()
{
}

//sum each
//sort
//softmax & percent