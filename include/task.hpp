#pragma once
#include "property.hpp"
#include <vector>
#include <algorithm>

struct Task
{
    std::string name;
    std::unordered_map<std::string, float &> property_values; //property_name, value refferences
};

struct TaskScore
{
    const std::string name;
    float overall_score;
    bool operator<(const TaskScore &ts) const
    {
        return this->overall_score < ts.overall_score;
    }
};

bool eval_required_flag = true; //true if changes have been made
static std::vector<TaskScore> task_buffer;

std::vector<TaskScore> TaskEval(const std::vector<Task> &tasks)
{
    if (eval_required_flag)
    {
        std::vector<TaskScore> res;
        res.reserve(tasks.size());

        //apply values to properties
        float total_sum = 0, sum;
        for (const Task &t : tasks)
        {
            sum = 0;
            for (const auto &p : t.property_values)
            {
                sum += properties.at(p.first) * p.second;
            }
            total_sum += sum;
            res.push_back({t.name, sum});
        }

        //sort
        std::sort(res.begin(), res.end());

        //softmax
        for (TaskScore &ts : res)
        {
            ts.overall_score = (int)((ts.overall_score / total_sum) * 100);
        }

        eval_required_flag = false;
        task_buffer = res;
        return res;
    }
    return task_buffer;
}