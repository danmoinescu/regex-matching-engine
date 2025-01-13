#include "regex-matching-engine.hpp"
#include <algorithm>
#ifdef DEBUG
#include <iterator>
#include <iomanip>
#include <iostream>


std::ostream& operator<<(std::ostream &out, const PatternItem &n)
{
    out << n.c;
    if(n.hasStar) { out << STAR; }
    return out;
}
#endif


void Solution::parsePattern(const char *p)
{
    parsedPattern.clear();
    parsedPattern.reserve(strlen(p));

    bool hasStar;
    char c;
    for(; *p; ++p)
    {
        c = *p;
        if(p[1] == STAR)
        {
            hasStar = true;
            ++p;
        }
        else
        {
            hasStar = false;
        }
        /* Merge consecutive identical star patterns
           For example: a*a* == a*
         */
        if(!parsedPattern.empty()
                && parsedPattern.back().hasStar
                && hasStar
                && parsedPattern.back().c == c)
        {}
        else
        {
            parsedPattern.emplace_back(c, hasStar);
        }
    }
#ifdef DEBUG
    std::cout << "Pattern items: ";
    std::copy(parsedPattern.cbegin(), parsedPattern.cend(),
            std::ostream_iterator<PatternItem>(std::cout, " "));
    std::cout << "\n";
#endif
}


bool Solution::isMatch(const char *s, PatternIterator pCrtPatternItem, int depth)
{
#ifdef DEBUG
    std::cout << std::right << std::setw(2) << depth << std::left << std::setw(0)
        << ": call isMatch("
        << std::right << std::setw(s_len) << s << std::left << std::setw(0)
        << ", ";
    std::copy(pCrtPatternItem, parsedPattern.cend(),
            std::ostream_iterator<PatternItem>(std::cout, " "));
    std::cout << ")\n";
#endif
    auto pair_id = mkid(s, pCrtPatternItem);
    auto cache_iterator = cache.find(pair_id);
    if(cache_iterator != cache.end())
    {
        return cache_iterator->second;
    }
    bool retval = isMatchInner(s, pCrtPatternItem, depth);
#ifdef DEBUG
    std::cout << std::right << std::setw(2) << depth << std::left << std::setw(0)
        << ":  ret isMatch("
        << std::right << std::setw(s_len) << s << std::left << std::setw(0)
        << ", ";
    std::copy(pCrtPatternItem, parsedPattern.cend(),
            std::ostream_iterator<PatternItem>(std::cout, " "));
    std::cout << "): " << retval << "\n";
#endif
    cache.emplace(pair_id, retval);
    return retval;
}

bool Solution::isMatchInner(const char *s, PatternIterator pCrtPatternItem, int depth)
{
    if(!s[0]) // the string is exhausted
    {
        /* We're happy if all remaining pattern items (if any)
           are starred (since they can match the empty string).
        */
        return std::all_of(
                pCrtPatternItem, parsedPattern.cend(),
                [](const PatternItem &item) { return item.hasStar; } );
    }
    if(pCrtPatternItem == parsedPattern.cend())
    {
        // the pattern is exhausted before the string
        return false;
    }
    if(! pCrtPatternItem->hasStar)
    {
        if(pCrtPatternItem->matches(s[0]))
        {
            //advance
            return isMatch(s+1, pCrtPatternItem+1, depth+1);
        }
        else // no match
        {
            return false;
        }
    }
    // Now we have a star pattern
    /* First, ignore it (assume it corresponds to the empty string)
       and try to match the subsequent items
    */
    if(isMatch(s, pCrtPatternItem+1, depth+1))
    {
        return true;
    }
    /* Next, assume it corresponds to ever-longer substrings
       (meaning we skip those substrings and keep matching the
       subsequent items).
    */
    for(const char *remainder = s; *remainder; remainder++)
    {
        if(!pCrtPatternItem->matches(remainder[0]))
        {
            break;
        }
        if(isMatch(remainder+1, pCrtPatternItem+1, depth+1))
        {
            return true;
        }
    }
    return false;
}


bool Solution::isMatch(const char *s, const char *p)
{
    parsePattern(p);
    orig_str = s;
    s_len = strlen(s);
    pFirstPatternItem = parsedPattern.cbegin();
    return isMatch(s, pFirstPatternItem, 1);
}
