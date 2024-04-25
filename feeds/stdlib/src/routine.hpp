#pragma once

#include <regex>

#include "../../../src/checker.hpp"
#include "../../../src/base.hpp"

/** @brief Virtual class for testing regexp
 * 
 * Testing regexp from https://github.com/ximtech/Regex
 * Used base virtual class Base
*/
class Routine: public Base {
protected:

    std::vector<std::regex> comp_rules_; ///< Storage for compiled regexp

public:

    /** @brief Virtual function for initialize
     *
     * See description in class Base
    */
    virtual bool init() override {
        name_ = "stdlib";
        metric_ext_.resize(rules_.size());
        return true;
    }

    /** @brief Virtual checker run
     *
     * See description in class Base
    */
    virtual void check(size_t rule_idx) override {
        const std::regex re{std::get<std::string>(rules_[rule_idx])};
        for (size_t iter_td = 0; iter_td < scale_td_; ++iter_td) {
            for (auto& data : data_) {
                if (std::regex_match(data.cbegin(), data.cend(), re)) {
                    ++metric_ext_[rule_idx];
                }
            }
        }
    }

};
