#include "regex-matching-engine.hpp"
#include <iostream>
#include <random>
#include <regex>
#include <string>

#define TEST_PASSED 0
#define TEST_FAILURE -1


std::minstd_rand rng(std::random_device{}());


static std::string random_ab_string()
{
    std::uniform_int_distribution<> dist(1, 11);
    std::string s;
    s.reserve(20);
    for(unsigned i=0; i<s.capacity(); ++i)
    {
        s.push_back(dist(rng) >= 7? 'a' : 'b'); // 70% a, 30% b
    }
    return s;
}


static std::string random_ab_pattern()
{
    std::uniform_int_distribution<> dist3(0, 3);
    std::uniform_int_distribution<> dist10(0, 10);
    std::string pattern;
    const int max_size = 20;
    pattern.reserve(max_size+1);
    int current_size=0;
    while(current_size<max_size)
    {
        char v;
        switch(dist3(rng))
        {
            case 0:
                v = 'a';
                break;
            case 1:
                v = 'b';
                break;
            default:
                v = '.';
                break;
        }
        pattern.push_back(v);
        ++current_size;
        if(dist10(rng) >= 2)
        {
            pattern.push_back('*');
            ++current_size;
        }
    }
    pattern.resize(max_size);
    return pattern;
}


/* Return true if the standard regex engine matches the
   passed string against the given pattern
*/
static bool std_match(const char *str, const char *pattern)
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
    bool solution_result = s.isMatch(str, pattern);
    bool std_result = std_match(str, pattern);
    if(solution_result == std_result)
    {
        return TEST_PASSED;
    }
    else
    {
        std::cerr << str << " against " << pattern
            << ": expected " << std_result
            << ", got " << solution_result << '\n';

        return TEST_FAILURE;
    }
}


static int compare_to_expected_result(
        const std::string &str, const std::string &pattern)
{
    return compare_to_expected_result(str.c_str(), pattern.c_str());
}


static int compare_to_expected_result(
        const char *str, const char *pattern, bool should_match)
{
    Solution s;
    bool solution_result = s.isMatch(str, pattern);
    bool std_result = std_match(str, pattern);
    if(solution_result == std_result &&
            solution_result == should_match)
    {
        return TEST_PASSED;
    }
    else
    {
        std::cerr << str << " against " << pattern
            << ": solution: " << solution_result
            << ", std: " << std_result << ", "
            << ", expected: " << should_match << '\n';

        return TEST_FAILURE;
    }
}


int test1()
{
    return compare_to_expected_result("aaa", ".*", true);
}


int test2()
{
    return compare_to_expected_result("aaa", "a*", true);
}


int test3()
{
    return compare_to_expected_result("aaa", "ab*", false);
}


int random_tests_ab()
{
    for(int i=1; i<= 300; ++i)
    {
        if(compare_to_expected_result(
                    random_ab_string(), random_ab_pattern())
                == TEST_FAILURE)
        {
            return TEST_FAILURE;
        }
    }
    return TEST_PASSED;
}

/* Failures:
   a*aa*.a*b*.*a*.*a*.* baaaabbbbabbababababaabaabbabb
*/
