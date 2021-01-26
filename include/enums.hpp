#pragma once
#include <fstream>
#include <memory>
#include <string>
#include <vector>

struct Value
{
    std::string name;
    float weight = 1;
    bool select;

    Json::Value Serialize() const
    {
        Json::Value res;
        res["name"] = name;
        res["weight"] = weight;

        return res;
    }
    static bool Save(const std::string &location, const std::vector<Value> &vb)
    {
        Json::StreamWriterBuilder builder;
        const std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
        std::ofstream values(location + "/values.json");
        if (values.is_open())
        {
            Json::Value res;
            for (int i = 0; i < vb.size(); ++i)
            {
                res[i] = vb[i].Serialize();
            }
            writer->write(res, &values);
            values.close();
            return true;
        }
        else
        {
            values.close();
            return false;
        }
    }
    static bool Open(const std::string &location, std::vector<Value> &vb)
    {
        Json::CharReaderBuilder builder;
        Json::Value temp;
        std::ifstream value_save(location + "/values.json");
        builder["collectComments"] = true;
        JSONCPP_STRING errs;
        if (!parseFromStream(builder, value_save, &temp, &errs))
        {
            value_save.close();
            return false;
        }
        value_save.close();
        vb.resize(temp.size());
        for (int i = 0; i < temp.size(); ++i)
        {
            vb[i] = {temp[i]["name"].asString(), temp[i]["weight"].asFloat()};
        }
        return true;
    }
};

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