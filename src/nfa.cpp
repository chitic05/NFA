#include "nfa.h"
#include "stdexcept"
#include <sstream>
#include <iostream>
#include <unordered_set>
NFA::NFA(const sectionsMap& sm){
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

NFA::NFA(const NFA& other)
    : startingState(other.startingState){
        this->transitions = other.transitions;
        this->finalStates = other.finalStates;
        this->states = other.states;
        this->alphabet = other.alphabet;
        this->allSymbol = other.allSymbol;
}

NFA& NFA::operator=(const NFA& other){

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

void NFA::setStartingState(const std::string& s){
    this->startingState = s;
}

void NFA::addState(const std::string& state){
    this->states.insert(state);
}

void NFA::addFinalState(const std::string& state){
    this->finalStates.insert(state);
}


void NFA::addSymbol(const char c){
    this->alphabet.insert(c);
}

void NFA::addTransition(const std::string& from, const std::string& symbol, const std::string& to){
    if(this->states.count(from) && this->states.count(to)){
        if(symbol.length() == 1 && this->alphabet.count(symbol[0]))
            this->transitions[from][symbol[0]].push_back(to);
        else if(symbol == "ALL"){
            for(char c : this->alphabet){
                this->transitions[from][c].push_back(to);
            }
        }else if(symbol == "EPSILON"){
            this->transitions[from]['\0'].push_back(to);
        }
        else throw std::runtime_error("Simbolul nu apartine alfabetului!\n");
    }else throw std::runtime_error("Nu exista state-ul respectiv\n");
}

bool NFA::accepts(const std::string& word){
    std::unordered_set<std::string> current;
    current.insert(this->startingState);

    for (char c : word) {
        bool onGoing = true;
        do{
            onGoing=false;
            std::unordered_set<std::string> next;
            for(const auto& state:current){
                
                auto stateIt = this->transitions.find(state);
                if (stateIt == this->transitions.end()) {
                    continue;
                }

                auto symbolIt = stateIt->second.find('\0');
                if (symbolIt == stateIt->second.end()) {
                    continue;
                }
                
                for (const auto& destination : symbolIt->second) {
                    if(next.insert(destination).second) { //.second verifica daca e noua valoare in set sau nu
                        onGoing = true;
                    }
                }
            }
            if(onGoing)
                current.swap(next);
        }while(onGoing);
        std::unordered_set<std::string> toDelete;
        for (auto& state : current) {
            auto stateIt = this->transitions.find(state);
            if (stateIt == this->transitions.end()) {
                continue;
            }

            auto symbolIt = stateIt->second.find(c);
            if (symbolIt == stateIt->second.end()) {
                continue;
            }

            for (const auto& destination : symbolIt->second) {
                toDelete.insert(state);
                current.insert(destination);
            }
        }
        for(const auto& delState:toDelete)
            current.erase(delState);
    }

    for (const auto& state : current) {
        if (finalStates.count(state) > 0)
            return true;
    }

    return false;
}
