// This file is part of EmuShell
// A multi video game systems emulator.
//
// Copyright(C) 2015 Ala Ibrahim Hadid
// E-mail: mailto:ahdsoftwares@hotmail.com
//
// This program is free software : you can redistribute it and / or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
// GNU General Public License for more details.
// You should have received a copy of the GNU General Public License
// along with this program.If not, see <http://www.gnu.org/licenses/>.
#include <map>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <stdint.h>
class Path
{
public:
    static std::string Separator()
    {
#ifdef __MINGW32__
        return "\\";
#else
        return "/";
#endif
    }
    static std::string Combine(const std::string &path1, const std::string &path2)
    {
        return path1 + Separator() + path2;
    }
    static std::string GetFileName(const std::string &path)
    {
        std::size_t found = path.find_last_of("/\\");
        return path.substr(found+1);
    }
    static std::string GetFileNameWithoutExtnsion(const std::string &path)
    {
        std::string retval = GetFileName(path);

        std::size_t found = retval.find_last_of(".");

        retval.erase(found, retval.size() -found);

        return retval;
    }
    static std::string GetExtnsion(const std::string &path)
    {
        std::size_t found = path.find_last_of(".");
        return path.substr(found);
    }
    static bool IsFileExist(const std::string &path)
    {
        std::ifstream f(path.c_str());
        if (f.good())
        {
            f.close();
            return true;
        }
        else
        {
            f.close();
            return false;
        }
    }
    static void DeleteFile(const std::string &path)
    {
        if (IsFileExist(path))
        {
            std::remove(path.c_str());
        }
    }
};
class Convert
{
public:
    template <typename T>
    static std::string T_to_string(T const &val)
    {
        std::ostringstream ostr;
        ostr << val;

        return ostr.str();
    }

    template <typename T>
    static T string_to_T(std::string const &val)
    {
        std::istringstream convert(val);
        T returnVal;
        convert >> returnVal;
        return returnVal;
    }

    static std::string to_string_hexa(const uint32_t &val)
    {
        std::stringstream stream;
        stream << std::hex << val;
        std::string result( stream.str() );

        return result;
    }

    static float string_to_float(std::string const &val)
    {
        return atof(val.c_str());
    }

    static double string_to_double(std::string const &val)
    {
        return atof(val.c_str());
    }
    template <typename T>
    static std::string to_string(T const &val)
    {
        std::stringstream ss;
        ss<<val;
        return ss.str();
    }

    static std::string string_to_T(std::string const &val)
    {
        return val;
    }
};

namespace CONFIG
{
extern std::string fName;

extern void Load(const std::string &fName);
extern void Load();
extern void Save(const std::string &fName);
extern void Save();
extern bool keyExists(const std::string &key);

extern bool GetBoolValue(const std::string &key, const bool &defaultValue);
extern std::string GetStringValue(const std::string &key, const std::string &defaultValue);
extern int GetInt32Value(const std::string &key, const int &defaultValue);
extern uint32_t GetUint32Value(const std::string &key, const uint32_t &defaultValue);
extern float GetFloatValue(const std::string &key, const float &defaultValue);
extern double GetDoubleValue(const std::string &key, const float &defaultValue);
extern void SetValue(const std::string &key, const bool &value);
extern void SetValue(const std::string &key, const std::string &value);
extern void SetValue(const std::string &key, const int &value);
extern void SetValue(const std::string &key, const uint32_t &value);
extern void SetValue(const std::string &key, const float &value);
extern void SetValue(const std::string &key, const double &value);
}
