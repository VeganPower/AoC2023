#include <iostream>
#include <fstream>
#include <string>

using std::ios;
using std::cout;
using std::ifstream;
using std::string;

int scan_line(string const& str)
{
    char first = '\0';
    char last = '\0';
    for(auto c : str)
    {
        if (!std::isdigit(c))
            continue;
        last = c;
        if (first == '\0')
        {
            first = c;
            continue;
        }
    }
    if (first == '\0')
        return 0;
    int a = int(first - '0');
    int b = int(last - '0');
    return a * 10 + b;
}

int main()
{
    ifstream file{"input.txt"};
    if (!file.is_open())
    {
        cout << "File not found\n";
        return -1;
    }
    string tp;
    int accum = 0;
    while(std::getline(file, tp))
    {
        accum += scan_line(tp);
    }
    cout << accum << "\n";
}
