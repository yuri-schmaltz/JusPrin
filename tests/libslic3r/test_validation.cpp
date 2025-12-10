#include <catch2/catch.hpp>
#include "../../src/slic3r/Utils/ValidationUtils.hpp"
#include <limits>

using namespace Slic3r::Utils;

TEST_CASE("parse_double_safe validates input correctly", "[validation]")
{
    SECTION("Valid positive number")
    {
        auto result = parse_double_safe("3.14");
        REQUIRE(result.success == true);
        REQUIRE(result.value == Approx(3.14));
        REQUIRE(result.error_message.empty());
    }

    SECTION("Valid negative number")
    {
        auto result = parse_double_safe("-2.5");
        REQUIRE(result.success == true);
        REQUIRE(result.value == Approx(-2.5));
    }

    SECTION("Valid integer")
    {
        auto result = parse_double_safe("42");
        REQUIRE(result.success == true);
        REQUIRE(result.value == Approx(42.0));
    }

    SECTION("Valid zero")
    {
        auto result = parse_double_safe("0");
        REQUIRE(result.success == true);
        REQUIRE(result.value == Approx(0.0));
    }

    SECTION("Invalid input - letters")
    {
        auto result = parse_double_safe("abc");
        REQUIRE(result.success == false);
        REQUIRE(!result.error_message.empty());
    }

    SECTION("Invalid input - mixed")
    {
        auto result = parse_double_safe("12abc");
        REQUIRE(result.success == false);
    }

    SECTION("Invalid input - empty")
    {
        auto result = parse_double_safe("");
        REQUIRE(result.success == false);
    }

    SECTION("Out of range - above max")
    {
        auto result = parse_double_safe("100", 0.0, 50.0);
        REQUIRE(result.success == false);
        REQUIRE(result.error_message.find("50") != std::string::npos);
    }

    SECTION("Out of range - below min")
    {
        auto result = parse_double_safe("-10", 0.0, 100.0);
        REQUIRE(result.success == false);
        REQUIRE(result.error_message.find("0") != std::string::npos);
    }

    SECTION("Within range")
    {
        auto result = parse_double_safe("25", 0.0, 50.0);
        REQUIRE(result.success == true);
        REQUIRE(result.value == Approx(25.0));
    }

    SECTION("Edge case - exactly at min")
    {
        auto result = parse_double_safe("0", 0.0, 100.0);
        REQUIRE(result.success == true);
        REQUIRE(result.value == Approx(0.0));
    }

    SECTION("Edge case - exactly at max")
    {
        auto result = parse_double_safe("100", 0.0, 100.0);
        REQUIRE(result.success == true);
        REQUIRE(result.value == Approx(100.0));
    }

    SECTION("Scientific notation")
    {
        auto result = parse_double_safe("1.23e-4");
        REQUIRE(result.success == true);
        REQUIRE(result.value == Approx(0.000123));
    }

    SECTION("Very large number")
    {
        auto result = parse_double_safe("1e10");
        REQUIRE(result.success == true);
        REQUIRE(result.value == Approx(1e10));
    }
}

TEST_CASE("parse_int_safe validates integer input", "[validation]")
{
    SECTION("Valid positive integer")
    {
        auto result = parse_int_safe("42");
        REQUIRE(result.success == true);
        REQUIRE(result.value == 42);
    }

    SECTION("Valid negative integer")
    {
        auto result = parse_int_safe("-15");
        REQUIRE(result.success == true);
        REQUIRE(result.value == -15);
    }

    SECTION("Invalid - decimal")
    {
        auto result = parse_int_safe("3.14");
        REQUIRE(result.success == false);
    }

    SECTION("Invalid - letters")
    {
        auto result = parse_int_safe("abc");
        REQUIRE(result.success == false);
    }

    SECTION("Out of range integer")
    {
        auto result = parse_int_safe("150", 0, 100);
        REQUIRE(result.success == false);
    }

    SECTION("Within range integer")
    {
        auto result = parse_int_safe("50", 0, 100);
        REQUIRE(result.success == true);
        REQUIRE(result.value == 50);
    }
}

TEST_CASE("Boundary conditions", "[validation][boundary]")
{
    SECTION("Maximum double value")
    {
        auto max_str = std::to_string(std::numeric_limits<double>::max());
        auto result  = parse_double_safe(max_str);
        REQUIRE(result.success == true);
    }

    SECTION("Minimum double value")
    {
        auto min_str = std::to_string(std::numeric_limits<double>::lowest());
        auto result  = parse_double_safe(min_str);
        REQUIRE(result.success == true);
    }

    SECTION("Whitespace handling")
    {
        auto result = parse_double_safe("  3.14  ");
        REQUIRE(result.success == true);
        REQUIRE(result.value == Approx(3.14));
    }
}
