#pragma once
#include <algorithm>
#include "enums.hpp"

struct Task
{
    std::string name = "nil";
    float score = 0;
    bool state;
    std::vector<int> task_values; //x being the value index, the int being the enum
    bool operator<(const Task &t) const
    {
        return this->score < t.score;
    }
};

static std::vector<Task> task_buffer;
static bool change_flag = true;

inline void UpdateScores()
{
    float sum, total_sum = 0;
    if (change_flag) //only updates when changes are made
    {
        sum = 0;

        for (Task &t : task_buffer)
        {
            //resize
            t.task_values.resize(values.size());

            //sum each
            if (!t.state)
            {
                for (int i = 0; i < values.size(); ++i)
                {
                    sum += t.task_values[i] * values[i].weight; //assigned enum * weight
                }
                t.score = sum;
                total_sum += sum;
            }
        }

        //sort
        std::sort(task_buffer.begin(), task_buffer.end());

        //softmax
        for (Task &t : task_buffer)
        {
            t.score /= total_sum;
        }

        //back to no change
        change_flag = false;
    }
}