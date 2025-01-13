#include "regex-matching-engine.hpp"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <ranges>
#include <regex>
#include <string>

#define TEST_PASSED 0
#define TEST_FAILURE -1

using namespace std::chrono_literals; // for constructs like 1ms or 2us


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


template<typename Duration=std::chrono::nanoseconds>
static int compare_to_expected_result(
        const char *str, const char *pattern,
        const Duration &max_duration=std::chrono::nanoseconds::zero())
{
    auto start_time = std::chrono::steady_clock::now();
    Solution s;
    bool solution_result = s.isMatch(str, pattern);
    auto end_time = std::chrono::steady_clock::now();

    if(
            (max_duration.count() > 0) &&
            (end_time - start_time > max_duration))
    {
        auto duration_in_microseconds =
            std::chrono::duration_cast<std::chrono::microseconds>(
                    end_time-start_time);
        std::cerr << "Warning: "
            << pattern << " against " << str
            << ": took " << duration_in_microseconds << "\n";
    }

    bool std_result = std_match(str, pattern);
    if(solution_result == std_result)
    {
        return TEST_PASSED;
    }
    else
    {
        std::cerr << pattern << " against " << str
            << ": expected " << std_result
            << ", got " << solution_result << '\n';

        return TEST_FAILURE;
    }
}


template<typename Duration>
static int compare_to_expected_result(
        const std::string &str, const std::string &pattern,
        const Duration &max_duration)
{
    return compare_to_expected_result(
            str.c_str(), pattern.c_str(), max_duration);
}


int test1()
{
    return compare_to_expected_result("aaa", ".*");
}


int test2()
{
    return compare_to_expected_result("aaa", "a*");
}


int test3()
{
    return compare_to_expected_result("aaa", "ab*");
}


int random_tests_ab()
{
    bool all_passed = std::ranges::all_of(
            std::views::iota(0, 300),
            [](int)
            {
                return compare_to_expected_result(
                        random_ab_string(), random_ab_pattern(),
                        1ms) // defined thanks to std::chrono_literals
                    == TEST_PASSED;
            });
    return all_passed? TEST_PASSED : TEST_FAILURE;
}
