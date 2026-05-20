#pragma once


#include <vector>

class TerminalOut {

public:
    TerminalOut() {}
    ~TerminalOut() = default;
    virtual void presentLine(const std::string &line) = 0;
    virtual void presentList(std::vector<std::string> list) = 0;
    virtual void presentSuggestion(const std::string &suggestion) = 0;

private:
};