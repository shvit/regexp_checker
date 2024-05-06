#pragma once

#include "../../../src/checker.hpp"
#include "../../../src/base.hpp"

extern "C" {
    #include "../repo/include/Regex.h"

    void regexCompile(Regex *regex, const char *pattern);
    Matcher regexMatch(Regex *regex, const char *text);
}

/** @brief Virtual class for testing regexp
 * 
 * Testing regexp from https://github.com/ximtech/Regex
 * Used base virtual class Base
*/
class Routine: public Base {
protected:

    std::vector<Regex> comp_rules_; ///< Storage for compiled regexp

public:

    /** @brief Virtual function for initialize
     *
     * Typically used for compile regexp if library is supported
     * Also need set library name to name_
     * @return True on success and no errors else false
    */
    virtual bool init() override {
        comp_rules_.resize(rules_.size());
        for (size_t idx = 0U; idx < comp_rules_.size(); ++idx) {
            regexCompile(&comp_rules_[idx], std::get<std::string>(rules_[idx]).c_str());
            if (!comp_rules_[idx].isPatternValid) {
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
                char *pos = data.data();
                Matcher ret;
                while ((ret = regexMatch(&comp_rules_[rule_idx],pos)).isFound) {
                    ++metric_ext_[rule_idx];
                    pos += ret.foundAtIndex + ret.matchLength;
                }
            }
        }
    }

};
