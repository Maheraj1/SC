#include "Engine/Math/Math.h"

#include "box2d/b2_math.h"

#include <string>

namespace SC
{
    Vector2f FromBox2dVector2(b2Vec2 vec)
    {
        return Vector2f(vec.x, vec.y);
    }
    
    b2Vec2 ToBox2dVector2(Vector2f vec)
    {
        return b2Vec2(vec.x, vec.y);
    }


    float Math::Lerp(float a, float b, float t)
    {
        return a + t * (b - a);
    }

    Vector2f Math::Lerp(Vector2f a, Vector2f b, float t)
    {
        return a + t * (b - a);
    }

    Vector3f Math::Lerp(Vector3f a, Vector3f b, float t)
    {
        return a + t * (b - a);
    }


    Vector3f operator/(Vector3i vec, float n)
    {
        float x, y, z;

        x = vec.x / n;
        y = vec.y / n;
        z = vec.z / n;

        return {x, y, z};
    }
};

namespace std {
    string to_string(SC::Vector2f val)
    {
        return to_string(val.x) + ',' + to_string(val.y);
    }

    string to_string(SC::Vector2i val)
    {
        return to_string(val.x) + ',' + to_string(val.y);
    }

    string to_string(SC::Vector3f val)
    {
        return to_string(val.x) + ',' + to_string(val.y) + ',' + to_string(val.z);
    }

    string to_string(SC::Vector3i val)
    {
        return to_string(val.x) + ',' + to_string(val.y) + ',' + to_string(val.z);
    }

    string to_string(SC::Vector4f val)
    {
        return to_string(val.x) + ',' + to_string(val.y) + ',' + to_string(val.z) + ',' + to_string(val.w);
    }

    string to_string(SC::Vector4i val)
    {
        return to_string(val.x) + ',' + to_string(val.y) + ',' + to_string(val.z) + ',' + to_string(val.w);
    }

    
    string to_string(b2Vec2 val)
    {
        return to_string(val.x) + ',' + to_string(val.y);
    }

}
