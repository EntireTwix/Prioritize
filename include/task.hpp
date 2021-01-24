#pragma once
#include <algorithm>
#include <json/json.h>
#include <fstream>
#include "enums.hpp"

struct Task
{
    std::string name = "nil";
    float score = 0;
    bool state;
    std::vector<int> task_values; //x being the value index, the int being the enum
    bool select;

    Task() = default;
    bool operator<(const Task &t) const
    {
        return this->score > t.score;
    }
    Json::Value Serialize() const
    {
        Json::Value res;
        res["name"] = this->name;
        res["score"] = this->score;
        res["state"] = this->state;

        Json::Value task_table;
        for (int i = 0; i < task_values.size(); ++i)
        {
            task_table[i] = task_values[i];
        }
        res["task_value"] = task_table;
        return res;
    }
    static bool Save(const std::string &location, const std::vector<Task> &tb)
    {
        Json::StreamWriterBuilder builder;
        const std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
        std::ofstream tasks(location + "/tasks.json");
        if (tasks.is_open())
        {
            Json::Value res;
            for (int i = 0; i < tb.size(); ++i)
            {
                res[i] = tb[i].Serialize();
            }
            writer->write(res, &tasks);
            tasks.close();
            return true;
        }
        else
        {
            tasks.close();
            return false;
        }
    }
    static bool Open(const std::string &location, std::vector<Task> &tb)
    {
        Json::CharReaderBuilder builder;
        Json::Value temp;
        std::ifstream task_save(location + "/tasks.json");
        builder["collectComments"] = true;
        JSONCPP_STRING errs;
        if (!parseFromStream(builder, task_save, &temp, &errs))
        {
            task_save.close();
            return false;
        }
        task_save.close();
        tb.resize(temp.size());
        std::vector<int> val_temp(values.size());
        for (int i = 0; i < temp.size(); ++i)
        {
            for (int j = 0; j < values.size(); ++j)
            {
                val_temp[j] = temp[i]["task_value"][j].asInt();
            }
            tb[i] = {temp[i]["name"].asString(), temp[i]["score"].asFloat(), temp[i]["state"].asBool(), val_temp, 0};
        }
        return true;
    }
};

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
            t.score /= total_sum;
        }

        //back to no change
        change_flag = false;
    }
}