#include "regex-matching-engine.hpp"
#include <regex>
#include <string>

#define TEST_PASSED 0
#define TEST_FAILURE -1


static bool expected_result(const char *str, const char *pattern)
{
    // match against the full string (^pattern$)
    std::string full_pattern("^");
    full_pattern += pattern;
    full_pattern += "$";
    return std::regex_match(str, std::regex(full_pattern));
}


static int compare_to_expected_result(
        const char *str, const char *pattern)
{
    Solution s;
    return s.isMatch(str, pattern) == expected_result(str, pattern)?
        TEST_PASSED : TEST_FAILURE;
}


int test1()
{
    return compare_to_expected_result("aaa", ".*");
}
