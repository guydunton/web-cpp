#pragma once
#include <any>
#include <tuple>
#include <string_view>
#include <iosfwd>

namespace Web {

	template <typename T>
	class HtmlBase {
		using AnyStreamFunctionPtr = std::ostream&(*)(std::ostream&, const std::any&);

		AnyStreamFunctionPtr streamFunc;
		std::any data;

	public:
		template <typename... Args>
		HtmlBase(Args&&... args) :
			streamFunc(dataStreamFunc<Args...>()),
			data(std::tuple(std::forward<Args>(args)...))
		{}

		std::ostream& streamOut(std::ostream& str) const {
			str << "<" << T::tag << ">";
			streamFunc(str, data);
			str << "</" << T::tag << ">";
			return str;
		}

		template <typename... Args>
		static AnyStreamFunctionPtr dataStreamFunc() {
			using AnyType = std::tuple<std::decay_t<Args>...>;

			return [](std::ostream& stream, const std::any& data) -> std::ostream& {
				try {
					const auto children = std::any_cast<AnyType>(data);
					std::apply([&stream](const auto&... item) { ((stream << item), ...); }, children);
				}
				catch (std::bad_any_cast& /*e*/) {
					stream << "Failed to evaluate children";
				}
				return stream;
			};
		}
	};

	template <typename T>
	std::ostream& operator<<(std::ostream& str, const HtmlBase<T>& base) {
		return base.streamOut(str);
	}


	class P : public HtmlBase<P> {
		static constexpr std::string_view tag = "p";

	public:
		template <typename... Args>
		explicit P(Args&&... args) :
			HtmlBase<P>(std::forward<Args>(args)...)
		{}
	};

	class H1 : public HtmlBase<H1> {
		static constexpr std::string_view tag = "h1";

	public:
		template <typename... Args>
		explicit H1(Args&&... args) :
			HtmlBase<H1>(std::forward<Args>(args)...)
		{}
	};
	
	class Div : public HtmlBase<Div> {
		static constexpr std::string_view tag = "div";

	public:
		template <typename... Args>
		explicit Div(Args&&... args) :
			HtmlBase<Div>(std::forward<Args>(args)...)
		{}
	};

	class Body : public HtmlBase<Body> {
		static constexpr std::string_view tag = "body";

	public:
		template <typename... Args>
		explicit Body(Args&&... args) :
			HtmlBase<Body>(std::forward<Args>(args)...)
		{
		}
	};

	class Html : public HtmlBase<Html> {
		static constexpr std::string_view tag = "html";

	public:
		template <typename... Args>
		explicit Html(Args&&... args) :
			HtmlBase<Html>(std::forward<Args>(args)...)
		{
			static_assert((0 + ... + (std::is_same_v<std::decay_t<Args>, Body> ? 1 : 0)) == 1, 
				"Html must contain one body");
		}
	};

	class Br {};
	inline std::ostream& operator<<(std::ostream& stream, const Br& /*br*/) {
		return stream << "<br/>";
	}
}
