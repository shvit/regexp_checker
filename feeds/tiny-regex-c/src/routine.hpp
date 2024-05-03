#pragma once

#include "../../../src/checker.hpp"
#include "../../../src/base.hpp"

extern "C" {
    #include "../repo/re.h"

    re_t re_compile(const char* pattern);
    int re_matchp(re_t pattern, const char* text, int* matchlength);
}

/** @brief Virtual class for testing regexp
 * 
 * Testing regexp from https://github.com/kokke/tiny-regex-c
 * Used base virtual class Base
*/
class Routine: public Base {
protected:

    std::vector<re_t> comp_rules_; ///< Storage for compiled regexp

public:

    /** @brief Virtual function for initialize
     *
     * Typically used for compile regexp if library is supported
     * Also need set library name to name_
     * @return True on success and no errors else false
    */
    virtual bool init() override {
        comp_rules_.reserve(rules_.size());
        for (size_t idx = 0U; idx < comp_rules_.size(); ++idx) {
            comp_rules_[idx] = re_compile(std::get<std::string>(rules_[idx]).c_str());
            if (!comp_rules_[idx]) {
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
        int dummy = 0;
        for (size_t iter_td = 0; iter_td < scale_td_; ++iter_td) {
            for (auto& data : data_) {
                if (re_matchp(comp_rules_[rule_idx], data.data(), &dummy) > -1) {
                    ++metric_ext_[rule_idx];
                }
            }
        }
    }

};
