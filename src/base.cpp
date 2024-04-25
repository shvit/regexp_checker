#include <cassert>
#include <string>
#include <sstream>
#include <fstream>
#include <regex>

#include "base.hpp"

auto Base::get_metric() -> std::string
{
    size_t bad_metric = 0U;
    for (auto& rule : rules_) {
        if (std::get<bool>(rule)) ++bad_metric;
    }
    assert(bad_metric <= rules_.size()); // never

    std::stringstream ss;
    auto push_metric = [&ss](std::string_view name, auto value) {
            ss << (ss.tellp() > 0 ? "," : "") << name << "=" << value;
        };

    push_metric("name", name_);
    push_metric("errors", errors_.size());
    //push_metric("regexp_count", rules_.size());
    //push_metric("regexp_bad", bad_metric);
    //push_metric("data_chunk_count", data_.size());
    push_metric("checked_regexps", (rules_.size() - bad_metric) * scale_re_);
    push_metric("checked_data_chunks", data_.size() * scale_td_);
    //push_metric("duration", time_.count());

    auto temp_duration = time_;
    auto hours = std::chrono::duration_cast<std::chrono::hours>(temp_duration);
    temp_duration -= hours;
    auto minutes = std::chrono::duration_cast<std::chrono::minutes>(temp_duration);
    temp_duration -= minutes;
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(temp_duration);
    temp_duration -= seconds;
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(temp_duration);
    {
        std::stringstream ss2;
        if (hours.count()) ss2 << hours.count() << "h";
        if (minutes.count() || (ss2.tellp() > 0)) ss2 << minutes.count() << "m";
        ss2 << seconds.count() << "." << milliseconds.count() << "s";
        push_metric("duration", ss2.str());
    }
    {
        std::stringstream ss2;
        size_t sum = 0U;
        for (size_t idx = 0U; idx < metric_ext_.size(); ++idx) {
            ss2 << (ss2.tellp() > 0 ? "," : "") << (idx + 1U) << "{" << (std::get<bool>(rules_[idx]) ? "bad" : std::to_string(metric_ext_[idx])) << "}";
            sum += (std::get<bool>(rules_[idx]) ? 0U : metric_ext_[idx]);
        }
        ss2 << (ss2.tellp() > 0 ? "," : "") << "SUM{" << sum << "}";
        push_metric("rule_metric=", ss2.str());
    }

    return ss.str();
}

bool Base::is_ready_prepare() const
{
    return !errors_.size() && !rules_.size() && !data_.size();
}

bool Base::is_ready_run() const
{
    return !errors_.size() && rules_.size() && data_.size();
}

bool Base::prepare(const fs::path dir_regexp, const fs::path dir_data)
{
    if (!is_ready_prepare()) {
        errors_.emplace_back("Try to prepare when not ready to prepare");
        return false;
    }

    // Load RegExp
    if (fs::is_directory(fs::status(dir_regexp))) {
        for (auto const& iter_dir : fs::recursive_directory_iterator{dir_regexp}) {
            if (fs::is_regular_file(fs::status(iter_dir))) {

                std::ifstream in(std::string{iter_dir.path()});
                if (in.is_open()) {
                    std::string line;
                    while (std::getline(in, line)) {
                        if (!line.empty()) {
                            auto& curr = rules_.emplace_back();
                            std::get<bool>(curr) = false;
                            std::get<std::string>(curr) = line;
                        }
                    }
                    in.close();
                } else {
                    std::stringstream ss;
                    ss << "File " << iter_dir.path() << " not found";
                    errors_.emplace_back(ss.str());
                }
            }
        }
    } else {
        std::stringstream ss;
        ss << "Directory " << dir_regexp << " not found";
        errors_.emplace_back(ss.str());
    }

    // Load data chunks
    if (fs::is_directory(fs::status(dir_data))) {
        for (auto const& iter_dir : fs::recursive_directory_iterator{dir_data}) {
            if (fs::is_regular_file(fs::status(iter_dir))) {
                size_t file_size = fs::file_size(iter_dir.path());
                std::ifstream data_file(iter_dir.path(), std::ios::binary);
                auto& curr = data_.emplace_back(file_size);
                data_file.read(curr.data(), file_size);
                data_file.close();
            }
        }
    } else {
        std::stringstream ss;
        ss << "Directory " << dir_data << " not found";
        errors_.emplace_back(ss.str());
    }

    return is_ready_run();
}

bool Base::run()
{
    if (!is_ready_run()) {
        errors_.emplace_back("Try to run when not ready to run");
        return false;
    }

    if (name_.empty()) {
        if(!init()) {
            errors_.emplace_back("Failed initialize - bad call init()");
            return false;
        }
    }

    // Run
    auto start = std::chrono::steady_clock::now();
    for (size_t iter_re = 0; iter_re < scale_re_; ++iter_re) {
        for (size_t rule_idx = 0U; rule_idx < rules_.size(); ++rule_idx) {
            if (std::get<bool>(rules_[rule_idx])) continue; // bad rule
            try {
                check(rule_idx);
            } catch(...) {
                std::get<bool>(rules_[rule_idx]) = true;
            }
        }
    }

    // Calc time
    time_ = std::chrono::steady_clock::now() - start;

    return true;
}

void Base::push_errors(std::ostream& os) const
{
    for (auto& err_msg : errors_) {
        os << "Error: " << err_msg << std::endl;
    }
}

bool Base::has_errors() const
{
    return errors_.size();
}

auto Base::get_name() const -> const std::string&
{
    return name_;
}