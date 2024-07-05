//
// Created by Taco on 7/4/2024.
//

#include "Console.h"

Console::Console() : m_History({"Welcome to the Marika console"}) {
}

void Console::Add(const char *entry) {
    m_History.insert(m_History.begin(), entry);
}

std::vector<const char *> Console::Get(uint64_t maxSize) {
    std::vector<const char *> consoleClone = m_History;

    if (consoleClone.size() > maxSize) {
        consoleClone.erase(consoleClone.begin(), consoleClone.end() - maxSize);
    }
    return consoleClone;
}


void Console::Clear() {
    m_History.clear();
}
