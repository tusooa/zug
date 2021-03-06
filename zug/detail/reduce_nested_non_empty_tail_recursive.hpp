//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <iterator>

namespace zug {
namespace detail {

template <typename ReducingFnT, typename StateT, typename InputIterT>
auto reduce_nested_non_empty_tail_recursive_impl(ReducingFnT&& step,
                                                 StateT&& state,
                                                 InputIterT&& first,
                                                 InputIterT&& last)
    -> std::decay_t<StateT>
{
    if (state_is_reduced(state) || first == last)
        return std::forward<StateT>(state);

    auto next_state = step(std::forward<StateT>(state), *first);
    return reduce_nested_non_empty_tail_recursive_impl(
        std::forward<ReducingFnT>(step),
        std::move(next_state),
        std::forward<InputIterT>(++first),
        std::forward<InputIterT>(last));
}

template <typename ReducingFnT, typename StateT, typename InputRangeT>
auto reduce_nested_non_empty_tail_recursive(ReducingFnT&& step,
                                            StateT&& initial,
                                            InputRangeT&& range)
    -> std::decay_t<decltype(step(initial, *std::begin(range)))>
{
    auto first = std::begin(range);
    auto last  = std::end(range);
    auto state = step(std::forward<StateT>(initial), *first);
    return reduce_nested_non_empty_tail_recursive_impl(
        std::forward<ReducingFnT>(step), std::move(state), ++first, last);
}

} // namespace detail
} // namespace zug
