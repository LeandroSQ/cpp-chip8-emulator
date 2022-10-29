#pragma once

#include <string>

/**
 * @brief Returns the minimum of two values
 *
 * @tparam The type of the values
 * @param a The first value
 * @param b The second value
 * @return The minimum between \p a and \p b
 */
template <typename T>
inline T min(T a, T b) {
    return a < b ? a : b;
}

/**
 * @brief Returns the maximum of two values
 *
 * @tparam The type of the values
 * @param a The first value
 * @param b The second value
 * @return The maximum between \p a and \p b
 */
template <typename T>
inline T max(T a, T b) {
    return a > b ? a : b;
}

/**
 * @brief Returns the hexadecimal representation of a number
 *
 * @param n The number to format
 * @param padding The number of characters to pad-left the output with
 * @return The hexadecimal representation of \p n
 */
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

namespace ImGui {
    void TextCentered(const char* fmt, ...);
}

#if defined(_MSC_VER)
    #define DISABLE_WARNING_PUSH           __pragma(warning( push ))
    #define DISABLE_WARNING_POP            __pragma(warning( pop ))
    #define DISABLE_WARNING(warningNumber) __pragma(warning( disable : warningNumber ))

    #define DISABLE_WARNING_UNREFERENCED_FORMAL_PARAMETER    DISABLE_WARNING(4100)
    #define DISABLE_WARNING_FORMAT_SECURITY    DISABLE_WARNING(4774)
#elif defined(__GNUC__) || defined(__clang__)
    #define DO_PRAGMA(X) _Pragma(#X)
    #define DISABLE_WARNING_PUSH           DO_PRAGMA(GCC diagnostic push)
    #define DISABLE_WARNING_POP            DO_PRAGMA(GCC diagnostic pop)
    #define DISABLE_WARNING(warningName)   DO_PRAGMA(GCC diagnostic ignored #warningName)

    #define DISABLE_WARNING_UNREFERENCED_FORMAL_PARAMETER    DISABLE_WARNING(-Wunused-parameter)
    #define DISABLE_WARNING_FORMAT_SECURITY    DISABLE_WARNING(-Wformat-security)
#else
    #define DISABLE_WARNING_PUSH
    #define DISABLE_WARNING_POP
    #define DISABLE_WARNING_UNREFERENCED_FORMAL_PARAMETER
    #define DISABLE_WARNING_FORMAT_SECURITY
#endif
