#pragma once
#include "unordered_map"
#include "unordered_set"
#include <string>
#include <vector>

using sectionsMap = std::unordered_map<std::string, std::vector<std::string>>;

class DFA{
    private:
        std::unordered_map<std::string, std::unordered_map<char, std::string>> transitions;
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
        DFA(const sectionsMap& sm);
        DFA(const DFA& other);
        DFA& operator=(const DFA& other);

        bool accepts(const std::string& word);

};