// test for static_string
// require C++17
#include "StaticString.hpp"
constexpr static_string a{"1"};
constexpr static_string b{""};
constexpr auto c = a + b;
template <std::make_signed_t<std::size_t> n, typename = void>
constexpr std::size_t width = 1 + width<n / 10>;
template <std::make_signed_t<std::size_t> n>
constexpr std::size_t width<n, std::enable_if_t<(n > 0 && n < 10)>> = 1;
template <> constexpr auto width<0> = 1;
template <std::make_signed_t<std::size_t> n>
constexpr std::size_t width<n, std::enable_if_t<(n < 0)>> = 1 + width<-n>;

template <std::make_signed_t<std::size_t> n, std::size_t i>
constexpr std::make_signed_t<std::size_t> cpow = n * cpow<n, i - 1>;
template <std::make_signed_t<std::size_t> n>
constexpr std::make_signed_t<std::size_t> cpow<n, 0> = 1;

template <std::make_signed_t<std::size_t> n, std::size_t i,
          typename = std::enable_if_t<(i < width<n>)>>
constexpr static_string<1> digit = {
    static_cast<char>('0' + static_cast<char>(n / cpow<10, i> % 10))};

template <std::make_signed_t<std::size_t> n, std::size_t... I>
constexpr static_string<width<n>> str_imp(std::index_sequence<I...>) {
  return static_string{""} + (... + digit<n, width<n> - 1 - I>);
}
template <std::make_signed_t<std::size_t> n, typename = void>
constexpr static_string<width<n>> str =
    str_imp<n>(std::make_index_sequence<width<n>>());
template <std::make_signed_t<std::size_t> n>
constexpr static_string<width<n>> str<n, std::enable_if_t<(n < 0)>> =
    static_string{"-"} + str<-n>;
template <std::size_t l, std::size_t i>
constexpr auto make_expr =
    str<i> + static_string{"*"} + str<l> + static_string{"="} + str<i * l> +
    static_string{"\t"};
template <std::size_t l, std::size_t... I>
constexpr auto make_line_imp(std::index_sequence<I...>) {
  return (make_expr<l, 1 + I> + ...);
}
template <std::size_t l>
constexpr auto make_line =
    make_line_imp<l>(std::make_index_sequence<l>()) + static_string{"\n"};
template <std::size_t... I>
constexpr auto make_table_imp(std::index_sequence<I...>) {
  return (make_line<1 + I> + ...);
}
template <std::size_t l>
constexpr auto make_table = make_table_imp(std::make_index_sequence<l>());
#include <iostream>
int main() {
  //   std::cout << a.data << b.data << c.data;
  std::cout << make_table<50>.data;
}
