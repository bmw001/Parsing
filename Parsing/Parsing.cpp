// Parsing.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#pragma once

#include <iostream>
#include <fstream>
#include <iomanip>

#include <string>
#include <string_view>

#include <algorithm>
#include <numeric>
#include <execution>

#include <vector>
#include <array>
#include <set>

#include <iterator>
#include <ranges>

#include <optional>

namespace ranges    = std::ranges ;
namespace stdr      = std::ranges ;
namespace rng       = std::ranges ;

namespace views     = std::views ;
namespace stdv      = std::views ;
namespace vws       = std::views;

namespace ex        = std::execution ;

using keyword       = std::vector<std::string> ;

const keyword BasicKeyWords 
{
    "and","and_eq","asm","auto",
    "bitand","bitor","bool","break",
    "case","catch","char","class","compl","const","const_cast","continue",
    "default","delete","do","double","dynamic_cast",
    "else","enum","explicit","export","extern",
    "false","float","for","friend","goto",
    "if","inline","int",
    "long",
    "mutable",
    "namespace","new","not","not_eq",
    "operator","or","or_eq",
    "private","protected","public",
    "register","reinterpret_cast","return",
    "short","signed","sizeof","static","static_cast","struct","switch",
    "template","this","throw","true","try","typedef","typeid","typename",
    "union","unsigned","using",
    "virtual","void","volatile",
    "wchar_t","while",
    "xor",
    "xor_eq"
} ;

const keyword CPP11KeyWords 
{
    "alignas","alignof",
    "char16_t","char32_t","constexpr",
    "decltype",
    "noexcept","nullptr",
    "static_assert",
    "thread_local"
} ;

const keyword CPP20KeyWords
{
    "concept","consteval","constinit","co_await","co_return","co_yield","char8_t",
    "requires"
} ;

const keyword OtherKeyWords
{
    "atomic_cancel","atomic_commit","atomic_noexcept",
    "reflexpr"
} ;

// const keyword KeyWords = ( BasicKeyWords )/*  + OtherKeyWords */;
// std::ranges::sort(KeyWords) ;

/*
keyword operator+(const keyword& lhs, const keyword& rhs)
{
    keyword unio = lhs;

    for (const auto& elem : rhs)
    {
        unio.insert(elem);
    }

    return std::move(unio);

} ;
*/

std::size_t word_count( std::string_view s )
{
    if ( s.empty() ) return 0 ; 
    
    return std::transform_reduce
    (
        std::execution::par_unseq,
        begin(s), 
        end(s) - 1, 
        begin(s) + 1,
        std::size_t(!std::isspace(s.front()) ? 1 : 0),
        std::plus(),
        [](char l, char r) { return std::isspace(l) && !std::isspace(r); }
    );
}

const std::string input_file_name{"ipsumlorem.txt"};
//std::string text{""}

void file_error(const char* p, const char* p2 = "")
{
    std::cerr << p << ' ' << p2 << '\n';
    std::exit(1);
}

std::string alnum(std::string s) 
{ 
    auto i{ 0 } ;
    auto it = s.begin();
    for (; it!=s.end() && std::isalnum(char(*it)); it++) { i++; };
    return s.substr(0, i);
};


int main()
{
    keyword KeyWords = BasicKeyWords /* + CPP11KeyWords + CPP20KeyWords + OtherKeyWords */;
    std::ranges::sort(KeyWords);
    std::vector<std::string> Word_flow;
    char ex_elem{};

    for ( const auto& elem : KeyWords ) 
    {
        if ( elem[0] != ex_elem ) std::cout << "\n" ;

        std::cout << elem << "  " ;
        ex_elem = elem[0] ;
    }

    std::ifstream input_file( input_file_name ) ;
    if ( !input_file )
    {
        std::cerr << "A bemeneti fájl nem nyitható meg" << input_file_name ;
        std::exit(1) ;
    }

    auto words = std::ifstream(input_file_name) ;
    
    auto loremipsum = std::ranges::istream_view<std::string>(words) ;
    auto ipsumlorem = std::ostream_iterator<std::string>(std::cout, " ") ;

    auto hogwash = loremipsum
        | std::views::transform( [](std::string s) {
                    return std::views::all(s)
                        | std::views::take_while(::isalnum)
                        | std::ranges::to<std::string>();
            })
        | std::views::take(10);

        
    std::ranges::copy(hogwash, ipsumlorem);

    /*
    for (const auto& s : std::ranges::istream_view<std::string>(words)) {
        std::cout << std::quoted(s, '/') << ' ';
    }
    */
 
    std::array<unsigned int, 256> FrequencyByChar{};
    
    auto lorem = std::ifstream("lorem.txt");
    lorem.unsetf(std::ios_base::skipws);
    auto ipsum = std::ranges::istream_view<char>(lorem);

//  std::transform(std::istreambuf_iterator<char>(lorem),
//  std::istreambuf_iterator<char>(),
//  std::ostreambuf_iterator<char>(std::cout),
//  [](int ch) { return isupper(ch) ? ((ch - 'A') + 3) % 26 + 'A' : ch; });
    
    for (const auto c : ipsum) {
        std::cout << c;
        ++FrequencyByChar[static_cast<unsigned>(c)];
    }

    auto sum = std::accumulate(FrequencyByChar.begin(), FrequencyByChar.end(), 0);

    for (int i = 0; i < 256; ++i)  {
        if (FrequencyByChar[i] != 0) {
            std::cout << "\n" << i << "   " << char(i) << "   " << FrequencyByChar[i] ;
        }
    }

    return 0;

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
