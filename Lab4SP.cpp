#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <vector>
#include <string>
#include <queue>
#include <sstream>

// Структура для збереження переходів
struct Transition {
    int fromState;
    char symbol;
    int toState;
};

int main() {
    std::string filename;
    std::cout << "Enter path to input file ";
    std::cin >> filename;

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return 1;
    }

    int alphabetSize, numStates, startState;
    std::string line;
    std::getline(file, line);
    std::istringstream size(line);
    size >> alphabetSize;
    std::getline(file, line);
    std::istringstream num(line);
    num >> numStates;
    std::getline(file, line);
    std::istringstream start(line);
    start >> startState;

    // Читання фінальних станів
    std::set<int> finalStates;
    int state;
    std::getline(file, line);
    std::istringstream fin(line);
    while(fin >> state){
        finalStates.insert(state);
    }

    // Читання переходів
    std::vector<Transition> transitions;
    std::map<int, std::vector<std::pair<char, int>>> transitionMap;
    int fromState, toState;
    char symbol;

    while(std::getline(file, line)){
        std::istringstream fst(line);
        fst >> fromState >> symbol >> toState;
        transitions.push_back({ fromState, symbol, toState });
        transitionMap[fromState].push_back({ symbol, toState });
    }
    file.close();

    // Виявлення недосяжних станів
    std::set<int> reachableStates;
    std::queue<int> stateQueue;
    stateQueue.push(startState);
    reachableStates.insert(startState);

    while (!stateQueue.empty()) {
        int currentState = stateQueue.front();
        stateQueue.pop();
        for (const auto& [sym, nextState] : transitionMap[currentState]) {
            if (reachableStates.find(nextState) == reachableStates.end()) {
                reachableStates.insert(nextState);
                stateQueue.push(nextState);
            }
        }
    }

    // Вивід недосяжних станів
    std::cout << "Unreachable states: ";
    for (int i = 0; i < numStates; ++i) {
        if (reachableStates.find(i) == reachableStates.end()) {
            std::cout << i << " ";
        }
    }
    std::cout << std::endl;

    // Виявлення тупикових станів
    std::set<int> deadEndStates;
    std::set<int> canReachFinal;

    // Використовуємо чергу для BFS починаючи з фінальних станів
    std::queue<int> queue;
    for (int finalState : finalStates) {
        queue.push(finalState);
        canReachFinal.insert(finalState);
    }
    
    while (!queue.empty()) {
        int state = queue.front();
        queue.pop();
    
        // Перевіряємо всі переходи, які ведуть до цього стану
        for (const auto& [fromState, transitions] : transitionMap) {
            for (const auto& [symbol, toState] : transitions) {
                if (toState == state && canReachFinal.find(fromState) == canReachFinal.end()) {
                    canReachFinal.insert(fromState);
                    queue.push(fromState);
                }
            }
        }
    }
    
    // Стани, які не можуть досягти фінального стану, є тупиковими
    for (int i = 0; i < numStates; ++i) {
        if (canReachFinal.find(i) == canReachFinal.end()) {
            deadEndStates.insert(i);
        }
    }

    // Вивід тупикових станів
    std::cout << "Dead-end states: ";
    for (int state : deadEndStates) {
        std::cout << state << " ";
    }
    std::cout << std::endl;

    return 0;
}
