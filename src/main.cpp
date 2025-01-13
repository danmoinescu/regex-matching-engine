#include "regex-matching-engine.hpp"
#include <chrono>
#include <iostream>
#include <regex>


void usage(const char *me)
{
    std::cerr << "Usage: " << me << " [-p] pattern string\n";
}


int main(int argc, const char *argv[])
{
    if(argc < 3)
    {
        usage(argv[0]);
        return 1;
    }
    bool run_correctness_test = true;
    if(std::string("-p") == argv[1]) // perf. only
    {
        /* Not interested in a correcness test, just want to
           measure the performance
        */
        run_correctness_test = false;
        --argc;
        ++argv;
        if(argc < 3)
        {
            usage(argv[0]);
            return 1;
        }
    }
    const char *pattern = argv[1];
    const char *tested = argv[2];

    Solution s;
    auto start_time = std::chrono::steady_clock::now();
    bool actual_result = s.isMatch(tested, pattern);
    auto end_time = std::chrono::steady_clock::now();
    auto duration_in_microseconds =
        std::chrono::duration_cast<std::chrono::microseconds>(
                end_time-start_time);
    std::cout << "Matching took " << duration_in_microseconds << "\n";

    bool expected_result;
    if(run_correctness_test)
    {
        std::string full_pattern("^");
        full_pattern += pattern;
        full_pattern += "$";
        std::regex full_regex(full_pattern);
        expected_result = std::regex_match(tested, full_regex);
    }
    else
    {
        expected_result = actual_result;
    }

    int retval;
    if(actual_result == expected_result)
    {
        std::cout << (actual_result? "matches" : "doesn't match") << "\n";
        retval = 0;
    }
    else
    {
        std::cerr << "Oops: I got " << actual_result
            << ", expected " << expected_result << '\n';
        retval = 2;
    }
    return retval;
}
