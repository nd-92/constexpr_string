#include <iostream>
#include "constexpr_string.H"

constexpr const std::integral_constant<uint64_t, 1434525> i_pos;
constexpr const std::integral_constant<uint64_t, 0> i_0;
constexpr const std::integral_constant<int64_t, -12341> j_neg;
constexpr const std::integral_constant<int64_t, 0> j_0;
constexpr const std::integral_constant<int64_t, 423341> j_pos;
constexpr const constexpr_string buffer =
    constexpr_string("This is the generated test string.") //
    + "\n"                                                 //
    + "The converted integral constants are: "             //
    + i_0 + ", "                                           //
    + i_pos + ", "                                         //
    + j_neg + ", "                                         //
    + j_0 + ", "                                           //
    + j_pos + "\n"                                         //
    + "This string compiling successfully tests both the addition operator and the conversion of integral_constants to constexpr_string objects.";

int main()
{

    std::cout << buffer << std::endl;

    return 0;
}