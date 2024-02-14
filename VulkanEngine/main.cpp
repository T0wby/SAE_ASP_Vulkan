#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "Core/System/Engine.h"


std::unique_ptr<CEngine> pEngine{ nullptr };

int main() {
    
    pEngine = std::make_unique<CEngine>();

    pEngine->Run();

    return 0;
}
