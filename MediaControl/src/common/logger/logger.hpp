#ifndef LOGGER_HPP
#define LOGGER_HPP
#include <iostream>

#define LOG(...) []<typename... Args>(Args&&... args) { \
    (std::cout << ... << std::forward<Args>(args)) << std::endl; \
}(__VA_ARGS__)

#endif