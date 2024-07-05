//
// Created by Taco on 7/4/2024.
//

#ifndef CONSOLE_H
#define CONSOLE_H

#include <vector>

class Console {
public:
    Console();

    void Add(const char *entry);

    std::vector<const char *> Get(uint64_t maxSize);

    void Clear();

private:
    std::vector<const char *> m_History;
};


#endif //CONSOLE_H
