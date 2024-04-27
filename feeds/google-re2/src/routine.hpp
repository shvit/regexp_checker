#pragma once

#include "../../../src/checker.hpp"
#include "../../../src/base.hpp"

#include "re2/re2.h"

/** @brief Virtual class for testing regexp
 * 
 * Testing regexp from https://github.com/ximtech/Regex
 * Used base virtual class Base
*/
class Routine: public Base {
protected:

    std::vector<std::shared_ptr<RE2>> comp_rules_; ///< Storage for compiled regexp

public:

    /** @brief Virtual function for initialize
     *
     * See description in class Base
    */
    virtual bool init() override {
        for (size_t idx = 0U; idx < rules_.size(); ++idx) {
            auto& curr = comp_rules_.emplace_back(std::make_shared<RE2>(std::get<std::string>(rules_[idx])));
            if (curr.get() && !curr->ok()) {
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
                std::string s;
                if (RE2::FullMatch(absl::string_view(data.data(),
                                                     data.size()),
                                   *comp_rules_[rule_idx],
                                   &s)) {
                    ++metric_ext_[rule_idx];
                }
            }
        }
    }

};
