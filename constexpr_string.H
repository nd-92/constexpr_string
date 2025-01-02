#ifndef CONSTEXPR_STRING_H
#define CONSTEXPR_STRING_H

#include <algorithm>
#include <array>
#include <cstdint>
#include <string_view>
#include <type_traits>

// Returns the length of a const char*
[[nodiscard]] consteval std::size_t constexpr_strlen(const char *s)
{
    return std::string::traits_type::length(s);
}

// Returns the string length for an integral constant of type T in base 10
template <typename T, const T value_>
[[nodiscard]] consteval std::size_t constexpr_strlen(const std::integral_constant<T, value_> value)
{
    // Special case for value = 0
    if constexpr (value() == 0)
    {
        return 2;
    }

    T n = value();

    std::size_t i = 1;
    // Add an extra space for negative vals if the type is signed
    if constexpr (std::is_signed_v<T>)
    {
        // Handle case of signed and negative
        if constexpr (value() < 0)
        {
            i = i + 1;
            while (n < 0)
            {
                // Divide n by the base
                n = n / 10;
                // Increment the digit counter
                i++;
            }
        }
        // Handle case of signed and positive
        else
        {
            while (n > 0)
            {
                // Divide n by the base
                n = n / 10;
                // Increment the digit counter
                i++;
            }
        }
    }
    else // Otherwise the type is unsigned and positive
    {
        while (n > 0)
        {
            // Divide n by the base
            n = n / 10;
            // Increment the digit counter
            i++;
        }
    }

    // Return the digit counter
    return i;
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

// Converts an integral constant of type T to a char array
template <typename T, const T value_, const std::size_t length_ = constexpr_strlen(std::integral_constant<T, value_>())>
[[nodiscard]] consteval std::array<char, length_> integral_to_char_array(const std::integral_constant<T, value_> value)
{
    // Value is zero
    if constexpr (value() == 0)
    {
        return std::array<char, 2>{"0"[0], 0};
    }

    // Value must be positive if not zero
    std::array<char, length_> buffer;

    if constexpr (value() < 0)
    {
        std::size_t i_ = static_cast<std::size_t>(-value());
        buffer[0] = "-"[0];
        for (std::size_t k = length_ - 1; k > 1; k--)
        {
            buffer[k - 1] = "0123456789"[i_ % 10];
            i_ = i_ / 10;
        }
        buffer[length_ - 1] = 0;
        return buffer;
    }
    // Value must be positive if nether zero nor negative
    else
    {
        std::size_t i_ = value();
        for (std::size_t k = length_ - 1; k > 0; k--)
        {
            buffer[k - 1] = "0123456789"[i_ % 10];
            i_ = i_ / 10;
        }
        buffer[length_ - 1] = 0;
        return buffer;
    }

    return buffer;
}

template <const std::size_t N>
class constexpr_string
{
public:
    // Constructor taking array of chars
    [[nodiscard]] consteval constexpr_string(const char (&a)[N]) : charString_(std::to_array(a)) {};

    // Constructor taking a std::array
    [[nodiscard]] consteval constexpr_string(const std::array<char, N> &a) : charString_(a) {};

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

    // Add a string to an integral constant
    template <typename T, const T value_, const std::size_t size = constexpr_strlen(std::integral_constant<T, value_>())>
    [[nodiscard]] consteval auto operator+(const std::integral_constant<T, value_> val) const
    {
        return constexpr_string<N + size - 1>(constexpr_arrcat(charString_, integral_to_char_array(val)));
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