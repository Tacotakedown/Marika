//
// Created by Taco on 7/2/2024.
//

#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <string>
#include <string_view>
#include <vector>

namespace ModUtils::Utils {
    std::vector<std::string> SplitString(const std::string_view string, const std::string_view &delimiters);

    std::vector<std::string> SplitString(const std::string_view string, const char delimiter);
}

#endif //STRINGUTILS_H
