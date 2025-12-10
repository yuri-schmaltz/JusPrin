#ifndef slic3r_ValidationUtils_hpp_
#define slic3r_ValidationUtils_hpp_

#include <string>
#include <limits>
#include <wx/string.h>

namespace Slic3r {
namespace Utils {

/**
 * @brief Result of a parse operation with validation
 * 
 * Contains the success status, parsed value (if successful), and
 * an error message describing what went wrong (if unsuccessful).
 */
struct ParseResult {
    bool        success;
    double      value;
    std::string error_message;
    
    /**
     * @brief Construct a successful parse result
     */
    static ParseResult Success(double val) {
        return ParseResult{true, val, ""};
    }
    
    /**
     * @brief Construct a failed parse result with error message
     */
    static ParseResult Failure(const std::string& error) {
        return ParseResult{false, 0.0, error};
    }
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
inline ParseResult parse_double_safe(
    const wxString& str,
    double min = -std::numeric_limits<double>::max(),
    double max = std::numeric_limits<double>::max())
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
        return ParseResult::Failure(
            "Value " + std::to_string(value) + " is below minimum " + std::to_string(min)
        );
    }
    
    if (value > max) {
        return ParseResult::Failure(
            "Value " + std::to_string(value) + " exceeds maximum " + std::to_string(max)
        );
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
inline ParseResult parse_int_safe(
    const wxString& str,
    long min = std::numeric_limits<long>::min(),
    long max = std::numeric_limits<long>::max())
{
    if (str.IsEmpty()) {
        return ParseResult::Failure("Value cannot be empty");
    }
    
    long value;
    if (!str.ToLong(&value)) {
        return ParseResult::Failure("Invalid integer value: '" + str.ToStdString() + "'");
    }
    
    if (value < min) {
        return ParseResult::Failure(
            "Value " + std::to_string(value) + " is below minimum " + std::to_string(min)
        );
    }
    
    if (value > max) {
        return ParseResult::Failure(
            "Value " + std::to_string(value) + " exceeds maximum " + std::to_string(max)
        );
    }
    
    return ParseResult::Success(static_cast<double>(value));
}

} // namespace Utils
} // namespace Slic3r

#endif // slic3r_ValidationUtils_hpp_
