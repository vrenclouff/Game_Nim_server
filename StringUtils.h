//
// Created by vrenclouff on 27/11/16.
//

#ifndef NIMSERVER_STRINGUTILS_H
#define NIMSERVER_STRINGUTILS_H


#include <string>
#include <vector>

class StringUtils {
public:
    static std::string format(int count, ...);
    static void replace(std::string& str, const std::string& from, const std::string& to);
    static std::vector<std::string> split(std::string const &str, std::string const &delimiter);
};


#endif //NIMSERVER_STRINGUTILS_H
