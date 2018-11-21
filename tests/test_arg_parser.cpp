#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"
#include "../SimpleArgParser.hpp"

TEST_CASE("Testing expected flag with no value")
{
    
    SECTION("Flag defined via command line"){
        
        /// Test no value argument (--debug)
        int argc = 2;

        char *argv[] = {
            "program",
            "--debug"
        };
        
        // Creating the parser instance
        parser::ArgumentParser argParser(argc, argv);

        // Including a new argument to parse via command line
        argParser.addArgument(
            "--debug", 
            false, 
            "for debug messages");

        argParser.parse();

        REQUIRE(argParser.isDefined("--debug"));
    }

    SECTION("Flag not defined by command line"){
        int argc2 = 1;
    
        char *argv2[] = {
            "program"
        };
    
        parser::ArgumentParser argParser2(argc2, argv2);

        // Including a new argument to parse via command line
        argParser2.addArgument(
            "--debug", 
            false, 
            "for debug messages");
        
        argParser2.parse();
    
        REQUIRE(!argParser2.isDefined("--debug"));
    }
}

TEST_CASE("Testing expected flag with value")
{
    
    SECTION("Integer value defined via command line"){
        
        /// Test no value argument (--debug)
        int argc = 3;

        char *argv[] = {
            "program",
            "--size",
            "2"
        };
        
        // Creating the parser instance
        parser::ArgumentParser argParser(argc, argv);

        // Including a new argument to parse via command line
        argParser.addArgument(
            "--size", 
            true, 
            "Size of the given vector");

        argParser.parse();

        REQUIRE(argParser.isDefined("--size"));
        REQUIRE(argParser.hasValue("--size"));

        auto size = argParser.getArgument<int>("--size");

        REQUIRE(size == 2);

    }


    SECTION("Float/double value defined via command line"){
        
        using namespace Catch::literals;

        /// Test no value argument (--debug)
        int argc = 3;

        char *argv[] = {
            "program",
            "--mass",
            "35.234"
        };
        
        // Creating the parser instance
        parser::ArgumentParser argParser(argc, argv);

        // Including a new argument to parse via command line
        argParser.addArgument(
            "--mass", 
            true, 
            "Mass of that thing used by that other thing");

        argParser.parse();

        REQUIRE(argParser.isDefined("--mass"));
        REQUIRE(argParser.hasValue("--mass"));

        auto mass_flt = argParser.getArgument<float>("--mass");
        
        REQUIRE(mass_flt == 35.234_a);

        auto mass_dbl = argParser.getArgument<double>("--mass");
        
        REQUIRE(mass_dbl == 35.234_a);

    }


}


