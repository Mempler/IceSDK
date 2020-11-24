#pragma once

namespace IceSDK::Utils::Math
{
    /*****************************************************
     * Max
     *
     * Get the biggest value between 2 values
     *
     * @param fX X
     * @param fY Y
     *
     * @return The larger value
     *****************************************************/
    constexpr float Max(const float fX, const float fY) noexcept
    {
        if (fX > fY) return fX;

        return fY;
    }

    /*****************************************************
     * Min
     *
     * Get the smallest value between 2 values
     *
     * @param fX X
     * @param fY Y
     *
     * @return The larger value
     *****************************************************/
    constexpr inline float Min(const float fX, const float fY) noexcept
    {
        if (fX > fY) return fY;

        return fX;
    }

    /*****************************************************
     * Clamp
     *
     * Clamp 2 values and gets the lowest or highest
     *
     * @param fValue Value
     * @param fMin Minimum value
     * @param fMax Maximum value
     *
     * @return in between
     *****************************************************/
    constexpr inline float Min(const float fValue, const float fMin,
                               const float fMax) noexcept
    {
        if (fValue < fMin) return fMin;
        if (fValue > fMax) return fMax;

        return fValue;
    }
}  // namespace IceSDK::Utils::Math
