#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <ranges>

using std::ios;
using std::cout;
using std::ifstream;
using std::string;
using std::string_view;
// using namespace std::views;

struct DiceExtraction
{
    int red_count;
    int green_count;
    int blue_count;
};

enum class ParserError
{
    k_ok,
    k_unexpected_end_of_stream,
    k_unexpected_token
};

char const* to_string(ParserError ec)
{
    switch(ec)
    {
    case ParserError::k_ok:                       return "ok";
    case ParserError::k_unexpected_end_of_stream: return "unexpected end of the stream";
    case ParserError::k_unexpected_token:         return "unexpected token";
    }
    return "unknown error";
}


struct StrView
{
    std::string_view::const_iterator cursor;
    std::string_view::const_iterator sentinel;
    bool done() const { return cursor == sentinel; }
};

StrView from_str(string_view const& str)
{
    return { str.begin(), str.end() };
}

void consume(StrView& iter, string_view const& cmp)
{
    for(auto c : cmp)
    {
        if (iter.cursor == iter.sentinel)
            throw ParserError::k_unexpected_end_of_stream;
        if (*iter.cursor == c)
            iter.cursor++;
        else
            throw ParserError::k_unexpected_token;
    }
}

bool is_(StrView& iter, string_view const& cmp)
{
    std::string_view::const_iterator begin = iter.cursor;
    for(auto c : cmp)
    {
        if (iter.done())
        {
            iter.cursor = begin;
            return false;
        }
        if (*iter.cursor == c)
            iter.cursor++;
        else
        {
            iter.cursor = begin;
            return false;
        }
    }
    return true;
}

void remove_trail_spaces(StrView& iter)
{
    while(!iter.done())
    {
        const int c = *iter.cursor;
        if (!std::isspace(c))
            break;
        ++iter.cursor;
    }
}

int32_t get_int32(StrView& iter)
{
    if (iter.done())
        throw ParserError::k_unexpected_end_of_stream;

    bool has_number = false;
    int32_t value = 0;
    while(!iter.done())
    {
        const int c = *iter.cursor;
        if (std::isspace(c))
        {
            ++iter.cursor;
            continue;
        }
        if (std::isdigit(c))
        {
            has_number = true;
            value *= 10;
            value += c - '0';
            ++iter.cursor;
        }
        else
        {
            if (has_number)
                break;
            throw ParserError::k_unexpected_token;
        }
    }
    return value;
}

bool is_valid_game(StrView& iter)
{
    while(!iter.done())
    {
        int32_t dice_v = get_int32(iter);
        int32_t limit = 0;
        if (is_(iter, "red"))
            limit = 12;
        else if (is_(iter, "green"))
            limit = 13;
        else if (is_(iter, "blue"))
            limit = 14;
        if (dice_v > limit) return false;
        is_(iter, ",");
        is_(iter, ";");
        remove_trail_spaces(iter);
    }
    return true;
}

int scan_line(string_view const& str_v)
{
    auto v = from_str(str_v);
    consume(v, "Game ");
    int32_t game_id = get_int32(v);
    consume(v, ": ");
    if (is_valid_game(v)) return game_id;
    return 0;
}

int main()
{
    ifstream file{"input.txt"};
    if (!file.is_open())
    {
        cout << "File not found\n";
        return -1;
    }
    int accum = 0;
    try
    {
        string tp;
        while(std::getline(file, tp))
        {
            accum += scan_line(tp);
        }
    }
    catch (ParserError const& error)
    {
        cout << "parsing error: " << to_string(error) << "\n";
    }
    cout << accum << "\n";
}
