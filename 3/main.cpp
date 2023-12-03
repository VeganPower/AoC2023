#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>

using std::ios;
using std::cout;
using std::ifstream;
using std::string;
using std::vector;
using std::array;
using std::max;
using std::min;

constexpr int k_line_size = 140;
using line_t = array<int8_t, k_line_size>;
constexpr int k_empty = -1;
constexpr int k_symbol = -2;

enum class ParseError
{
    k_ok,
    k_unexpected_end_of_stream,
    k_unexpected_token,
    k_invalid_format
};

char const* to_string(ParseError ec)
{
    switch(ec)
    {
    case ParseError::k_ok:                       return "Ok";
    case ParseError::k_unexpected_end_of_stream: return "Unexpected end of the stream";
    case ParseError::k_unexpected_token:         return "Unexpected token";
    case ParseError::k_invalid_format:           return "Invalid format";
    }
    return "unknown error";
}

void to_array(line_t& result, string const& str)
{
    if (str.length() != k_line_size)
        throw ParseError::k_invalid_format;
    for(int i = 0; i < k_line_size; ++i)
    {
        char c = str[i];
        if (c == '.')
            result[i] = k_empty;
        else if (std::isdigit(c))
            result[i] = c - '0';
        else
            result[i] = k_symbol;
    }
}

int scan_for_symbols(int number_begin, int number_end, line_t const& current_line, line_t const& prev_line, line_t const& next_line)
{
    const int scan_begin = max(number_begin - 1, 0);
    const int scan_end   = min(number_end + 1, k_line_size - 1);
    bool valid = (current_line[scan_begin] == k_symbol) || (current_line[scan_end] == k_symbol);
    if (!valid)
    {
        for (int k = scan_begin; k <= scan_end; ++k)
        {
            if ((prev_line[k] == k_symbol) || (next_line[k] == k_symbol))
            {
                valid = true;
            }
        }
    }
    if (valid)
    {
        int engine_part = 0;
        for (int k = number_begin; k <= number_end; ++k)
        {
            engine_part *= 10;
            engine_part += current_line[k];
        }
        return engine_part;
    }

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
    vector<line_t> engine;
    try
    {
        string line;
        line_t schematic_line;
        while(std::getline(file, line))
        {
            to_array(schematic_line, line);
            engine.push_back(schematic_line);
        }
    }
    catch (ParseError const& error)
    {
        cout << "parsing error: " << to_string(error) << "\n";
    }
    int accum = 0;
    for(int line_idx = 0; line_idx < engine.size(); ++line_idx)
    {
        line_t const& current_line = engine[line_idx];
        line_t const& prev_line    = (line_idx != 0) ? engine[line_idx - 1] : current_line;
        line_t const& next_line    = (line_idx != (engine.size() - 1)) ? engine[line_idx + 1] : current_line;
        int number_begin = k_line_size;
        int number_end = 0;
        for (int j = 0; j < k_line_size; ++j)
        {
           const int8_t cursor = current_line[j];
            if (cursor>= 0)
            {
                number_begin = min(number_begin, j);
                number_end   = j;
            } else if (number_begin != k_line_size)
            {
                accum += scan_for_symbols(number_begin, number_end, current_line, prev_line, next_line);
                number_begin = k_line_size;
                number_end = 0;
            }
        }
        if (number_begin != k_line_size)
           accum += scan_for_symbols(number_begin, number_end, current_line, prev_line, next_line);
    }
    cout << accum << "\n";
    return 0;
}
