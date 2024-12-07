CXX = g++
CXXSTANDARD = -std=c++23
OPTFLAGS = -O3 -flto -fwhole-program
MFLAGS = -march=native -mtune=native
WFLAGS = -Werror -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wformat-security -Winline -Wformat=2 -Wattributes \
-Wbuiltin-macro-redefined -Wcast-align -Wdiv-by-zero -Wdouble-promotion -Wfloat-equal -Wint-to-pointer-cast -Wlogical-op -Woverflow \
-Wpointer-arith -Wredundant-decls -Wshadow -Wsign-promo -Wwrite-strings -Wimplicit-fallthrough=5 -Wstringop-overflow=4 -Wstrict-aliasing=3

CXXFLAGS = $(OPTFLAGS) $(CXXSTANDARD) $(MFLAGS) $(WFLAGS) $(EXTRACXXFLAGS)

default:
	clear
	make clean
	$(CXX) $(CXXFLAGS) stringTest.C -o stringTest

clean:
	rm -rf stringTest