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
#include <set>

#include <iterator>
#include <ranges>

#include<optional>

namespace ranges    = std::ranges ;
namespace stdr      = std::ranges ;
namespace rng       = std::ranges ;

namespace views     = std::views ;
namespace stdv      = std::views ;
namespace vws       = std::views;

namespace ex        = std::execution ;

using keyword       = std::set<std::string> ;

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


keyword operator+(const keyword& lhs, const keyword& rhs)
{
    keyword unio = lhs;

    for (const auto elem : rhs)
    {
        unio.insert(elem);
    }

    return std::move(unio);

} ;

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

/*

template < std::ranges::view V, typename Pred>
    requires    std::ranges::bidirectional_range<V> &&
                std::indirect_unary_predicate<Pred, std::ranges::iterator_t<V>>
class drop_last_while_view
    : public std::ranges::view_interface < drop_last_while_view<V, Pred>>
{
    V       base_ ;
    Pred    pred_ ;
    std::optional<std::ranges::iterator_t<V>> cached_end_ ;

public:
    drop_last_while_view() = default;

    drop_last_while_view(V base, Pred pred)
        : base_(std::move(base)),
        pred_(std::move(pred))
    {}

    auto begin() { return std::ranges::begin(base_); }

    auto end() {
        if (!cached_end_) {
            auto view   = std::views::reverse(base_) ;
            cached_end_ = std::ranges::find_if_not(view, pred_).base() ;
        }
        return *cached_end_;
    }
};


namespace my_views {
    struct drop_last_while_fn
    {
        template <std::ranges::viewable_range R, typename Pred>
        constexpr auto operator()(R&& r, Pred pred) const
            -> drop_last_while_view<std::views::all_t<R>, Pred>
        {
            return drop_last_while_view<std::views::all_t<R>, Pred>
                (std::views::all(std::forward<R>(r)), std::move(pred));
        }
    };

    inline constexpr auto drop_last_while = drop_last_while_fn{};
};



inline constexpr auto trim_front = views::drop_while(::isalnum);

inline constexpr auto trim_back = drop_last_while(::isalnum);

inline constexpr auto trim = trim_front | trim_back;
*/

template <typename R>
auto trim_front(R&& rng)
{
    return views::drop_while(std::forward<R>(rng), ::isalnum);
//  return std::forward<R>(rng) | std::views::drop_while(!isalnum); // ez is jó
}

template <typename R>
auto trim_back(R&& rng) 
{
    return forward<R>(rng)
        | std::views::reverse
        | std::views::drop_while(::isalnum)
        | std::views::reverse;
}

template <typename R>
auto trim(R&& rng)
{
    return trim_back(trim_front(std::forward<R>(rng)));
}

std::string trim_str(const std::string& str)
{
    return str | trim | std::ranges::to<std::string>;
}

int main()
{
    char ex_elem{ 'a' } ;
    keyword KeyWords = BasicKeyWords + CPP11KeyWords + CPP20KeyWords + OtherKeyWords;
    std::vector<std::string> Word_flow;

    for ( const auto elem : KeyWords ) 
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
   /*   
   for (const auto& s : std::ranges::istream_view<std::string>(words)) {
        std::cout << std::quoted(s, '/') << ' ';
    }
    std::cout << '\n';
    */
    
    auto loremipsum = std::ranges::istream_view<std::string>(words) ;
    auto ipsumlorem = std::ostream_iterator<std::string>(std::cout, " ") ;

  /*    auto takeWhileAlnum = [](std::string text) mutable
    { return text
        //      | std::views::common 
        | std::views::take_while([](char c) { return std::isalnum(c); })
        | std::ranges::to<std::string> ;

    };
  */

    



    auto hogwash = loremipsum
        | std::views::transform(alnum)
        | std::views::take(10);
        
/*
    for (auto i : hogwash) {
        std::cout << i << "\n";
    };
*/
    std::ranges::copy(hogwash, ipsumlorem);

    /*
    for (const auto& s : std::ranges::istream_view<std::string>(words)) {
        std::cout << std::quoted(s, '/') << ' ';
    }
    */
 

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
