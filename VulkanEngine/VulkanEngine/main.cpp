#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm/vec4.hpp>
#include <glm/glm/mat4x4.hpp>
#include <iostream>
#include "Core/System/Engine.h"


std::unique_ptr<CEngine> pEngine{ nullptr };

int main() {
    
    pEngine = std::make_unique<CEngine>();

    pEngine->Run();

    return 0;
}
