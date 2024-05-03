#pragma once

#include "../../../src/checker.hpp"
#include "../../../src/base.hpp"

#include "re2/re2.h"

/** @brief Virtual class for testing regexp
 * 
 * Testing regexp from https://github.com/google/re2
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
                continue;
            }
            //if (comp_rules_[idx]->NumberOfCapturingGroups() == 0) {
                //std::string  temp_str;
                //temp_str.append("(").append(std::get<std::string>(rules_[idx])).append(")");
                //temp_str.append(std::get<std::string>(rules_[idx]));
            //    curr = std::make_shared<RE2>(std::get<std::string>(rules_[idx]));
            //    if (curr.get() && !curr->ok()) {
            //        std::get<bool>(rules_[idx]) = true;
            //        continue;
            //    }
            //}
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
                absl::string_view temp_str{data.data(), data.size()};
                while (RE2::FindAndConsumeN(&temp_str,
                                     *comp_rules_[rule_idx].get(),
                                     NULL,
                                     0)) {
                    ++metric_ext_[rule_idx];
                }
            }
        }
    }

};
