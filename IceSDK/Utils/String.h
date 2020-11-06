#pragma once

#include "Jenkins.h"

#include <string>

namespace IceSDK
{
    namespace String
    {
        inline std::string Trim(std::string input, std::string_view search)
        {
            input.erase(0, input.find_first_not_of(search));
            input.erase(input.find_last_not_of(search) + 1);

            return input;
        }

        inline uint64_t CalculateHash(std::string_view input)
        {
            if (input.empty()) return 0;

            uint32_t hash1 = 0;
            uint32_t hash2 = 0;
            hashlittle2(input.data(), input.size(), &hash1, &hash2);

            uint64_t hash = 0;
            hash = hash1;
            hash <<= 32;
            hash |= hash2;

            return hash;
        }
    }  // namespace String
}  // namespace IceSDK
