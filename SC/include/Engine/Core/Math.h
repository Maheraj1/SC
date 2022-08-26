#pragma once

#include "Core.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-volatile"
#include "glm/glm.hpp"
#pragma GCC diagnostic pop

#include <chrono>
#include <string>

struct b2Vec2;
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

    typedef glm::fvec3 ColorF;
    typedef glm::dvec3 ColorD;

    typedef glm::mat4 Matrix4;

    typedef Vector2f Vector2;
    typedef Vector3f Vector3;
    typedef Vector4f Vector4;

    Vector3f operator/(Vector3i vec, float n);

    struct SC_API Math
    {
        static constexpr float PI = 22.0f/7;
        static constexpr float RAD = PI/180.0f;

        static float Lerp(float a, float b, float t);
        static Vector2f Lerp(Vector2f a, Vector2f b, float t);
        static Vector3f Lerp(Vector3f a, Vector3f b, float t);
    };

    namespace UnitLiterals {
        long double operator"" _km(long double n);
        long double operator"" _ham(long double n);
        long double operator"" _dam(long double n);
        long double operator"" _m(long double n);
        long double operator"" _cm(long double n);
        long double operator"" _dm(long double n);
        long double operator"" _mm(long double n);
        long double operator"" _um(long double n);
    }

    /**
     * @brief Convertor of glm::vec2/Vector2 to box2dVec2 
     * 
     * @param vec glm::vec2/Vector2 to convert
     * @return b2Vec2
     */
    b2Vec2 SC_API ToBox2dVector2(Vector2f vec);

    /**
     * @brief Convertor of box2dVec2 to glm::vec2/Vector2
     * 
     * @param vec box2dVec2 to convert
     * @return @ref Vector2
     */
    Vector2f SC_API FromBox2dVector2(b2Vec2 vec);
};

namespace std {
    string SC_API to_string(SC::Vector2f val);
    string SC_API to_string(SC::Vector2i val);
    string SC_API to_string(SC::Vector3f val);
    string SC_API to_string(SC::Vector3i val);
    string SC_API to_string(SC::Vector4f val);
    string SC_API to_string(SC::Vector4i val);
    
    string SC_API to_string(b2Vec2 val);
}
