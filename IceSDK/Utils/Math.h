#pragma once

#include <glm/vec2.hpp>

namespace IceSDK::Math
{
    template<typename T>
    struct Rect
    {
        T top, bottom, left, right;

        Rect(glm::vec2 pos, glm::vec2 size)
        {
            top = pos.y;
            left = pos.x;

            bottom = pos.y + size.y;
            right = pos.x + size.x;
        }

        T Width() const noexcept { return right - left; }

        T Height() const noexcept { return bottom - top; }

        T X() const noexcept { return left; }

        T Y() const noexcept { return top; }

        glm::vec2 Position() noexcept { return { X(), Y() }; }

        glm::vec2 Size() noexcept { return { Width(), Height() }; }

        T length() noexcept { return (left - right) * (top - bottom); }
    };

    typedef Rect<float> Rectf;
    typedef Rect<double> Rectd;

    constexpr float Max(float x, float y) noexcept
    {
        if (x > y) return x;

        return y;
    }

    constexpr inline float Min(float x, float y) noexcept
    {
        if (x > y) return y;

        return x;
    }
}  // namespace IceSDK::Math
