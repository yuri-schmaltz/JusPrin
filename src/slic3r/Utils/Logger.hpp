/**
 * @file Logger.hpp
 * @brief Thread-safe logging system for JusPrin
 *
 * Provides a centralized, structured logging facility with multiple
 * log levels, file output, and console output. Thread-safe for use
 * in multi-threaded environments.
 *
 * @author JusPrin Development Team
 * @date 2024
 */

#ifndef SLIC3R_UTILS_LOGGER_HPP
#define SLIC3R_UTILS_LOGGER_HPP

#include <string>
#include <fstream>
#include <mutex>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <iostream>

namespace Slic3r {

/**
 * @enum LogLevel
 * @brief Severity levels for log messages
 */
enum class LogLevel {
    ERROR,   ///< Critical errors requiring immediate attention
    WARNING, ///< Warnings about potential issues
    INFO,    ///< Informational messages about normal operation
    DEBUG    ///< Detailed debug information for development
};

/**
 * @brief Structured logging system for JusPrin
 *
 * Provides centralized logging with:
 * - Multiple log levels (ERROR, WARNING, INFO, DEBUG)
 * - Thread-safe file and console output
 * - Component-based categorization
 * - Timestamp on every log entry
 * - Optional log file rotation
 */
class LoggerInstance
{
private:
    std::ofstream log_file_;
    std::mutex    log_mutex_;
    LogLevel      min_level_;
    bool          console_output_;
    std::string   log_file_path_;

    std::string get_timestamp()
    {
        auto now  = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        auto ms   = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

        std::stringstream ss;
        ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
        ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
        return ss.str();
    }

    std::string level_to_string(LogLevel level)
    {
        switch (level) {
        case LogLevel::ERROR: return "ERROR";
        case LogLevel::WARNING: return "WARN ";
        case LogLevel::INFO: return "INFO ";
        case LogLevel::DEBUG: return "DEBUG";
        default: return "UNKN ";
        }
    }

    void write_log(LogLevel level, const std::string& component, const std::string& message)
    {
        if (level < min_level_)
            return;

        std::lock_guard<std::mutex> lock(log_mutex_);

        std::string log_entry = "[" + get_timestamp() + "] [" + level_to_string(level) + "] [" + component + "] " + message + "\n";

        // Write to file if open
        if (log_file_.is_open()) {
            log_file_ << log_entry;
            log_file_.flush();
        }

        // Write to console if enabled
        if (console_output_) {
            if (level == LogLevel::ERROR) {
                std::cerr << log_entry;
            } else {
                std::cout << log_entry;
            }
        }
    }

public:
    LoggerInstance() : min_level_(LogLevel::INFO), console_output_(true) {}

    ~LoggerInstance()
    {
        if (log_file_.is_open()) {
            log_file_.close();
        }
    }

    /**
     * @brief Initialize logger with log file path
     *
     * @param filepath Path to log file (e.g., "~/.config/jusprin/jusprin.log")
     * @param min_level Minimum level to log (default: INFO)
     * @param console Whether to also output to console (default: true)
     */
    void init(const std::string& filepath, LogLevel min_level = LogLevel::INFO, bool console = true)
    {
        std::lock_guard<std::mutex> lock(log_mutex_);

        log_file_path_  = filepath;
        min_level_      = min_level;
        console_output_ = console;

        log_file_.open(filepath, std::ios::app);
        if (!log_file_.is_open()) {
            std::cerr << "Failed to open log file: " << filepath << std::endl;
        }
    }

    void error(const std::string& component, const std::string& message) { write_log(LogLevel::ERROR, component, message); }

    void warning(const std::string& component, const std::string& message) { write_log(LogLevel::WARNING, component, message); }

    void info(const std::string& component, const std::string& message) { write_log(LogLevel::INFO, component, message); }

    void debug(const std::string& component, const std::string& message) { write_log(LogLevel::DEBUG, component, message); }

    void set_level(LogLevel level)
    {
        std::lock_guard<std::mutex> lock(log_mutex_);
        min_level_ = level;
    }

    void set_console_output(bool enabled)
    {
        std::lock_guard<std::mutex> lock(log_mutex_);
        console_output_ = enabled;
    }
};

// Global logger instance
inline LoggerInstance& get_logger()
{
    static LoggerInstance logger;
    return logger;
}

// Convenience functions
inline void error(const std::string& component, const std::string& message) { get_logger().error(component, message); }

inline void warning(const std::string& component, const std::string& message) { get_logger().warning(component, message); }

inline void info(const std::string& component, const std::string& message) { get_logger().info(component, message); }

inline void debug(const std::string& component, const std::string& message) { get_logger().debug(component, message); }

inline void init(const std::string& filepath, LogLevel min_level = LogLevel::INFO, bool console = true)
{
    get_logger().init(filepath, min_level, console);
}

}
} // namespace Slic3r::Logger

#endif // slic3r_Logger_hpp_
