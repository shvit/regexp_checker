#pragma once

#include <optional>
//#include <iostream>

#include "../../../src/checker.hpp"
#include "../../../src/base.hpp"

#include "hs.h"
#include "database.h"
#include "scratch.h"

/** @brief Virtual class for testing regexp
 * 
 * Testing regexp from https://github.com/intel/hyperscan
 * Used base virtual class Base
*/
class Routine: public Base {
protected:

    std::vector<std::tuple<hs_database_t*, hs_scratch_t*>> comp_rules_; ///< Storage for compiled regexp

public:
    static size_t last_idx;

    static int eventHandler(unsigned int id, unsigned long long from,
                        unsigned long long to, unsigned int flags, void *ctx) {
        //std::cout << "!!! event: id=" << id << ", from=" << from << ", to=" << to << std::endl;
        ++Routine::last_idx;
        return 0;
    }

    ~Routine() {
        for (size_t idx = 0U; idx < comp_rules_.size(); ++idx) {
            if (!std::get<1>(comp_rules_[idx])) {
                hs_free_scratch(std::get<1>(comp_rules_[idx]));
            }
            if (!std::get<0>(comp_rules_[idx])) {
                hs_free_database(std::get<0>(comp_rules_[idx]));
            }
        }
    }

    /** @brief Virtual function for initialize
     *
     * See description in class Base
    */
    virtual bool init() override {
        comp_rules_.resize(rules_.size());
        for (size_t idx = 0U; idx < rules_.size(); ++idx) {
            hs_database_t *database;
            hs_compile_error_t *compile_err;
            if (hs_compile(std::get<std::string>(rules_[idx]).c_str(), HS_FLAG_DOTALL | HS_FLAG_SOM_LEFTMOST, HS_MODE_BLOCK, NULL, &database, &compile_err) != HS_SUCCESS) {
                hs_free_compile_error(compile_err);
                 std::get<bool>(rules_[idx]) = true;
                 continue;
            } else {
                hs_scratch_t *scratch = NULL;
                if (hs_alloc_scratch(database, &scratch) != HS_SUCCESS) {
                    hs_free_database(database);
                    std::get<bool>(rules_[idx]) = true;
                     continue;
                }
                std::get<0>(comp_rules_[idx]) = database;
                std::get<1>(comp_rules_[idx]) = scratch;
            }
        }
        for (size_t idx = 0U; idx < comp_rules_.size(); ++idx) {
            if (!std::get<0>(comp_rules_[idx]) || !std::get<1>(comp_rules_[idx])) {
                std::get<bool>(rules_[idx]) = true;
                continue;
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
                Routine::last_idx = 0U;
                hs_scan(std::get<0>(comp_rules_[rule_idx]), 
                        data.data(), 
                        data.size(), 
                        0, 
                        std::get<1>(comp_rules_[rule_idx]), 
                        &Routine::eventHandler, 
                        NULL);
                //std::cout << "*** Iteration idx=" << rule_idx << ": counter=" << Routine::last_idx << std:: endl;
                if (Routine::last_idx) {
                    metric_ext_[rule_idx] += Routine::last_idx;
                }
            }
        }
    }

};

size_t Routine::last_idx;