#include "gmock/gmock.h"

#include "../../engine/core/Texture.hpp"

/*
 * @file MockTexture.hpp
 * @brief Class that mocks the behavior of a Texture2D class
 *
 * @deails Can be used to simualte behavior of a Texture2D.
 *          Attention: not perfect yet, I am unsure if it sets the values acctually correct.
 *
 * @author Felix Hommel
 * @date Dec 1, 2024
 */
class MockTexture2D : public Texture2D
{
public:
    MockTexture2D() : Texture2D(true) {}

    MOCK_METHOD(void, generate, (int width, int height, unsigned char* data));
};
