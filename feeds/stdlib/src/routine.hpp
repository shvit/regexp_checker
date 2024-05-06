#pragma once

#include <regex>

#include "../../../src/checker.hpp"
#include "../../../src/base.hpp"

/** @brief Virtual class for testing regexp
 * 
 * Testing regexp from C++ STL
 * Used base virtual class Base
*/
class Routine: public Base {
protected:

    std::vector<std::regex> comp_rules_; ///< Storage for compiled regexp

public:

    /** @brief Virtual function for initialize
     *
     * Typically used for compile regexp if library is supported
     * Also need set library name to name_
     * @return True on success and no errors else false
    */
    virtual bool init() override {
        for (size_t idx = 0U; idx < rules_.size(); ++idx) {
            try {
                comp_rules_.emplace_back(std::get<std::string>(rules_[idx]));
            } catch (...) {
                std::get<bool>(rules_[idx]) = true;
            }
        }
        return true;
    }

    /** @brief Virtual checker for one regexp and for all data chunks
     *
     * By default only for stdlib regexp
     * @param [in] rule_idx RegEx rule index for run
    */
    virtual void check(size_t rule_idx) override {
        for (size_t iter_td = 0; iter_td < scale_td_; ++iter_td) {
            for (auto& data : data_) {
                std::string data_str(data.data(), data.size());
                for (std::smatch sm; std::regex_search(data_str, sm, comp_rules_[rule_idx]);) {
                    ++metric_ext_[rule_idx];
                    data_str = sm.suffix();
                }
            }
        }
    }

};
