#include "Variables.h"

const float F_ZERO = 0.0f;
const float F_ONE = 1.0f;

void Vertex::Position::operator+=(Position a_pos)
{
    x += a_pos.x;
    y += a_pos.y;
    //z += a_pos.z;
}

Vertex::Position Vertex::Position::Zero()
{
    return Position{ F_ZERO, F_ZERO, F_ZERO };
}

Vertex::Position Vertex::Position::Forward()
{
    return Position{ F_ZERO, F_ZERO, F_ONE };
}

Vertex::Color Vertex::Color::Black(void)
{
    return Color{ F_ZERO, F_ZERO, F_ZERO };
}

Vertex::Color Vertex::Color::White(void)
{
    return Color{ F_ONE, F_ONE, F_ONE };
}

Vertex::Color Vertex::Color::Red(void)
{
    return Color{ F_ONE, F_ZERO, F_ZERO };
}

Vertex::Color Vertex::Color::Green(void)
{
    return Color{ F_ZERO, F_ONE, F_ZERO };
}

Vertex::Color Vertex::Color::Blue(void)
{
    return Color{ F_ZERO, F_ZERO, F_ONE };
}
//
//Vertex::Normal Vertex::Normal::Zero()
//{
//    return Normal{ F_ZERO, F_ZERO, F_ZERO };
//}
//Vertex::Normal Vertex::Normal::One()
//{
//    return Normal{ F_ONE, F_ONE, F_ONE };
//}