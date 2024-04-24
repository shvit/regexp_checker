#pragma once

#include <chrono>

#include "checker.hpp"

using Duration = std::chrono::nanoseconds;

/** @brief Virtual base class for all regex checkers
*/
class Base {
protected:

    size_t scale_re_{default_scale_regexp}; ///< Common regexp iterations need for test

    size_t scale_td_{default_scale_test_data}; ///< Common data iterations need for test

    RegExpList rules_; ///< Regex rules for testing

    TestData data_; ///< Data chunks for testing

    Errors errors_; ///< All errors list

    Duration time_; ///< Duration of test

    std::string name_; ///< Current regexp name (need will fill in init())

    /** @brief Check is ready for prepare
     *
     * Check class for was not prepared and was not runned and has not errors
     * @return True if ready else false
    */
    bool is_ready_prepare() const;

    /** @brief Check is ready for run
     *
     * Check class for was prepared and was not runned and has not errors
     * @return True if ready else false
    */
    bool is_ready_run() const;

public:

    /** @brief Get string with all metric from test
     *
     * Sample: "name1=value1,name2=value2,name3=value3"
     * @return String with metric
    */
    auto get_metric() -> std::string;

    //void save_metric(std::string_view);

    /** @brief Prepare class for test
     *
     * Load all regexp from directory (recursively)
     * Load all data chunks from directory (recursively)
     * @param [in] dir_regexp DIrectory with regexp
     * @param [in] dir_data DIrectory with data chunks
     * @return True on success and no errors else false
    */
    bool prepare(std::string_view dir_regexp, std::string_view dir_data);

    /** @brief Run test
     *
     * Run for all regexp and for all iterations count
     * Run for all data chunks and for all iterations count
     * @return True on success and no errors else false
    */
    bool run();

    /** @brief Virtual function for initialize
     *
     * Typically used for compile regexp if library is supported
     * Also need set library name to name_
     * @return True on success and no errors else false
    */
    virtual bool init();

    /** @brief Virtual checker for one regexp and for all data chunks
     *
     * By default only for stdlib regexp
     * @param [in] rule_idx RegEx rule index for run
    */
    virtual void check(size_t rule_idx);

    /** @brief Push errors to output stream
     *
     * If no error then nothing to do
     * @param [out] os Output stream
    */
    void push_errors(std::ostream& os) const;

    /** @brief Chek if was any errors
     *
     * @return True if was errors else false
    */
    bool has_errors() const;
};
