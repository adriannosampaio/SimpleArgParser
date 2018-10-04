#define CATCH_CONFIG_MAIN
#include "catch2/catch.cpp"
#include "../SinpleArgParser.hpp"

TEST_CASE()
{
    /// Test no value argument (--debug)
    int argc = 2;

    char *argv[] = {
        "program",
        "--debug"
    };

    parser::ArgumentParser arg(argc, argv);

    arg.addArgument("--debug", false, "for debug messages");

    arg.parse();

    REQUIRE(arg.isDefined("--debug") == true);
    REQUIRE(arg.isDefined("--debug") == false);

}


