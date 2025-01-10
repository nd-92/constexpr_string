#ifndef CONSTEXPR_STRING_H
#define CONSTEXPR_STRING_H

#include <algorithm>
#include <array>
#include <cstdint>
#include <string_view>
#include <type_traits>

// Returns the length of a const char*
[[nodiscard]] consteval std::size_t constexpr_strlen(const char *s) noexcept
{
    return std::string::traits_type::length(s);
}

// Returns the string length for an integral constant of type T in base 10
template <typename T, const T value_>
[[nodiscard]] consteval std::size_t constexpr_strlen(const std::integral_constant<T, value_> value) noexcept
{
    // Special case for value = 0
    if constexpr (value() == 0)
    {
        return 2;
    }

    // Store the integer value and the total number of chars
    T n = value();
    std::size_t i = 1;

    // The type is signed
    if constexpr (std::is_signed_v<T>)
    {
        // Handle case of signed and negative
        if constexpr (value() < 0)
        {
            // Add an extra space for negative values for the - sign
            i = i + 1;
            while (n < 0)
            {
                // Divide n by the base, then increment the digit counter
                n = n / 10;
                i++;
            }
        }
        // Handle case of signed and positive
        else
        {
            while (n > 0)
            {
                // Divide n by the base, then increment the digit counter
                n = n / 10;
                i++;
            }
        }
    }
    // Otherwise the type is unsigned and positive
    else
    {
        while (n > 0)
        {
            // Divide n by the base, then increment the digit counter
            n = n / 10;
            i++;
        }
    }

    // Return the digit counter
    return i;
}

// Catenates an arbitrary number of char arrays
template <std::size_t... sizes>
[[nodiscard]] inline consteval std::array<char, ((sizes - 1) + ...) + 1> constexpr_arrcat(const std::array<char, sizes> &...arrays) noexcept
{
    std::array<char, ((sizes - 1) + ...) + 1> result;
    std::size_t index{};

    ((std::copy_n(arrays.begin(), sizes - 1, result.begin() + index), index += sizes - 1), ...);

    return result;
}

// Converts an integral constant of type T to a char array
template <typename T, const T value_, const std::size_t length_ = constexpr_strlen(std::integral_constant<T, value_>())>
[[nodiscard]] consteval std::array<char, length_> integral_to_char_array(const std::integral_constant<T, value_> value) noexcept
{
    // Value is zero
    if constexpr (value() == 0)
    {
        return std::array<char, 2>{"0"[0], 0};
    }

    // Allocate memory
    std::array<char, length_> buffer;

    // The value is negative
    if constexpr (value() < 0)
    {
        // Get the absolute value of the integer
        std::size_t i_ = static_cast<std::size_t>(-value());

        // First character is the minus sign
        buffer[0] = "-"[0];

        // Loop over the digits
        for (std::size_t k = length_ - 1; k > 1; k--)
        {
            buffer[k - 1] = "0123456789"[i_ % 10];
            i_ = i_ / 10;
        }

        // Terminate with null
        buffer[length_ - 1] = 0;

        return buffer;
    }
    // Value must be positive if nether zero nor negative
    else
    {
        // Get the absolute value of the integer
        std::size_t i_ = value();

        // Loop over the digits
        for (std::size_t k = length_ - 1; k > 0; k--)
        {
            buffer[k - 1] = "0123456789"[i_ % 10];
            i_ = i_ / 10;
        }

        // Terminate with null
        buffer[length_ - 1] = 0;

        return buffer;
    }
}

template <const std::size_t N>
class constexpr_string
{
public:
    // Constructor taking an array of chars
    [[nodiscard]] consteval explicit constexpr_string(const char (&s)[N]) noexcept : charString_(std::to_array(s)) {};

    // Constructor taking a std::array
    [[nodiscard]] consteval explicit constexpr_string(const std::array<char, N> &s) noexcept : charString_(s) {};

    // Construct from copy
    [[nodiscard]] consteval explicit constexpr_string(const constexpr_string<N> &s) noexcept : charString_(s.data()) {};

    // Returns the string length
    [[nodiscard]] consteval std::size_t length() const noexcept
    {
        return N;
    }

    // Returns a reference to the underlying char array
    [[nodiscard]] consteval const std::array<char, N> &data() const noexcept
    {
        return charString_;
    }

    // Returns a string_view
    [[nodiscard]] consteval const std::string_view string_view() const noexcept
    {
        return std::string_view(charString_.begin(), charString_.end());
    }

    // Add two strings
    template <const std::size_t size_>
    [[nodiscard]] consteval constexpr_string<N + size_ - 1> operator+(const constexpr_string<size_> &s) const noexcept
    {
        return constexpr_string<N + size_ - 1>(constexpr_arrcat(charString_, s.data()));
    }

    // Add a string to an array of chars
    template <const std::size_t size_>
    [[nodiscard]] consteval constexpr_string<N + size_ - 1> operator+(const char (&s)[size_]) const noexcept
    {
        return constexpr_string<N + size_ - 1>(constexpr_arrcat(charString_, std::to_array(s)));
    }

    // Add a string to a std::array of chars
    template <const std::size_t size_>
    [[nodiscard]] consteval constexpr_string<N + size_ - 1> operator+(const std::array<char, size_> s) const noexcept
    {
        return constexpr_string<N + size_ - 1>(constexpr_arrcat(charString_, s));
    }

    // Add a string to an integral constant
    template <typename T, const T value_, const std::size_t size_ = constexpr_strlen(std::integral_constant<T, value_>())>
    [[nodiscard]] consteval constexpr_string<N + size_ - 1> operator+(const std::integral_constant<T, value_> value) const noexcept
    {
        return constexpr_string<N + size_ - 1>(constexpr_arrcat(charString_, integral_to_char_array(value)));
    }

    // Overload the << operator
    template <const std::size_t size_>
    friend inline constexpr std::ostream &operator<<(std::ostream &os, const constexpr_string<size_> &s) noexcept;

    // Overload the [] operator
    [[nodiscard]] consteval char operator[](const std::size_t index) const noexcept
    {
        return charString_[index];
    }

    // Overload the () operator
    [[nodiscard]] consteval std::array<char, 2> operator()(const std::size_t index) const noexcept
    {
        return std::array<char, 2>{charString_[index], 0};
    }

    // Return a c string
    [[nodiscard]] consteval const char *c_str() const noexcept
    {
        return &charString_[0];
    }

private:
    // Stores the string as an array of chars
    const std::array<char, N> charString_;
};

template <const std::size_t size_>
[[nodiscard]] std::ostream inline constexpr &operator<<(std::ostream &os, const constexpr_string<size_> &s) noexcept
{
    os << &s.charString_[0];
    return os;
}

// Compares two strings at compile time
template <const std::size_t size_1, const std::size_t size_2>
[[nodiscard]] consteval int constexpr_strcmp(const constexpr_string<size_1> &s_1, const constexpr_string<size_2> &s_2) noexcept
{
    // Get the minimum of size_1 and size_2
    // const std::size_t N = std::min(size_1, size_2);

    // Loop over the underlying arrays and compare
    for (std::size_t i = 0; i < std::min(size_1, size_2); i++)
    {
        // Is element i of s_1 greater than s_2?
        if (s_1.data()[i] > s_2.data()[i])
        {
            // Return s_1[i] - s_2[i]
            // return std::abs(s_1.data()[i] - s_2.data()[i]);
            return 1;
        }
        // Is element i of s_2 greater than s_1?
        else if (s_2.data()[i] > s_1.data()[i])
        {
            // Return s_2[i] - s_1[i]
            // return -1 * std::abs(s_1.data()[i] - s_2.data()[i]);
            return -1;
        }
    }

    // Strings must be identical up to this point, so check sizes
    if constexpr (size_1 > size_2)
    {
        return 1;
    }
    else if constexpr (size_1 < size_2)
    {
        return -1;
    }

    // Otherwise strings are a match, so return 0
    return 0;
}

// Takes an integral constant of type T and converts it to a constexpr_string
template <typename T, const T value_, const std::size_t size_ = constexpr_strlen(std::integral_constant<T, value_>())>
[[nodiscard]] consteval constexpr_string<size_> constexpr_itoa(const std::integral_constant<T, value_> value) noexcept
{
    return constexpr_string(integral_to_char_array(value));
}

template <const std::size_t size_>
[[nodiscard]] consteval constexpr_string<size_> constexpr_strcopy(const constexpr_string<size_> &s) noexcept
{
    return constexpr_string(s);
}

// Find out if a string contains a particular character
template <const std::size_t size_>
[[nodiscard]] consteval bool constexpr_strcontains(const constexpr_string<size_> &s, const char *character) noexcept
{
    // Loop over the characters, return true if found
    for (std::size_t i = 0; i < size_; i++)
    {
        if (s[i] == character[0])
        {
            return true;
        }
    }

    // Otherwise s does not contain character
    return false;
}

template <const std::size_t size_>
[[nodiscard]] consteval bool constexpr_strcontains(const constexpr_string<size_> &s, const char character) noexcept
{
    // Loop over the characters, return true if found
    for (std::size_t i = 0; i < size_; i++)
    {
        if (s[i] == character)
        {
            return true;
        }
    }

    // Otherwise s does not contain character
    return false;
}

// Returns an std::size_t corresponding to the first occurrence of character in s
// If character is not found, returns an std::size_t pointing to the end of s, i.e. the null character
template <const std::size_t size_>
[[nodiscard]] consteval std::size_t constexpr_strchr(const constexpr_string<size_> &s, const char *character) noexcept
{
    // Loop over the characters, return the index if found
    for (std::size_t i = 0; i < size_; i++)
    {
        if (s[i] == character[0])
        {
            return i;
        }
    }

    // Otherwise return the end of s
    return size_ - 1;
}

// Returns an std::size_t corresponding to the first occurrence of character in s
// If character is not found, returns an std::size_t pointing to the end of s, i.e. the null character
template <const std::size_t size_>
[[nodiscard]] consteval std::size_t constexpr_strchr(const constexpr_string<size_> &s, const char character) noexcept
{
    // Loop over the characters, return the index if found
    for (std::size_t i = 0; i < size_; i++)
    {
        if (s[i] == character)
        {
            return i;
        }
    }

    // Otherwise return the end of s
    return size_ - 1;
}

#endif