#include <iostream>
#include <cstring>
#include <cstdint>
#include "constexpr_string.H"

int main()
{
    // constexpr const std::integral_constant<unsigned int, 124701383> i;
    // static constexpr const constexpr_string buffer = constexpr_string(integral_to_char_array(std::integral_constant<uint64_t, 124701383>()));
    // static constexpr const constexpr_string buffer = constexpr_string(std::integral_constant<uint64_t, 124701383>());

    constexpr const std::integral_constant<uint64_t, 1434525> i;

    std::cout << i() << " has string length " << constexpr_strlen(i) << std::endl;

    static constexpr const constexpr_string buffer = constexpr_string("Buffer: ") + i + constexpr_string(", which seems correct");

    std::cout << buffer.string_view() << std::endl;

    // std::cout << "Buffer:" << std::endl;
    // for (std::size_t k = 0; k < buffer.size() - 1; k++)
    // {
    //     std::cout << buffer[k];
    // }
    // std::cout << std::endl;

    // std::cout << "foo(0) = " << constexpr_strlen(-2141) << std::endl;

    // std::cout << "constexpr_string s_1 = " << s_1.string_view() << std::endl;
    // std::cout << "constexpr_string s_2 = " << s_2.string_view() << std::endl;
    // std::cout << "constexpr_string s_3 = " << s_3.string_view() << std::endl;
    // std::cout << "constexpr_string s_4 = " << s_4.string_view() << std::endl;

    // std::cout << "const char *S_1 = " << S_1 << std::endl;
    // std::cout << "const char *S_2 = " << S_2 << std::endl;
    // std::cout << "const char *S_3 = " << S_3 << std::endl;
    // std::cout << "const char *S_4 = " << S_4 << std::endl;

    // constexpr const int i_12 = constexpr_strcmp(s_1, s_2);
    // constexpr const int i_23 = constexpr_strcmp(s_2, s_3);
    // constexpr const int i_24 = constexpr_strcmp(s_2, s_4);
    // constexpr const int i_42 = constexpr_strcmp(s_4, s_2);
    // std::cout << "constexpr_strcmp(s_1, s_2) = " << i_12 << std::endl;
    // std::cout << "constexpr_strcmp(s_2, s_3) = " << i_23 << std::endl;
    // std::cout << "constexpr_strcmp(s_2, s_4) = " << i_24 << std::endl;
    // std::cout << "constexpr_strcmp(s_4, s_2) = " << i_42 << std::endl;

    // const int j_12 = std::strcmp(S_1.c_str(), S_2);
    // const int j_23 = std::strcmp(S_2, S_3);
    // const int j_24 = std::strcmp(S_2, S_4);
    // const int j_42 = std::strcmp(S_4, S_2);
    // std::cout << "std::strcmp(S_1, S_2) = " << j_12 << std::endl;
    // std::cout << "std::strcmp(S_2, S_3) = " << j_23 << std::endl;
    // std::cout << "std::strcmp(S_2, S_4) = " << j_24 << std::endl;
    // std::cout << "std::strcmp(S_4, S_2) = " << j_42 << std::endl;

    // constexpr const constexpr_string s_1 = constexpr_string(to_string<30, 10>);
    // std::cout << s_1.string_view() << std::endl;

    return 0;
}