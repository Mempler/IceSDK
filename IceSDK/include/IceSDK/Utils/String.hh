#pragma once

#include <sstream>
#include <string>

namespace IceSDK::Utils::String
{
    /*****************************************************
     * Trim
     *
     * Trimg a string bi-directional.
     *
     * @param sInput Input
     * @param svSearch Searching Character / String
     *
     * @return Trimmed string
     *****************************************************/
    inline std::string Trim(std::string sInput, std::string_view svSearch)
    {
        sInput.erase(0, sInput.find_first_not_of(svSearch));
        sInput.erase(sInput.find_last_not_of(svSearch) + 1);

        return sInput;
    }

    /*****************************************************
     * CalculateHash
     *
     * Simple algorithm to calculate a hash of a string
     * used to cache things
     *
     * @param svInput Input
     *
     * @return 64bit Hash Value
     *****************************************************/
    constexpr uint64_t CalculateHash(std::string_view svInput)
    {
        if (svInput.empty()) return 0x00;

        uint64_t hash = 0x00;

        for (auto&& c : svInput)
        {
            hash += c;
            hash += (hash << 10);
            hash ^= (hash >> 6);
        }

        hash += (hash << 3);
        hash ^= (hash >> 11);
        hash += (hash << 15);
        hash ^= (hash >> 23);
        hash += (hash << 27);

        return hash;
    }

    /*****************************************************
     * SizeInText
     *
     * Simple algorthm to calculate bytes in other formats
     *
     * @param dSize Amounf ot bytes
     *
     * @return e.g "12.4G"
     *****************************************************/
    inline std::string SizeInText(const double dSize)
    {
        std::ostringstream out;
        out.precision(2);

        if (dSize < 1024.0)
        {
            out << dSize << " B";

            return out.str();
        }

        if (dSize < 1024.0 * 1024.0)
        {
            out << dSize / 1024.0 << " KB";

            return out.str();
        }

        if (dSize < 1024.0 * 1024.0 * 1024.0)
        {
            out << dSize / (1024.0 * 1024.0) << " MB";

            return out.str();
        }

        if (dSize < 1024.0 * 1024.0 * 1024.0 * 1024.0)
        {
            out << dSize / (1024.0 * 1024.0 * 1024.0) << " GB";

            return out.str();
        }

        return std::to_string(dSize) + " UK";
    }
}  // namespace IceSDK::Utils::String
