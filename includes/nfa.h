#pragma once
#include "unordered_map"
#include "unordered_set"
#include <string>
#include <vector>

using sectionsMap = std::unordered_map<std::string, std::vector<std::string>>;

class NFA{
    private:
        std::unordered_map<std::string, std::unordered_map<char, std::vector<std::string>>> transitions;
        std::unordered_set<std::string> finalStates;
        std::unordered_set<std::string> states;
        std::unordered_set<char> alphabet;
        std::string allSymbol;
        std::string startingState;

        void setStartingState(const std::string& s);

        void addState(const std::string& state);

        void addFinalState(const std::string& state);

        void addSymbol(const char c);

        void addTransition(const std::string& from, const std::string& symbol, const std::string& to);
    public:
        NFA(const sectionsMap& sm);
        NFA(const NFA& other);
        NFA& operator=(const NFA& other);

        bool accepts(const std::string& word);

};