#pragma once

#include <thread>

#include <iostream>
#include <GLFW/glfw3.h>

class Time {

private:
    static const unsigned int FPS = 100;
   
public:
    static float currentTime;
    static float previousTime;
    static float deltaTime;
    static float frameTime;
    static float timePassed;

public:
    static void calculateCurrentTime();
    static void calculateDeltaTime();
};