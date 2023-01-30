#pragma once

///////////////////////////////////////////////////
//		PROBLEM 1 : MINIMUM ARRAY ELEMENT        //
// ----------------------------------------------//
// --- Given a non-empty array of integers,      // 
// --- find the minimum value it contains        //
///////////////////////////////////////////////////
//
//  A.
//
std::vector<int> vec = get_input();
auto iter = std::min_element(vec.begin(), vec.end());
//
//  B.
//
namespace rng = std::ranges;
std::vector<int> vec = get_input();
auto iter = rng::min_element(vec.begin(), vec.end());
//
//  C.
//
std::vector<int> vec = get_input();
auto iter = rng::min_element(vec);
//
//  D.	when you pass an rvalue range into algorithm
//		which normally raturns an iterator, ranges will return
//			std::ranges::dangling object
auto iter = rng::min_element(get_input()); // !!! dangling iterator !!!
// This will compile, but iter will have type
//		std::ranges::dangling
//
// Attempting to use a dangling object
// will cause a compile error :
auto iter = rng::min_element(get_input());
std::cout << *iter; // COMPILE ERROR !!!
// "error: no match for 'operator*'
// (operand type is 'std::ranges::dangling' "
//
//	------------	VIEWS   -----------------
// 
// informally, views are sometimes referred to as
// " ranges which do not own their elements".
//
// today, a view is a range such that
// 1. its move operations are constant-time
// 2. (if it is copyable) its operations are constant-time
// 3. destruction of a moved-from object is constant-time
// 4. otherwise, destruction is O(N)
//
// Just as borrowed ranges, a type can opt in to
// being a view using the enable_view trait, or by
// inheriting from ranges::view_base.
//
// Today, we can turn any movable range into a view
//		using the std::views::all() function :

std::vector<int> vec = get_input();
auto view1 = std::views::all(vec); // Always okay, view1 is movable and copyable
auto view2 = std::views::all(get_input()); // Okay since P2415, view is move only

// View adaptors implicitly convert their arguments to 
//		views using wies::all() :

auto vec = get_vector();
auto v1 = vec | views::transform(func); // Always okay: vec is an lvalue => borrowed
auto v2 = get_vector() | views::transform(func); // Okay since P2415, v2 is move only
//
///////////////////////////////////////////////////
//		PROBLEM 2 : SUM OF SQUARES               //
// ----------------------------------------------//
// --- Given aN array of integers, calculate the // 
// --- sum of squares of the values it contains  //
///////////////////////////////////////////////////
//
// A.
//
auto vec = get_input();
std::transform(vec.begin(), vec.end(), vec.begin(),
					[](int i) { return i * i; });
auto sumsq = std::accumulate(vec.begin(), vec.end(), 0);
//
// B.
//
auto vec = get_input();
ranges::transform(vec, vec.begin()),
					[](int i) { return i * i; });
auto sumsq = std::accumulate(vec.begin(), vec.end(), 0);
//
// C.
//
auto vec = get_input();
auto view = std::views::transform(vec,
	[](int i)) {return i * i; });
auto sumsq = std::accumulate(view.begin(), view.end(), 0);
//
// D.
//
auto vec = get_input();
auto view = vec
	| std::views::transform([](int i) { return i * i; })
	| std::views::common;
auto sumsq = std::accumulate(view.begin(), view.end(), 0);
//
//  E. range-ified custon accumulate - see below
// 
auto vec = get_input();
auto view = vec
	| views::transform([](int i) { return i * i; });
auto sumsq = accumulate(view);
//
//  F. alternative
// 
auto vec = get_input();
auto sumsq = accumulate(
	vec | views::transform([](int i) { return i * i; }));
	);
//
//  G. with projection -> transform(square) lambda
// 
auto vec = get_input();
auto sumsq = accumulate(vec, {}, {},
				[](int i) { return i * i; }));
			);
//
//  H. finally...
// 
auto sumsq = accumulate(get_input(), {}, {},
				[](int i) { return i * i; });
//
//-------------------------------------------------------
// Unfortunately, C++20 does not have "range-ified"
//	versions of the algorithm in the <numeric> header.
//
// All being well, C++23 vill provide a family of range-
//	based fold() functions which we can use instead
// 
// In the men time, let's write our own
//	range-based accumulate()
//--------------------------------------------------------
// 
// A.
//
template <typename I, typename Init>
Init accumulate(I first, I last, Init init)
{
	while (first != last) {
		init = std::move(init) + *first;
		++first;
	}
	return init;
}
//
// B. - sentinel
//
template <typename I, typename S, typename Init>
Init accumulate(I first, S last, Init init)
{
	while (first != last) {
		init = std::move(init) + *first;
		++first;
	}
	return init;
}
//
// C. - default op : '+'
//
template <typename I, typename S, typename Init,
			typename Op = std::plus<> >
Init accumulate(I first, S last, Init init, Op op = Op{}) // <= the default op. -> '+'
{
	while (first != last) {
		init = op(std::move(init), *first);
		++first;
	}
	return init;
}
//
// D. - requires
template <typename I, typename S, Typename Init, 
			typename Op = std::plus<> >
	requires
		std::input_iteraotr<I> &&
		std::sentinel_for<S, I>
Init accumulate(I first, S last), Init init, Op op = Op{})
{
	while (first != last) {
		Init = op(std::move(init), *first);
		++first;
	}
	return init;	
}
//
// E - concepts
//
template <std::input_Iterator I, std::sentinel_for<I> S,
		typename Init, typename Op = std::plus<>>
Init accumulate(I first, S last), Init init, Op op = Op{})
{
	while (first != last) {
		Init = op(std::move(init), *first);
		++first;
	}
	return init;
}
//
// F. - init default value
//
template <std::input_iterator I, std::sentinel_for<I> S,
	typename Init = std::iter_value_t<I>, // <= the type of the iterated value -> int
	typename Op = std::plus<>>
Init accumulate(I first, S last, Init init = Init{},
		Op op = Op{})
{
	while (first != last) {
		init = std::invoke(op, std::move(init), *first);
		++first;
	}
	return init;
}
//
// G. - Range overload
// 
template <rng::input_range R,
	typename Init = rng::range_value_t<R>, // <= the type of the iterated value -> int
	typename Op = std::plus<>>
Init accumulate(R&& rng, Init init = Init{}, Op op = Op{})
{
	return accumulate(rng::begin(rng), rng::end(rng),
		std::move(init), std::move(op));
}
//------------------Projections-----------------------
//	
// Almost all of the standard library rangified algorithms
// take function objects. They take a new extra optional
// parameter called: projection. 
// 
// Projections are function adaptors. Projections do not
// change the algorithm. Projections are a simply convenient 
// way to adapt the algorithm’s predicate (or function).
// Note that projections are always unary.
// 
// What projections do that they modify the view of the data
// just before the function object sees it, 
// 
// So, if you pass a custom comparator - e.g. std::sort -
// or you pass a custom binary operation to accumulate, 
// for example 'transform', we can basically split this into 
// two and we can use a projection to modify the value 
// just before we send it to the function object.
// 
//--------------------------------------------------------
//
// G. - Projection - non-range
// 
template<std::input_iterator I, 
		std::sentinel_for<I> S,
		typename Init = std::iter_value_t<I>,
		typename Op = std::plus<>,
		typename Proj = std::identity> // default op.: identity -> to do nothing
Init accumulate(I first, S last, Init init = Init{},
	Op op = Op{}, Proj proj = Proj{})
{
	while (first != last) {
		init = std::invoke(op, std::move(init),
			std::invoke(proj, *first));
		++first;
	}
	return init;
}
//
// H. - Projection - range
// 
template <rng::input_range R,
		typename Init = rng::range_value_t<R>,
		typename Op = std::plus<>,
		typename Proj = std::identity>
Init accumulate(R&& rng, Init init = Init{},
	Op op = Op{}, Proj proj = Proj{})
{
	return accumulate(rng::begin(rng), rng::end(rng),
		std::move(init), std::move(op),
		std::move(proj));
}
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
/*
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
*/
