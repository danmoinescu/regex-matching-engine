#pragma once
#include <vector>
#include <string>
#include <cstring>

#define STAR '*'
#define DOT '.'

struct PatternItem
{
    const char c;
    const bool hasStar;

    PatternItem(char c, bool hasStar): c(c), hasStar(hasStar)
    {}

    bool matches(char s) const
    {
        return (c == s) || (c == DOT);
    }
};

typedef std::vector<PatternItem>::const_iterator PatternIterator;


class Solution
{
    private:
        std::vector<PatternItem> parsedPattern;

    public:
        bool isMatch(const char *s, const char *pattern);
        bool isMatch(const std::string &s, const std::string &pattern)
        {
            return isMatch(s.c_str(), pattern.c_str());
        }

    private:
        void parsePattern(const char *p);
        bool isMatch(const char *s, PatternIterator pCrtPatternItem);
};
