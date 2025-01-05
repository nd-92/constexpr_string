#include <iostream>
#include "constexpr_string.H"

int main()
{

    static constexpr const std::integral_constant<uint64_t, 1434525> i_pos;
    static constexpr const std::integral_constant<uint64_t, 0> i_0;
    static constexpr const std::integral_constant<int64_t, -12341> j_neg;
    static constexpr const std::integral_constant<int64_t, 0> j_0;
    static constexpr const std::integral_constant<int64_t, 423341> j_pos;
    static constexpr const constexpr_string buffer =
        constexpr_string("This is the generated test string.") //
        + "\n"                                                 //
        + "The converted integral constants are: "             //
        + i_0 + ", "                                           //
        + i_pos + ", "                                         //
        + j_neg + ", "                                         //
        + j_0 + ", "                                           //
        + j_pos + "\n"                                         //
        + "This string compiling successfully tests both the " //
        + "addition operator and the conversion of "           //
        + "integral_constants to constexpr_string objects.";   //

    std::cout << buffer << std::endl;

    std::cout << "Buffer has length " << buffer.length() << std::endl;

    // Search for a character within buffer
    constexpr const char *t = "t";
    constexpr const std::integral_constant<uint64_t, constexpr_strchr(buffer, t)> bufferContains;

    // Print it out as a constexpr_string
    std::cout << constexpr_string("Found ") + buffer(bufferContains) + " at element number " + bufferContains << std::endl;

    return 0;
}