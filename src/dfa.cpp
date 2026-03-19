#include "dfa.h"
#include "stdexcept"
#include <sstream>
#include <iostream>
DFA::DFA(const sectionsMap& sm){
    /*====STATES======*/
    auto statesIt = sm.find(".states");
    if (statesIt != sm.end() && !statesIt->second.empty()) {
        for (const auto& l : statesIt->second) {
            std::istringstream line(l);
            std::string word;
            if (!(line >> word)) {
                continue;
            }

            if(word == "F") {
                line >> word;
                this->addFinalState(word);
                this->addState(word);
            } else if (word == "S") {
                line >> word;
                this->setStartingState(word);
                this->addState(word);
            } else {
                this->addState(word);
            }
        }
    } else {
        throw std::runtime_error("States este gol!\n");
    }

    if (this->startingState.empty()) {
        throw std::runtime_error("Nu ai definit starea initiala (S <stare>).\n");
    }

    /*====ALPHABET======*/

    auto alphabetIt = sm.find(".alphabet");
    if (alphabetIt != sm.end() && !alphabetIt->second.empty()) {
        for (const auto& l : alphabetIt->second) {
            std::istringstream line(l);
            char c;
            while (line >> c) {
                this->addSymbol(c);
            }
        }
    } else {
        throw std::runtime_error("Alphabet este gol!\n");
    }

    /*====TRANSITIONS======*/
    auto transitionsIt = sm.find(".transitions");
    if (transitionsIt != sm.end() && !transitionsIt->second.empty()) {
        for (const auto& l : transitionsIt->second) {
            std::istringstream line(l);
            std::string from, sym, to;

            if (!(line >> from >> sym >> to)) {
                continue;
            }

            try {
                this->addTransition(from, sym, to);
            } catch (const std::exception& e) {
                std::cerr << "Eroarea cand s-au parsat tranzitiile: " << e.what() << '\n';
            }
        }
    } else {
        throw std::runtime_error("Transitions este gol!\n");
    }

}

DFA::DFA(const DFA& other)
    : startingState(other.startingState){
        this->transitions = other.transitions;
        this->finalStates = other.finalStates;
        this->states = other.states;
        this->alphabet = other.alphabet;
        this->allSymbol = other.allSymbol;
}

DFA& DFA::operator=(const DFA& other){

    if(this == &other)
        return *this;

    this->startingState = other.startingState;
    this->transitions = other.transitions;
    this->finalStates = other.finalStates;
    this->states = other.states;
    this->alphabet = other.alphabet;
    this->allSymbol = other.allSymbol;

    return *this;
}

void DFA::setStartingState(const std::string& s){
    this->startingState = s;
}

void DFA::addState(const std::string& state){
    this->states.insert(state);
}

void DFA::addFinalState(const std::string& state){
    this->finalStates.insert(state);
}


void DFA::addSymbol(const char c){
    this->alphabet.insert(c);
}

void DFA::addTransition(const std::string& from, const std::string& symbol, const std::string& to){
    if(this->states.count(from) && this->states.count(to)){
        if(symbol.length() == 1 && this->alphabet.count(symbol[0]))
            this->transitions[from][symbol[0]] = to;
        else if(symbol == "ALL"){
            for(char c : this->alphabet){
                this->transitions[from][c] = to;
            }
        }else throw std::runtime_error("Simbolul nu apartine alfabetului!\n");
    }else throw std::runtime_error("Nu exista state-ul respectiv\n");
}

bool DFA::accepts(const std::string& word){
    std::string current = startingState;

    for (char c : word) {
        if (transitions[current].find(c) == transitions[current].end())
            return false;

        current = transitions[current][c];
    }

    return finalStates.count(current) > 0;
}
