// require C++17 for CTAD
#pragma once
#include <cstddef>
#include <type_traits>
#include <utility>

template <std::size_t N> struct static_string {
  char data[N + 1];
  constexpr static std::size_t length = N;
  constexpr auto &operator[](std::size_t i) { return data[i]; }
  constexpr auto &operator[](std::size_t i) const { return data[i]; }
  constexpr static_string() : data{0} {}
  template <std::size_t... I>
  constexpr static_string(const char (&str)[length + 1],
                          std::index_sequence<I...>)
      : data{str[I]..., '\0'} {}
  constexpr static_string(const char (&str)[length + 1])
      : static_string{str, std::make_index_sequence<length>()} {}
  template <typename... Ts>
  constexpr static_string(const Ts &...str) : data{str...} {}
  constexpr auto *begin() { return data; }
  constexpr auto *begin() const { return data; }
  constexpr auto *end() { return data + length; }
  constexpr auto *end() const { return data + length; }
  template <std::size_t begin, std::size_t count, std::size_t... I>
  constexpr static_string<count> substr(std::index_sequence<I...>) const {
    return static_string<count>{data[begin + I]..., '\0'};
  }
  template <std::size_t begin, std::size_t count,
            typename = std::enable_if_t<(begin + count <= length)>>
  constexpr static_string<count> substr() const {
    return substr<begin, count>(std::make_index_sequence<count>());
  }
};
template <std::size_t N>
static_string(const char (&)[N]) -> static_string<N - 1>;
template <std::size_t N1, std::size_t N2, std::size_t... I1, std::size_t... I2>
constexpr static_string<N1 + N2>
concat(const static_string<N1> &a, const static_string<N2> &b,
       std::index_sequence<I1...>, std::index_sequence<I2...>) {

  return static_string<N1 + N2>{a[I1]..., b[I2]..., '\0'};
}
template <std::size_t N1, std::size_t N2,
          typename = std::enable_if_t<(N1 > 0 && N2 > 0)>>
constexpr static_string<N1 + N2> operator+(const static_string<N1> &a,
                                           const static_string<N2> &b) {
  return concat<N1, N2>(a, b, std::make_index_sequence<N1>(),
                        std::make_index_sequence<N2>());
}
constexpr static_string<0> operator+(const static_string<0> &a,
                                     const static_string<0> &b) {
  return "";
}
template <std::size_t N, typename = std::enable_if_t<(N > 0)>>
constexpr static_string<N> operator+(const static_string<N> &a,
                                     const static_string<0> &b) {
  return a;
}
template <std::size_t N, typename = std::enable_if_t<(N > 0)>>
constexpr static_string<N> operator+(const static_string<0> &a,
                                     const static_string<N> &b) {
  return b;
}
