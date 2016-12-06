//
// Created by vrenclouff on 27/11/16.
//

#include "StringUtils.h"
#include <stdarg.h>

std::string StringUtils::format(int count, ...)
{
    std::string result;
    va_list list;
    va_start(list, count);
    for (int arg=0; arg < count; ++arg)
    {
        std::string temp = va_arg(list, char *);
        replace(temp, "\n", "");
        result.append(temp);
    }
    va_end(list);
    return result;
}

void StringUtils::replace(std::string& str, const std::string& from, const std::string& to)
{
    if(from.empty()) { return; }
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}

std::vector<std::string> StringUtils::split(std::string const &str, std::string const &delimiter)
{
    std::string copy_str = str.substr(0, str.length());
    std::vector<std::string> temp_array;
    size_t pos = 0;
    std::string token;
    while ((pos = copy_str.find(delimiter)) != std::string::npos) {
        token = copy_str.substr(0, pos);
        temp_array.push_back(token);
        copy_str.erase(0, pos + delimiter.length());
    }
    temp_array.push_back(copy_str);
    return temp_array;
}