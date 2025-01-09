#include "regex-matching-engine.hpp"
#include <iostream>
#include <regex>


void usage(const char *me)
{
    std::cerr << "Usage: " << me << "pattern string\n";
}


int main(int argc, const char *argv[])
{
    if(argc != 3)
    {
        usage(argv[0]);
        return 1;
    }
    const char *pattern = argv[1];
    const char *tested = argv[2];

    std::string full_pattern("^");
    full_pattern += pattern;
    full_pattern += "$";
    std::regex full_regex(full_pattern);
    bool expected_result = std::regex_match(tested, full_regex);

    Solution s;
    bool actual_result = s.isMatch(tested, pattern);

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
