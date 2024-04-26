#pragma once

#include <regex>
#include <boost/regex.hpp>

#include "../../../src/checker.hpp"
#include "../../../src/base.hpp"

/** @brief Virtual class for testing regexp
 * 
 * Testing regexp from https://github.com/ximtech/Regex
 * Used base virtual class Base
*/
class Routine: public Base {
protected:

    std::vector<boost::regex> comp_rules_; ///< Storage for compiled regexp

public:

    /** @brief Virtual function for initialize
     *
     * See description in class Base
    */
    virtual bool init() override {
        name_ = "boost-compiled";
        for (size_t idx = 0U; idx < rules_.size(); ++idx) {
            try {
                comp_rules_.emplace_back(std::get<std::string>(rules_[idx]));
            } catch (...) {
                std::get<bool>(rules_[idx]) = true;
            }
        }
        return true;
    }

    /** @brief Virtual checker run
     *
     * See description in class Base
    */
    virtual void check(size_t rule_idx) override {
        for (size_t iter_td = 0; iter_td < scale_td_; ++iter_td) {
            for (auto& data : data_) {
                if (boost::regex_match(data.cbegin(), data.cend(), comp_rules_[rule_idx])) {
                    ++metric_ext_[rule_idx];
                }
            }
        }
    }

};
