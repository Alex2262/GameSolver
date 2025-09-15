//
// Created by Alexander Tian on 5/25/25.
//

#include <iostream>
#include <sstream>
#include "utilities.h"


template <typename Out>
void split(const std::string &s, char delim, Out result) {
    std::istringstream iss(s);
    std::string item;
    while (std::getline(iss, item, delim)) {
        *result++ = item;
    }
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}


std::string concat(std::vector<std::string>& elems, char delim, size_t start, size_t end) {
    std::string res = elems[start];
    for (size_t i = start + 1; i < end; i++) {
        res += delim;
        res += elems[i];
    }

    return res;
}

std::string get_project_root() {
    std::vector<std::string> tokens = split(std::string(__FILE__), '/');
    std::string join;
    for (int i = 0; i < tokens.size() - 3; i++) {
        if (tokens[i].empty()) continue;
        join += "/";
        join += tokens[i];
    }

    std::cout << "PROJECT ROOT: " << join << std::endl;
    return join;
}