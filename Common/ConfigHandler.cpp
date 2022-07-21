// This file is part of EmuShell
// A multi video game systems emulator.
//
// Copyright(C) 2015 - 2022 Alaa Ibrahim Hadid
// E-mail: mailto:alaahadidfreeware@gmail.com
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
#include "ConfigHandler.h"

namespace CONFIG
{
std::string fName;
std::map<std::string, std::string> contents;

void removeComment(std::string &line)
{
    if (line.find(';') != line.npos)
        line.erase(line.find(';'));
}
bool onlyWhitespace(const std::string &line)
{
    return (line.find_first_not_of(' ') == line.npos);
}
bool validLine(const std::string &line)
{
    std::string temp = line;
    temp.erase(0, temp.find_first_not_of("\t "));
    if (temp[0] == '=')
        return false;

    for (size_t i = temp.find('=') + 1; i < temp.length(); i++)
        if (temp[i] != ' ')
            return true;

    return false;
}
void extractKey(std::string &key, size_t const &sepPos, const std::string &line)
{
    key = line.substr(0, sepPos);
    if (key.find('\t') != line.npos || key.find(' ') != line.npos)
        key.erase(key.find_first_of("\t "));
}
void extractValue(std::string &value, size_t const &sepPos, const std::string &line)
{
    value = line.substr(sepPos + 1);
    value.erase(0, value.find_first_not_of("\t "));
    value.erase(value.find_last_not_of("\t ") + 1);
}
void extractContents(const std::string &line)
{
    std::string temp = line;
    temp.erase(0, temp.find_first_not_of("\t "));
    size_t sepPos = temp.find('=');

    std::string key, value;
    extractKey(key, sepPos, temp);
    extractValue(value, sepPos, temp);

    if (!keyExists(key))
        contents.insert(std::pair<std::string, std::string>(key, value));
}
void parseLine(const std::string &line, size_t const lineNo)
{
    //if (line.find('=') == line.npos)
    //    exitWithError("CFG: Couldn't find separator on line: " + Convert::T_to_string(lineNo) + "\n");

    //if (!validLine(line))
    //    exitWithError("CFG: Bad format for line: " + Convert::T_to_string(lineNo) + "\n");

    extractContents(line);
}
void ExtractKeys()
{
    std::ifstream file;
    file.open(fName.c_str());
    // if (!file)
    //     exitWithError("CFG: File " + fName + " couldn't be found!\n");

    std::string line;
    size_t lineNo = 0;
    while (std::getline(file, line))
    {
        lineNo++;
        std::string temp = line;

        if (temp.empty())
            continue;

        removeComment(temp);
        if (onlyWhitespace(temp))
            continue;

        parseLine(temp, lineNo);
    }

    file.close();
}

void Load(const std::string &fileName)
{
    fName = fileName;
    ExtractKeys();
}
void Load()
{
    ExtractKeys();
}
void Save(const std::string &fileName)
{
    fName = fileName;
    Save();
}
void Save()
{
    std::ofstream f( fName.c_str() );
    for(std::map<std::string,std::string>::iterator it = contents.begin(); it != contents.end(); ++it)
    {
        f << it->first << " = " << contents[it->first] << "\n";
    }
    f.close();
}
bool keyExists(const std::string &key)
{
    return contents.find(key) != contents.end();
}

bool GetBoolValue(const std::string &key, const bool &defaultValue)
{
    if (!keyExists(key))
        return defaultValue;

    return contents[key] == "1" ? true : false;
}
std::string GetStringValue(const std::string &key, const std::string &defaultValue)
{
    if (!keyExists(key))
        return defaultValue;

    return contents[key];
}
int GetInt32Value(const std::string &key, const int &defaultValue)
{
    if (!keyExists(key))
        return defaultValue;

    return Convert::string_to_T<int>(contents[key]);
}
uint32_t GetUint32Value(const std::string &key, const uint32_t &defaultValue)
{
    if (!keyExists(key))
        return defaultValue;

    return Convert::string_to_T<uint32_t>(contents[key]);
}
float GetFloatValue(const std::string &key, const float &defaultValue)
{
    if (!keyExists(key))
        return defaultValue;

    return Convert::string_to_float(contents[key]);
}
double GetDoubleValue(const std::string &key, const float &defaultValue)
{
    if (!keyExists(key))
        return defaultValue;

    return Convert::string_to_double(contents[key]);
}
void SetValue(const std::string &key, const bool &value)
{
    std::string val_temp = Convert::T_to_string<bool>(value);
    if (!keyExists(key))
    {
        contents.insert(std::pair<std::string, std::string>(key, val_temp));
    }
    else
    {
        contents[key] = val_temp;
    }
}
void SetValue(const std::string &key, const std::string &value)
{
    if (!keyExists(key))
    {
        contents.insert(std::pair<std::string, std::string>(key, value));
    }
    else
    {
        contents[key] = value;
    }
}
void SetValue(const std::string &key, const int &value)
{
    std::string val_temp = Convert::T_to_string<int>(value);
    if (!keyExists(key))
    {
        contents.insert(std::pair<std::string, std::string>(key, val_temp));
    }
    else
    {
        contents[key] = val_temp;
    }
}
void SetValue(const std::string &key, const uint32_t &value)
{
    std::string val_temp = Convert::T_to_string<uint32_t>(value);
    if (!keyExists(key))
    {
        contents.insert(std::pair<std::string, std::string>(key, val_temp));
    }
    else
    {
        contents[key] = val_temp;
    }
}
void SetValue(const std::string &key, const float &value)
{
    std::string val_temp = Convert::T_to_string<float>(value);
    if (!keyExists(key))
    {
        contents.insert(std::pair<std::string, std::string>(key, val_temp));
    }
    else
    {
        contents[key] = val_temp;
    }
}
void SetValue(const std::string &key, const double &value)
{
    std::string val_temp = Convert::T_to_string<double>(value);
    if (!keyExists(key))
    {
        contents.insert(std::pair<std::string, std::string>(key, val_temp));
    }
    else
    {
        contents[key] = val_temp;
    }
}
};


