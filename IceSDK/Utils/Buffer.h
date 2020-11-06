#pragma once

#include <string>
#include <string_view>
#include <cstring>
#include <iostream>
#include <vector>

namespace IceSDK
{
    class Buffer
    {
    public:
        typedef std::vector<uint8_t> value_type;

        template<typename T>
        Buffer* push(T val)
        {
            size_t vSz = sizeof(T);
            prealloc(vSz);

            uint8_t* vPtr;
            if (std::is_pointer<T>::value)  // This is fine if we're on an x86
                                            // CPU as it uses little endian by
                                            // default!
                vPtr = *(uint8_t**) &val;
            else
                vPtr = (uint8_t*) &val;

            if (std::is_same<T, std::string>::value)  // Implementation for
                                                      // wrapping strings
            {
                auto inner = *(std::string*) (&val);
                vPtr = nullptr;
                vSz = 0;

                this->push(inner.c_str());
            }

            if (std::is_same<T, std::string_view>::value)  // Implementation for
                // wrapping strings
            {
                auto inner = *(std::string*) (&val);
                vPtr = nullptr;
                vSz = 0;

                this->push(inner.c_str());
            }


            if (std::is_same<T, const char*>::value)  // Implementation for
                                                      // wrapping const char*
            {
                vPtr = *(uint8_t**) &val;
                vSz = strlen((const char*) vPtr);

                prealloc(vSz);
            }

            for (size_t i = 0; i < vSz; i++) _inner.push_back(vPtr[i]);

            return this;
        }

        template<typename T>
        T pop(size_t len)
        {
            T* vPtr = (T*) (_inner.data() + _r_pos);
            _r_pos += len;

            if (std::is_same<T, std::string>::value)  // Implementation for
                                                      // wrapping strings
            {
                std::string empty =
                    std::string((const char*) vPtr, (const char*) vPtr + len);

                return *(T*) &empty;
            }

            if (std::is_same<T, const char*>::value)  // Implementation for
                                                      // wrapping const char*
            {
                auto buff = new char[len + 1];
                memcpy(buff, vPtr, len);
                buff[len] = 0;  // Set null terminator
                vPtr = (T*) buff;
                return *((T*) &vPtr);
            }

            return *vPtr;
        }

        const value_type& data() { return this->_inner; }

    private:
        void prealloc(size_t l)
        {
            if (_inner.capacity() < _inner.size() + l)
                _inner.reserve(_inner.capacity() + 1024);
        }

        size_t _r_pos{};
        value_type _inner{};
    };
}  // namespace IceSDK
