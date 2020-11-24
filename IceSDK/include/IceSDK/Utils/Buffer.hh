#pragma once

#include <cstring>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace IceSDK::Utils
{
    class Buffer
    {
    public:
        typedef std::vector<uint8_t> value_type;

        /*****************************************************
         * Push
         *
         * Pushes a value onto the buffer
         *
         * @param tVal Value
         *
         * @return Self
         *****************************************************/
        template<typename T>
        Buffer* Push(T tVal)
        {
            size_t vSz = sizeof(T);
            prealloc(vSz);

            uint8_t* vPtr;
            if (std::is_pointer<T>::value)  // This is fine if we're on an x86
                                            // CPU as it uses little endian by
                                            // default!
                vPtr = *(uint8_t**) &tVal;
            else
                vPtr = (uint8_t*) &tVal;

            if (std::is_same<T, std::string>::value)  // Implementation for
                                                      // wrapping strings
            {
                auto inner = *(std::string*) (&tVal);
                vPtr = nullptr;
                vSz = 0;

                this->Push(inner.c_str());
            }

            if (std::is_same<T, std::string_view>::value)  // Implementation for
                                                           // wrapping strings
            {
                auto inner = *(std::string*) (&tVal);
                vPtr = nullptr;
                vSz = 0;

                this->Push(inner.c_str());
            }

            if (std::is_same<T, const char*>::value)  // Implementation for
                                                      // wrapping const char*
            {
                vPtr = *(uint8_t**) &tVal;
                vSz = strlen((const char*) vPtr);

                prealloc(vSz);
            }

            for (size_t i = 0; i < vSz; i++) m_vInnerBuffer.push_back(vPtr[i]);

            return this;
        }

        /*****************************************************
         * Pop
         *
         * Pops and returns a value from the buffer
         *
         * NOTE: It wont be removed! it'll just increase
         *       an internal value
         *
         * @param tVal Value
         *
         * @return Self
         *****************************************************/
        template<typename T>
        T Pop(size_t sSize)
        {
            T* vPtr = (T*) (m_vInnerBuffer.data() + m_sPos);
            m_sPos += sSize;

            if (std::is_same<T, std::string>::value)  // Implementation for
                                                      // wrapping strings
            {
                std::string empty =
                    std::string((const char*) vPtr, (const char*) vPtr + sSize);

                return *(T*) &empty;
            }

            if (std::is_same<T, const char*>::value)  // Implementation for
                                                      // wrapping const char*
            {
                auto buff = new char[sSize + 1];
                memcpy(buff, vPtr, sSize);
                buff[sSize] = 0;  // Set null terminator
                vPtr = (T*) buff;
                return *((T*) &vPtr);
            }

            return *vPtr;
        }

        /*****************************************************
         * Data
         *
         * if called, Returns it's inner data
         *
         * @return Written data
         *****************************************************/
        const value_type& data() { return this->m_vInnerBuffer; }

    private:
        void prealloc(size_t l)
        {
            if (m_vInnerBuffer.capacity() < m_vInnerBuffer.size() + l)
                m_vInnerBuffer.reserve(m_vInnerBuffer.capacity() + 1024);
        }

        size_t m_sPos{};
        value_type m_vInnerBuffer{};
    };
}  // namespace IceSDK::Utils
