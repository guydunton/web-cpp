#pragma once
#include <any>
#include <tuple>
#include <string_view>
#include <iosfwd>

namespace Web {

	template <typename T>
	std::ostream& WriteToStream(std::ostream& stream, const T& data)
	{
		return stream << data;
	}

	template <typename T>
	class HtmlBase {
		using AnyStreamFunctionPtr = std::ostream&(*)(std::ostream&, const std::any&);

		AnyStreamFunctionPtr streamFunc;
		std::any data;

	public:
		template <typename... Args>
		HtmlBase(Args&&... args) noexcept :
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
		AnyStreamFunctionPtr dataStreamFunc() const {
			using AnyType = std::tuple<std::decay_t<Args>...>;

			return [](std::ostream& stream, const std::any& data) -> std::ostream& {
				try {
					const auto& children = std::any_cast<const AnyType&>(data);
					std::apply([&stream](const auto&... item) { (WriteToStream(stream, item), ...); }, children);
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

#define WEB_HTML_TAG_NO_VALIDATION(NAME, TAG) 		\
class NAME : public HtmlBase<NAME> {				\
	static constexpr std::string_view tag = TAG;	\
public:												\
	template <typename... Args>						\
	explicit NAME(Args&&... args) noexcept :		\
		HtmlBase<NAME>(std::forward<Args>(args)...)	\
	{}												\
};

	WEB_HTML_TAG_NO_VALIDATION(H2, "h2");
	WEB_HTML_TAG_NO_VALIDATION(P, "p");
	WEB_HTML_TAG_NO_VALIDATION(H1, "h1");
	WEB_HTML_TAG_NO_VALIDATION(Div, "div");
	WEB_HTML_TAG_NO_VALIDATION(Body, "body");

	class Html : public HtmlBase<Html> {
		static constexpr std::string_view tag = "html";

	public:
		template <typename... Args>
		explicit Html(Args&&... args) noexcept :
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
