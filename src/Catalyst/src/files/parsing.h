#pragma once

#include "core/internal.h"

#include <unordered_map>

namespace Catalyst
{

    extern size_t CatalystFindMatch(std::string data, size_t index, char open, char close);
    extern std::vector<std::string> CatalystSplitString(std::string data, char delim);

    /**
     * JSON Object
     * 
     * @li Out Types
     * @li std::string
     * @li long
     * @li double
     * @li bool
     * @li A std::vector of any previous type
     * @li An other json object
     */
    class json
    {

    public:
        json() = default;
        ~json() = default;

        json& construct(std::string data);

    public:
        template<typename T>
        inline T value(std::string key);

        template<>
        inline std::string value(std::string key)
        {
            return (m_Data.find(key) != m_Data.end() ? m_Data[key] : "");
        }
        template<>
        inline long value(std::string key)
        {
            return (m_Data.find(key) != m_Data.end() ? std::stol(m_Data[key]) : 0);
        }
        template<>
        inline double value(std::string key)
        {
            return (m_Data.find(key) != m_Data.end() ? std::stod(m_Data[key]) : 0.0);
        }
        template<>
        inline bool value(std::string key)
        {
            return (m_Data.find(key) != m_Data.end() ? (m_Data[key] == "true" ? true : (m_Data[key] == "false" ? false : std::stoi(m_Data[key]))) : false);
        }
        template<>
        inline json value(std::string key)
        {
            return (m_Data.find(key) != m_Data.end() ? json().construct(m_Data[key]) : json());
        }

        ///===========================================================================
        /// JSON Array Types
        ///===========================================================================

        template<>
        inline std::vector<std::string> value(std::string key)
        {
            return (m_Data.find(key) != m_Data.end() ? split_string(m_Data[key]) : std::vector<std::string>({""}));
        }
        template<>
        inline std::vector<long> value(std::string key)
        {
            return (m_Data.find(key) != m_Data.end() ? split_int(m_Data[key]) : std::vector<long>({ 0 }));
        }
        template<>
        inline std::vector<double> value(std::string key)
        {
            return (m_Data.find(key) != m_Data.end() ? split_double(m_Data[key]) : std::vector<double>({ 0.0 }));
        }
        template<>
        inline std::vector<bool> value(std::string key)
        {
            return (m_Data.find(key) != m_Data.end() ? split_bool(m_Data[key]) : std::vector<bool>({ false }));
        }
        template<>
        inline std::vector<json> value(std::string key)
        {
            return (m_Data.find(key) != m_Data.end() ? split_json(m_Data[key]) : std::vector<json>({ json() }));
        }

    private:
        std::vector<std::string> split_string(std::string data);
        std::vector<long>        split_int(std::string data);
        std::vector<double>      split_double(std::string data);
        std::vector<bool>        split_bool(std::string data);
        std::vector<json>        split_json(std::string data);

    private:
        std::unordered_map<std::string, std::string> m_Data;

    };

    /**
     * Class that stores.
     */
    class csv
    {
        // Reads column major
        //
        // Key1, Key2, Key3
        // Val1, Val2, Val3
        // ... , ... , ...
        //

    public:
        csv() = default;
        ~csv() = default;

        csv& construct(std::string data);

        template<typename T>
        inline std::vector<T> value(std::string key);

        template<>
        inline std::vector<std::string> value(std::string key)
        {
            return (m_Data.find(key) != m_Data.end() ? split_string(m_Data[key]) : std::vector<std::string>({ "" }));
        }
        template<>
        inline std::vector<long> value(std::string key)
        {
            return (m_Data.find(key) != m_Data.end() ? split_int(m_Data[key]) : std::vector<long>({ 0 }));
        }
        template<>
        inline std::vector<double> value(std::string key)
        {
            return (m_Data.find(key) != m_Data.end() ? split_double(m_Data[key]) : std::vector<double>({ 0.0 }));
        }
        template<>
        inline std::vector<bool> value(std::string key)
        {
            return (m_Data.find(key) != m_Data.end() ? split_bool(m_Data[key]) : std::vector<bool>({ false }));
        }
        
    private:
        std::vector<std::string> split_string(std::vector<std::string> data);
        std::vector<long>        split_int(   std::vector<std::string> data);
        std::vector<double>      split_double(std::vector<std::string> data);
        std::vector<bool>        split_bool(  std::vector<std::string> data);

    private:
        std::unordered_map<std::string, std::vector<std::string>> m_Data;

    };

    namespace file
    {

        class CSVFile
        {

        };

        class JSONMultiLangFile : protected json
        {

        public:
            JSONMultiLangFile(std::string path);
            ~JSONMultiLangFile();

            std::vector<std::string> getLanguages();
            json getValueByLanguage(std::string lang);

        private:

        };

    }

}
