#define CATCH_CONFIG_MAIN
#define _SILENCE_CXX17_UNCAUGHT_EXCEPTION_DEPRECATION_WARNING

#include "catch.hpp"

#include <iostream>
#include <string>

unsigned int Factorial( unsigned int number ) {
    return number <= 1 ? number : Factorial(number-1)*number;
}

TEST_CASE( "Factorials are computed", "[factorial]" ) {
    REQUIRE( Factorial(1) == 1 );
    REQUIRE( Factorial(2) == 2 );
    REQUIRE( Factorial(3) == 6 );
    REQUIRE( Factorial(10) == 3628800 );
}
