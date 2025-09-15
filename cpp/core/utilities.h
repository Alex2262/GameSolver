//
// Created by Alexander Tian on 5/25/25.
//

#ifndef GAMESOLVER_UTILITIES_H
#define GAMESOLVER_UTILITIES_H

#include <string>

template <typename Out>
void split(const std::string &s, char delim, Out result);

std::vector<std::string> split(const std::string &s, char delim);

std::string concat(std::vector<std::string>& elems, char delim, size_t start, size_t end);

std::string get_project_root();

#endif //GAMESOLVER_UTILITIES_H
