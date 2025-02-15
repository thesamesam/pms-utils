#pragma once

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/nonterminal/rule.hpp>
#include <format>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>

template <typename T> struct parse_result {
    T result;
    bool success{};
    std::string consumed;
    std::string remainder;
    bool as_expected{};
};

template <typename Rule>
[[nodiscard]] static inline auto try_parse(std::string_view input, Rule rule, bool expected = true,
                                           bool complete = true) {
    parse_result<typename Rule::attribute_type> ret;
    const auto *begin = input.begin();
    const auto *const end = input.end();
    ret.success = parse(begin, end, rule, ret.result);
    ret.consumed = {input.begin(), begin};
    ret.remainder = {begin, end};

    if (expected && !ret.success) {
        std::cerr << std::format("parser failed\n\tinput: {}\n\tconsumed: {}\n\tremainder: {}\n", input,
                                 ret.consumed, ret.remainder);
        ret.as_expected = false;
        return ret;
    }
    if (!expected && ret.success) {
        std::cerr << std::format(
            "parser succeeded unexpectedly\n\tinput: {}\n\tconsumed: {}\n\tremainder: {}\n", input,
            ret.consumed, ret.remainder);
        ret.as_expected = false;
        return ret;
    }

    // until I specialize std::formatter for all...
    std::stringstream stream;
    stream << ret.result;

    if (expected && complete && begin != end) {
        std::cerr << std::format(
            "parser did not consume all input\n\tinput: {}\n\tconsumed: {}\n\tremainder: {}\n\tparsed: {}\n",
            input, ret.consumed, ret.remainder, stream.view());
        ret.as_expected = false;
        return ret;
    }

    if (!complete && begin == end) {
        std::cerr << std::format("parser unexpectedly consumed all input\n\tinput: {}\n\tconsumed: "
                                 "{}\n\tremainder: {}\n\tparsed: {}\n",
                                 input, ret.consumed, ret.remainder, stream.view());
        ret.as_expected = false;
        return ret;
    }

    ret.as_expected = true;
    return ret;
}