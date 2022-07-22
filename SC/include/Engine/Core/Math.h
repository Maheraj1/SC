#pragma once

#include "Core.h"

#include "glm/glm.hpp"
// #include "box2d/b2_math.h"

#include <string>

namespace SC
{
    // Type defines

    typedef glm::fvec2 Vector2f;
    typedef glm::fvec3 Vector3f;
    typedef glm::fvec4 Vector4f;

    typedef glm::ivec2 Vector2i;
    typedef glm::ivec3 Vector3i;
    typedef glm::ivec4 Vector4i;

    typedef glm::vec<3, uint8_t> Color;
    typedef glm::vec<3, short> Color16;

    typedef glm::fvec3  ColorF;
    typedef glm::dvec3 ColorD;

    typedef glm::mat4 Matrix4;

    typedef Vector2f Vector2;
    typedef Vector3f Vector3;
    typedef Vector4f Vector4;

    Vector3f operator/(Vector3i vec, float n);

    /**
     * @brief Convertor of glm::vec2/Vector2 to box2dVec2 
     * 
     * @param vec glm::vec2/Vector2 to convert
     * @return b2Vec2
     */
    // b2Vec2 SC_API ToBox2dVector2(Vector2f vec);

    /**
     * @brief Convertor of box2dVec2 to glm::vec2/Vector2
     * 
     * @param vec box2dVec2 to convert
     * @return @ref Vector2
     */
    // Vector2f SC_API FromBox2dVector2(b2Vec2 vec);
};

namespace std {
    string to_string(SC::Vector2f val);
    string to_string(SC::Vector2i val);
    string to_string(SC::Vector3f val);
    string to_string(SC::Vector3i val);
    string to_string(SC::Vector4f val);
    string to_string(SC::Vector4i val);
    
    // string to_string(b2Vec2 val);
    // string to_string(b2Vec3 val);
}
