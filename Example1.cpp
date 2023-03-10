#include <iostream>
#include <limits>
#include <algorithm>
#include <tuple>
#include <type_traits>

template <int First, int Last>
struct static_for {
	template <typename Lambda>
	static inline constexpr void apply(Lambda const& f) {
		if (First < Last) {
			f(std::integral_constant<int, First>{});
			static_for<First + 1, Last>::apply(f);
		}
	}
};
template <int N>
struct static_for<N, N> {
	template <typename Lambda>
	static inline constexpr void apply(Lambda const& f) {}
};

template <int N, typename... Ts>
struct gett;

template <int N, typename T, typename... Ts>
struct gett<N, std::tuple<T, Ts...>> {
	using type = typename gett<N - 1, std::tuple<Ts...>>::type;
};

template <typename T, typename... Ts>
struct gett<0, std::tuple<T, Ts...>> {
	using type = T;
};

template <typename... Args>
struct make_tuple {
	using type = std::tuple<std::remove_pointer_t<Args>...>;
	type b;
	make_tuple(type t) : b(t) {}
	float sumData() {
		float sum = 0;

		static_for<0, sizeof...(Args)>::apply([&](auto i) {
			auto val = std::get<i.value>(b);
			if constexpr (std::is_floating_point_v<gett<i.value, type>::type>) {
				sum += val;
			}
			if constexpr (std::is_integral_v<gett<i.value, type>::type>) {
				sum += (float)val;
			}
			if constexpr (std::is_same_v<gett<i.value, type>::type, std::string>) {
				sum += (float)std::atof(val.c_str());
			}
			});
		return sum;
	}

};

int main() {
	make_tuple<int, std::string, float> tuple({1,"5,6",3.5});
	std::cout << tuple.sumData();
}
