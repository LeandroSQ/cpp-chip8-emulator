#pragma once

#include <string>

std::string toHex(int n, int padding);

/**
 * @brief Attempts to read a file in binary mode
 * @attention Remember to delete[] buffer after use
 *
 * @param path The path to the file
 * @param buffer The buffer to store the file contents
 * @param size The size of the file
 * @return 0 if the file was read successfully, -1 otherwise
 */
int8_t readFile(const char* path, uint8_t*& buffer, size_t& size);

/**
 * @brief Extract the filename, without extension, from a given path
 *
 * @param path The path to the file
 * @return The extracted filename
 */
std::string getFileName(const char* path);

#if defined(_MSC_VER)
    #define DISABLE_WARNING_PUSH           __pragma(warning( push ))
    #define DISABLE_WARNING_POP            __pragma(warning( pop ))
    #define DISABLE_WARNING(warningNumber) __pragma(warning( disable : warningNumber ))

    #define DISABLE_WARNING_UNREFERENCED_FORMAL_PARAMETER    DISABLE_WARNING(4100)

#elif defined(__GNUC__) || defined(__clang__)
    #define DO_PRAGMA(X) _Pragma(#X)
    #define DISABLE_WARNING_PUSH           DO_PRAGMA(GCC diagnostic push)
    #define DISABLE_WARNING_POP            DO_PRAGMA(GCC diagnostic pop)
    #define DISABLE_WARNING(warningName)   DO_PRAGMA(GCC diagnostic ignored #warningName)

    #define DISABLE_WARNING_UNREFERENCED_FORMAL_PARAMETER    DISABLE_WARNING(-Wunused-parameter)

#else
    #define DISABLE_WARNING_PUSH
    #define DISABLE_WARNING_POP
    #define DISABLE_WARNING_UNREFERENCED_FORMAL_PARAMETER
#endif
