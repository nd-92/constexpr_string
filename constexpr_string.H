#ifndef CONSTEXPR_STRING_H
#define CONSTEXPR_STRING_H

#include <algorithm>
#include <string_view>
#include <array>

// Returns the length of a const char*
[[nodiscard]] consteval std::size_t constexpr_strlen(const char *s)
{
    return std::string::traits_type::length(s);
}

// Catenates an arbitrary number of char arrays
template <std::size_t... sizes>
[[nodiscard]] inline consteval auto constexpr_arrcat(const std::array<char, sizes> &...arrays)
{
    std::array<char, ((sizes - 1) + ...) + 1> result;
    std::size_t index{};

    ((std::copy_n(arrays.begin(), sizes - 1, result.begin() + index), index += sizes - 1), ...);

    return result;
}

template <const std::size_t N>
class constexpr_string
{
public:
    // Constructor taking array of chars
    [[nodiscard]] consteval constexpr_string(const char (&a)[N]) : charString_(std::to_array(a)) {};

    // Constructor taking a std::array
    [[nodiscard]] consteval constexpr_string(const std::array<char, N> &arr) : charString_(arr) {};

    // Returns the string length
    [[nodiscard]] consteval std::size_t length() const
    {
        return charString_.size();
    }

    // Returns a reference to the underlying char array
    [[nodiscard]] consteval const std::array<char, N> &ref() const
    {
        return charString_;
    }

    // Returns a string_view
    [[nodiscard]] consteval const std::string_view string_view() const
    {
        return std::string_view(charString_.begin(), charString_.end());
    }

    // Add two strings
    template <const std::size_t size>
    [[nodiscard]] consteval auto operator+(const constexpr_string<size> &s) const
    {
        return constexpr_string<N + size - 1>(constexpr_arrcat(charString_, s.ref()));
    }

private:
    // Stores the string as an array of chars
    const std::array<char, N> charString_;
};

// Compares two strings at compile time
template <const std::size_t size_1, const std::size_t size_2>
[[nodiscard]] consteval int constexpr_strcmp(const constexpr_string<size_1> &s_1, const constexpr_string<size_2> &s_2)
{
    // Get the minimum of size_1 and size_2
    // const std::size_t N = std::min(size_1, size_2);

    // Loop over the underlying arrays and compare
    for (std::size_t i = 0; i < std::min(size_1, size_2); i++)
    {
        // Is element i of s_1 greater than s_2?
        if (s_1.ref()[i] > s_2.ref()[i])
        {
            // Return s_1[i] - s_2[i]
            // return std::abs(s_1.ref()[i] - s_2.ref()[i]);
            return 1;
        }
        // Is element i of s_2 greater than s_1?
        else if (s_2.ref()[i] > s_1.ref()[i])
        {
            // Return s_2[i] - s_1[i]
            // return -1 * std::abs(s_1.ref()[i] - s_2.ref()[i]);
            return -1;
        }
    }

    // Strings must be identical up to this point, so check sizes
    if (size_1 > size_2)
    {
        return 1;
    }
    else if (size_1 < size_2)
    {
        return -1;
    }

    // Otherwise strings are a match, so return 0
    return 0;
}

#endif