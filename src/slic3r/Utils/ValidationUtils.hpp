/**
 * @file ValidationUtils.hpp
 * @brief Safe input validation utilities for numeric parsing
 *
 * This header provides robust validation functions for parsing user input
 * from wxString to numeric types. These functions prevent crashes and
 * undefined behavior from invalid or out-of-range input.
 *
 * @author JusPrin Development Team
 * @date 2024
 */

#ifndef SLIC3R_UTILS_VALIDATIONUTILS_HPP
#define SLIC3R_UTILS_VALIDATIONUTILS_HPP

#include <wx/string.h>
#include <string>
#include <limits>

namespace Slic3r { namespace Utils {

/**
 * @struct ParseResult
 * @brief Result of a parsing operation
 *
 * Contains the parsed value (if successful), success flag, and
 * a human-readable error message for user feedback.
 */
struct ParseResult
{
    bool        success;       ///< True if parsing succeeded
    double      value;         ///< Parsed value (valid only if success=true)
    std::string error_message; ///< User-friendly error description

    /**
     * @brief Construct a successful parse result
     */
    static ParseResult Success(double val) { return ParseResult{true, val, ""}; }

    /**
     * @brief Construct a failed parse result with error message
     */
    static ParseResult Failure(const std::string& error) { return ParseResult{false, 0.0, error}; }
};

/**
 * @brief Safely parse a wxString to double with bounds checking
 *
 * This function provides robust parsing of wxString to double with:
 * - Validation that the string represents a valid number
 * - Bounds checking against min/max values
 * - Clear error messages for all failure cases
 *
 * @param str The string to parse
 * @param min Minimum acceptable value (default: -infinity)
 * @param max Maximum acceptable value (default: +infinity)
 * @return ParseResult containing success status, value, and error message
 *
 * @example
 * auto result = parse_double_safe("3.14", 0.0, 10.0);
 * if (result.success) {
 *     // Use result.value
 * } else {
 *     // Show result.error_message to user
 * }
 */
inline ParseResult parse_double_safe(const wxString& str,
                                     double          min = -std::numeric_limits<double>::max(),
                                     double          max = std::numeric_limits<double>::max())
{
    // Check for empty string
    if (str.IsEmpty()) {
        return ParseResult::Failure("Value cannot be empty");
    }

    // Attempt to parse
    double value;
    if (!str.ToDouble(&value)) {
        return ParseResult::Failure("Invalid numeric value: '" + str.ToStdString() + "'");
    }

    // Check for NaN or infinity
    if (std::isnan(value)) {
        return ParseResult::Failure("Value is not a number (NaN)");
    }

    if (std::isinf(value)) {
        return ParseResult::Failure("Value is infinite");
    }

    // Check bounds
    if (value < min) {
        return ParseResult::Failure("Value " + std::to_string(value) + " is below minimum " + std::to_string(min));
    }

    if (value > max) {
        return ParseResult::Failure("Value " + std::to_string(value) + " exceeds maximum " + std::to_string(max));
    }

    return ParseResult::Success(value);
}

/**
 * @brief Parse a wxString to integer with bounds checking
 *
 * Similar to parse_double_safe but for integer values.
 *
 * @param str The string to parse
 * @param min Minimum acceptable value
 * @param max Maximum acceptable value
 * @return ParseResult (value will be integer but stored as double)
 */
inline ParseResult parse_int_safe(const wxString& str,
                                  long            min = std::numeric_limits<long>::min(),
                                  long            max = std::numeric_limits<long>::max())
{
    if (str.IsEmpty()) {
        return ParseResult::Failure("Value cannot be empty");
    }

    long value;
    if (!str.ToLong(&value)) {
        return ParseResult::Failure("Invalid integer value: '" + str.ToStdString() + "'");
    }

    if (value < min) {
        return ParseResult::Failure("Value " + std::to_string(value) + " is below minimum " + std::to_string(min));
    }

    if (value > max) {
        return ParseResult::Failure("Value " + std::to_string(value) + " exceeds maximum " + std::to_string(max));
    }

    return ParseResult::Success(static_cast<double>(value));
}

}} // namespace Slic3r::Utils

#endif // slic3r_ValidationUtils_hpp_
