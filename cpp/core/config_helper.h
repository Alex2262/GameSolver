//
// Created by Alexander Tian on 9/14/25.
//

#ifndef GAMESOLVER_CONFIG_HELPER_H
#define GAMESOLVER_CONFIG_HELPER_H

#include "../include/nlohmann/json.hpp"
#include <string>

using json = nlohmann::json;

template <class T>
T require_key(const json& file, const std::string& key) {
    if (!file.contains(key)) {
        throw std::runtime_error(std::string("Missing key: ") + key);
    }

    try {
        return file.at(key).get<T>();
    } catch (...) {
        throw std::runtime_error(std::string("Bad type for key: ") + key);
    }
}

template <class T>
T require_range(const json& file, const std::string& key, T low, T high) {
    T value = require_key<T>(file, key);

    if (value < low || value > high) {
        throw std::runtime_error(std::string("Out of range: ") + key);
    }

    return value;
}

#endif //GAMESOLVER_CONFIG_HELPER_H
