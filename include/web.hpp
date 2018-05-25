#pragma once
#include <any>
#include <tuple>
#include <iostream>
#include <vector>
#include <optional>
#include <cassert>
#include "meta.hpp"


namespace Web {

	class Class {
		std::string name;
	public:
		explicit Class(std::string name) :
			name(std::move(name))
		{}
		const std::string& getName() const {
			return name;
		}
	};
	std::ostream& operator<<(std::ostream& stream, const Class& c) {
		return stream << "class=\"" << c.getName() << "\"";
	}

	class Id {
		std::string name;
	public:
		explicit Id(std::string name) :
			name(std::move(name))
		{}
		const std::string& getName() const {
			return name;
		}
	};
	std::ostream& operator<<(std::ostream& stream, const Id& id) {
		return stream << "id=\"" << id.getName() << "\"";
	}

	template <typename... Args>
	class Attr {

		std::tuple<Args...> attributes;

	public:
		explicit Attr(Args&&... args) : 
			attributes(std::make_tuple(std::forward<Args>(args)...))
		{
			static_assert(!contains_duplicates<Args...>::value, "Must not contain duplicates");
		}

		void print(std::ostream& stream) const {
			std::apply([&stream](auto&&... args)
			{
				((stream << " " << args), ...);
			}, attributes);
		}
	};
	template <typename... Args>
	std::ostream& operator<<(std::ostream& stream, const Attr<Args...>& d) {
		d.print(stream);
		return stream;
	}


	template <typename T>
	void WriteToStream(std::ostream& stream, const T& data)
	{
		stream << data;
	}
	template <typename T>
	void WriteToStream(std::ostream& stream, const std::vector<T>& data) {
		for(const auto& d : data) {
			stream << d;
		}
	}

	template <typename T>
	class HtmlBase {
		using AnyStreamFunctionPtr = std::ostream&(*)(std::ostream&, const std::any&);

		AnyStreamFunctionPtr streamFunc;
		std::any data;

		std::optional<std::any> attributes;
		AnyStreamFunctionPtr attrStreamFuncPtr;

	public:

		template <typename... Attributes, typename... Args>
		HtmlBase(Attr<Attributes...>&& attr, Args&&... args) noexcept :
			streamFunc(dataStreamFunc<Args...>()),
			data(std::make_tuple(std::forward<Args>(args)...)),
			attributes(std::make_tuple(std::forward<Attr<Attributes...>>(attr))),
			attrStreamFuncPtr(dataStreamFunc<Attr<Attributes...>>())
		{
		}

		template <typename... Args>
		HtmlBase(Args&&... args) noexcept :
			streamFunc(dataStreamFunc<Args...>()),
			data(std::make_tuple(std::forward<Args>(args)...))
		{}

		std::ostream& streamOut(std::ostream& str) const {
			str << "<" << T::tag;
			if (attributes)
			{
				attrStreamFuncPtr(str, *attributes);
			}
			str << ">";
			streamFunc(str, data);
			str << "</" << T::tag << ">";
			return str;
		}

		template <typename... Args>
		AnyStreamFunctionPtr dataStreamFunc() const {
			using AnyType = std::tuple<std::decay_t<Args>...>;

			return [](std::ostream& stream, const std::any& data) -> std::ostream& {
				try {
					const AnyType& children = std::any_cast<const AnyType&>(data);
					std::apply([&stream](const auto&... item) { (WriteToStream(stream, item), ...); }, children);
				}
				catch (std::bad_any_cast& /*e*/) {
					assert(false); // The types have not worked if we are here
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
public:												\
	static constexpr const char* tag = TAG;	\
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
	public:
		static constexpr const char* tag = "html";
		
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
