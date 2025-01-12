#include <iostream>

#include "app/Application.hpp"

int main(void) {
   try {
        Application::initialize();
        Application::render();
        Application::close();
    }
    catch (const char* exception) {
        std::cout << exception << std::endl;
    }

    return 0;
}