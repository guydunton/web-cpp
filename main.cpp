#include <iostream>
#include <string>
#include <string_view>
#include <sstream>

namespace Web {

	template <typename T>
	class Base;

	template <template <typename...>typename T, typename... Args>
	class Base<T<Args...>> {
	public:
		std::string to_string() const {
			auto r = static_cast<const T<Args...>&>(*this);
			std::stringstream ss;
			
			ss << "<" << T<Args...>::tag << ">";
			std::apply([&ss](const auto &... item) { ((ss << item), ...); }, r.children);
			ss << "</" << T<Args...>::tag << ">";

			return ss.str();
		}
	};

	template <typename T>
	std::ostream& operator<<(std::ostream& str, const Base<T>& b) {
		return str << b.to_string();
	}


	//---------------------------------------------------
	// br
	//---------------------------------------------------
	class br {};

	std::ostream& operator<<(std::ostream& output, const br& /*b*/) {
		return output << "<br/>";
	}

	//---------------------------------------------------
	// text
	//---------------------------------------------------
	class text {
		std::string_view str;

	public:
		constexpr text(const std::string_view &sv) : 
			str(sv)
		{}
		constexpr const std::string_view &get_view() const {
			return str;
		}
	};

	std::ostream &operator<<(std::ostream &output, const text &t) {
		return output << t.get_view();
	}

	//---------------------------------------------------
	// p
	//---------------------------------------------------
	template <typename... Args>
	class p : public Base<p<Args...>>{
		std::tuple<Args...> children;
		friend class Base<p<Args...>>;
	public:
		static constexpr std::string_view tag = "p";

		constexpr p(Args &&... args) : 
			children(std::forward<Args>(args)...)
		{
			static_assert((... + (std::is_same_v<Args, text> ? 1 : 0)) < 2, 
				"Cannot have 2 text elements in a p");
		}
	};
	template <typename... Args>
	p(Args &&...)->p<Args...>;

	//---------------------------------------------------
	// h1
	//---------------------------------------------------


	//---------------------------------------------------
	// div
	//---------------------------------------------------
	template <typename... Args>
	class div
	{
		std::tuple<Args...> children;

	public:
		constexpr div(Args &&... args) : 
			children(std::forward<Args>(args)...)
		{
		}

		std::ostream &streamOut(std::ostream &stream) const
		{
			stream << "<div>\n";
			std::apply([&stream](const auto &... item) { ((stream << item << "\n"), ...); }, children);
			stream << "</div>\n";
			return stream;
		}
	};
	template <typename... Args>
	div(Args &&...)->div<Args...>;

	template <typename... Args>
	std::ostream &operator<<(std::ostream &stream, const div<Args...> &d)
	{
		return d.streamOut(stream);
	}

	//---------------------------------------------------
	// body
	//---------------------------------------------------
	template <typename... Args>
	class body
	{
		std::tuple<Args...> children;

	public:
		constexpr body(Args &&... args) : 
			children(std::forward<Args>(args)...)
		{
		}

		std::ostream &streamOut(std::ostream &stream) const
		{
			stream << "<body>\n";
			std::apply([&stream](const auto &... item) { ((stream << item << "\n"), ...); }, children);
			stream << "</body>\n";
			return stream;
		}
	};
	template <typename... Args>
	body(Args &&...)->body<Args...>;

	template <typename... Args>
	std::ostream &operator<<(std::ostream &stream, const body<Args...> &d)
	{
		return d.streamOut(stream);
	}

	//---------------------------------------------------
	// html
	//---------------------------------------------------
	template <typename T>
	struct is_body
	{
		static const bool value = false;
	};
	template <typename... Args>
	struct is_body<Web::body<Args...>>
	{
		static const bool value = true;
	};

	template <typename... Args>
	class html
	{
		std::tuple<Args...> children;

	public:
		constexpr html(Args &&... args) : 
			children(std::forward<Args>(args)...)
		{
			static_assert((0 + ... + (is_body<Args>::value ? 1 : 0)) == 1, 
				"Must have 1 body in a html document");
		}

		std::ostream &streamOut(std::ostream &stream) const
		{
			stream << "<html>\n";
			std::apply([&stream](const auto &... item) { ((stream << item), ...); }, children);
			stream << "</html>\n";
			return stream;
		}
	};
	template <typename... Args>
	html(Args &&...)->html<Args...>;

	template <typename... Args>
	std::ostream &operator<<(std::ostream &stream, const html<Args...> &d)
	{
		return d.streamOut(stream);
	}
}

int main()
{
	using namespace Web;

	/*auto para = p{
		text{ "Text" }
	};
	std::cout << para << "\n";

	constexpr auto t = text{ "text" };
	std::cout << t << "\n";*/

	constexpr auto d = Web::div {
		p { text { "para1" } },
		br {},
		p { text { "para2" } }
	};

	std::cout << d << "\n";

	constexpr auto doc = html {
		body {
			p { text { "Hello World" } }
		}
	};

	std::cout << doc << "\n";

	auto b = body{};
	std::cout << b << "\n";

	constexpr auto doc2 = html {
		body {
			p { text { "people" } }
		}
	};
	std::cout << doc2 << "\n";

	return 0;
}
