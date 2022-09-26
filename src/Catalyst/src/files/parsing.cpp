#include "phc.h"
#include "parsing.h"

#include <algorithm>
#include <fstream>
#include <streambuf>

namespace Catalyst
{
    json& json::construct(std::string data)
    {
        
        if ((std::count(data.begin(), data.end(), '{') - std::count(data.begin(), data.end(), '}')) != 0)
        {

            CATALYST_CORE_INFO("Json Parser Error [Missing brackets] : {0} - {1}", std::count(data.begin(), data.end(), '{'), std::count(data.begin(), data.end(), '}'));

            Engine::raiseError({Level::Error, "CatalystResult::Json_Object_Missing_Bracket", CatalystResult::Json_Object_Missing_Bracket, __FUNCTION__}); ///TODO
            return (*this);
        }
        if ((std::count(data.begin(), data.end(), '"') % 2) != 0)
        {
            Engine::raiseError({}); ///TODO
            return (*this);
        }
        if ((std::count(data.begin(), data.end(), '[') - std::count(data.begin(), data.end(), ']')) != 0)
        {
            Engine::raiseError({}); ///TODO
            return (*this);
        }

        

        data.erase(std::remove_if(data.begin(), data.end(), iswspace), data.end());
        data.erase(std::remove_if(data.begin(), data.end(), iswcntrl), data.end());

        size_t prev = 0, next = 0;

        std::string buffer;
        std::pair<std::string, std::string> storage;

        prev = data.find_first_of('{', prev);
        next = data.find_first_of('}', prev);

        if (next - prev <= 1)
            return (*this);

        while (prev != data.npos)
        {

            if (data.find_first_of('{', prev + 1) == 0) //named
            {
                storage.first = std::to_string(m_Data.size());
                goto UNNAMED;
            }

            //Read Key
            prev = data.find_first_of('"', prev+1);
            if (prev == data.npos)
            {
                Engine::raiseError({}); ///TODO
                return (*this);
            }

            next = data.find_first_of('"', prev+1);
            if (next == data.npos)
            {
                Engine::raiseError({}); ///TODO
                return (*this);
            }

            storage.first = data.substr(prev+1, (next-prev)-1);
            
            //Read value                                   V - find this
            prev = data.find_first_of(':', prev+1); // "Key" : Value
            //                                              -- cheack these two for any symbols

        UNNAMED:

            if (((     next = data.find_first_of('"', prev+1)) - prev) == 1) //Read string
            {
                prev = next;

                next = data.find_first_of('"', prev+1);
                if (next == data.npos)
                {
                    Engine::raiseError({}); ///TODO
                    return (*this);
                }

                storage.second = data.substr(prev+1, (next - prev)-1);

                prev = next;
            }
            else if (((next = data.find_first_of('[', prev+1)) - prev) == 1) //Read array
            {
                prev = next;

                next = CatalystFindMatch(data, prev, '[', ']');

                if (next == data.size())
                {
                    Engine::raiseError({}); ///TODO
                    return (*this);
                }

                storage.second = data.substr(prev+1, (next - prev)-1);

                prev = next;
            }
            else if (((next = data.find_first_of('{', prev+1)) - prev) == 1) //Read object
            {
                prev = next;

                next = CatalystFindMatch(data, prev, '{', '}');

                if (next == data.size())
                {
                    Engine::raiseError({}); ///TODO
                    return (*this);
                }

                storage.second = data.substr(prev, next - prev + 1);

                prev = next;
            }
            else //Read int/double/bool
            {
                next = data.find_first_of(',', prev+1);

                if (next == data.npos)
                    next = data.find_first_of('}', prev+1); //EOF

                if (next == data.npos)
                {
                    Engine::raiseError({}); ///TODO
                    return (*this);
                }

                storage.second = data.substr(prev+1, (next - prev)-1);

                prev = next;
            }

            m_Data[storage.first] = storage.second;

            if (data.find_first_of(',', prev) == data.npos) //If EOF find will return (*this) npos and break the loop
                prev = data.npos;
        }

        return (*this);
    }

    std::vector<std::string> json::split_string(std::string data)
    {
        size_t prev = 0, next = 0;
        std::vector<std::string> result;

        data.erase(std::remove(data.begin(), data.end(), '\"'), data.end());

        while ((next = data.find(',', prev)) != data.npos)
        {
            result.push_back(data.substr(prev, next - prev));
            prev = next + 1;
        }

        result.push_back(data.substr(prev, next - prev));

        return result;
    }
    std::vector<long> json::split_int(std::string data)
    {
        auto split = split_string(data);
        std::vector<long> result;

        for (auto& item : split)
        {
            result.push_back(std::stol(item));
        }

        return result;
    }
    std::vector<double> json::split_double(std::string data)
    {
        auto split = split_string(data);
        std::vector<double> result;

        for (auto& item : split)
        {
            result.push_back(std::stod(item));
        }

        return result;
    }
    std::vector<bool> json::split_bool(std::string data)
    {
        auto split = split_string(data);
        std::vector<bool> result;

        for (auto& item : split)
        {
            if (item == "true")
                result.push_back(true);
            else if (item == "false")
                result.push_back(false);
            else
                result.push_back(std::stoi(item));
        }

        return result;
    }
    std::vector<json> json::split_json(std::string data)
    {

        size_t prev = 0, next = 0;
        std::vector<std::string> split;

        while (prev != data.npos)
        {

            next = CatalystFindMatch(data, prev, '{', '}');

            if (next == data.size())
            {
                Engine::raiseError({}); ///TODO
                return std::vector<json>({ json() });
            }

            split.push_back(data.substr(prev, next - prev + 1));

            prev = data.find('{', next);
        }

        std::vector<json> result;

        for (auto& item : split)
        {
            result.push_back(json().construct(item));
        }

        return result;
    }


    csv& csv::construct(std::string data)
    {
        if (!std::count(data.begin(), data.end(), ','))
        {
            Engine::raiseError({});
            return (*this);
        }

        //================================================
        // Clean Data
        //================================================

        data.erase(std::remove(data.begin(), data.end(), '\"'), data.end()); 

        std::stringstream ss(data);
        std::string buffer;
        std::vector<std::string> keys;


        //================================================
        // Read Title/Header Data
        //================================================

        if (std::getline(ss, buffer))
        {
            for (auto& item : (keys = CatalystSplitString(buffer, ',')))
            {
                m_Data[item];
            }
        }


        //================================================
        // Add data to appropriate header
        //================================================
        while (std::getline(ss, buffer))
        {
            auto items = CatalystSplitString(buffer, ',');

            if (items.size() > keys.size()) //Elements must match, otherwise data set is ill-formed
            {
                Engine::raiseError({});
                return (*this);
            }

            for (unsigned int i = 0; i < items.size(); i++)
            {
                m_Data[keys[i]].push_back(items[i]);
            }
        }

        return (*this);
    }

    std::vector<std::string> csv::split_string(std::vector<std::string> data)
    {
        return data;
    }
    std::vector<long>        csv::split_int(std::vector<std::string> data)
    {
        std::vector<long> result;

        for (auto& item : data)
        {
            result.push_back(std::stol(item));
        }

        return result;
    }
    std::vector<double>      csv::split_double(std::vector<std::string> data)
    {
        std::vector<double> result;

        for (auto& item : data)
        {
            result.push_back(std::stod(item));
        }

        return result;
    }
    std::vector<bool> csv::split_bool(std::vector<std::string> data)
    {
        std::vector<bool> result;

        for (auto& item : data)
        {
            if (item == "true")
                result.push_back(true);
            else if (item == "false")
                result.push_back(false);
            else
                result.push_back(std::stoi(item));
        }

        return result;
    }

    size_t CatalystFindMatch(std::string data, size_t index, char open, char close)
    {
        unsigned int balence = 1;
        for (size_t next = index+1; next < data.size(); next++) {
            if (data[next] == open)
            {
                ++balence;
            }
            else if (data[next] == close) {
                --balence;
                if (!balence)
                {
                    return next;
                }
            }
        }
        return index;
    }

    std::vector<std::string> CatalystSplitString(std::string data, char delim)
    {
        size_t prev = 0, next = 0;
        std::vector<std::string> result;

        while ((next = data.find(delim, prev)) != data.npos)
        {
            result.push_back(data.substr(prev, next - prev));
            prev = next + 1;
        }

        result.push_back(data.substr(prev, next - prev));

        return result;
    }


    namespace file
    {
        JSONMultiLangFile::JSONMultiLangFile(std::string path)
            : json()
        {

            std::ifstream file(path, std::ios::_Noreplace);
            std::string str((std::istreambuf_iterator<char>(file)),
                std::istreambuf_iterator<char>());

            (json&)(*this) = json().construct(str);

            file.close();
        }
        JSONMultiLangFile::~JSONMultiLangFile()
        {
        }
        std::vector<std::string> JSONMultiLangFile::getLanguages()
        {
            return value<std::vector<std::string>>("languages");
        }
        json JSONMultiLangFile::getValueByLanguage(std::string lang)
        {
            return value<json>(lang);
        }
    }
}