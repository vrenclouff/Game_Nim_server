//
// Created by vrenclouff on 26/11/16.
//

#ifndef NIMSERVER_MAINOPTIONS_H
#define NIMSERVER_MAINOPTIONS_H


#include <map>
#include <string>

class MainOptions {
public:
    typedef std::pair<std::string, std::string> Option;
    MainOptions(int argc, char *argv[]);
    virtual ~MainOptions();
    std::string getAppName() const;
    bool hasKey(const std::string&) const;
    Option* getParamFromKey(const std::string&) const;
    void printOptions() const;
private:
    typedef std::map<std::string, std::string> Options;
    void parse();
    const char* const *begin() const;
    const char* const *end() const;
    const char* const *last() const;
    Options options_;
    int argc_;
    char** argv_;
    std::string appName_;
};


#endif //NIMSERVER_MAINOPTIONS_H
