#pragma once

#include <memory>
#include <sstream>
#include <string>

namespace IceSDK
{
    namespace Memory
    {
        template<typename T>
        using Ptr = std::shared_ptr<T>;

        template<typename T>
        using WeakPtr = std::weak_ptr<T>;

        inline std::string SizeInText(const double pLen)
        {
            std::ostringstream out;
            out.precision(2);

            if (pLen < 1024.0)
            {
                out << pLen << " B";

                return out.str();
            }

            if (pLen < 1024.0 * 1024.0)
            {
                out << pLen / 1024.0 << " KB";

                return out.str();
            }

            if (pLen < 1024.0 * 1024.0 * 1024.0)
            {
                out << pLen / (1024.0 * 1024.0) << " MB";

                return out.str();
            }

            if (pLen < 1024.0 * 1024.0 * 1024.0 * 1024.0)
            {
                out << pLen / (1024.0 * 1024.0 * 1024.0) << " GB";

                return out.str();
            }

            return std::to_string(pLen) + " UK";
        }
    }  // namespace Memory
}  // namespace IceSDK
