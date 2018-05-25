#include "catch.hpp"
#include <web.hpp>
#include <iostream>
#include <string>
#include <sstream>

namespace {
template <typename T>
std::string toString(T&& t) {
	std::stringstream ss;
	ss << t;
	return ss.str();
}
}

TEST_CASE("P can contain text")
{
	auto paragraph = Web::P {
		"Some text"
	};

	auto paragraphString = toString(paragraph);

	REQUIRE(paragraphString == "<p>Some text</p>");
}

TEST_CASE("Div can contain a br")
{
	auto d = Web::Div{
		Web::Br{}
	};

	auto divText = toString(d);
	REQUIRE(divText == "<div><br/></div>");
}

TEST_CASE("H1 produces correct text")
{
	auto header = Web::H1 {
		"text"
	};
	auto headerText = toString(header);
	REQUIRE(headerText == "<h1>text</h1>");
}

TEST_CASE("H2 produces correct text")
{
	auto header = Web::H2 {
		"text"
	};
	auto headerText = toString(header);
	REQUIRE(headerText == "<h2>text</h2>");
}

TEST_CASE("HTML can contain a body tag")
{
	auto doc = Web::Html {
		Web::Body {
		}
	};

	auto docText = toString(doc);
	REQUIRE(docText == "<html><body></body></html>");
}

TEST_CASE("div can contain a paragraph")
{
	auto data = Web::Div {
		Web::P {
			"text"
		}
	};

	auto dataText = toString(data);
	REQUIRE(dataText == "<div><p>text</p></div>");
}

TEST_CASE("Div can contain a vector of paragraphs")
{
	std::vector<Web::P> paragraphs;
	paragraphs.push_back(Web::P{ "first" });
	paragraphs.push_back(Web::P{ "second" });
	auto d = Web::Div {
		std::move(paragraphs)
	};

	auto divText = toString(d);
	REQUIRE(divText == "<div><p>first</p><p>second</p></div>");
}

TEST_CASE("Div can have a class attribute")
{
	auto d = Web::Div {
		Web::Attr {
			Web::Class("basic")
		}
	};

	auto divText = toString(d);
	REQUIRE(divText == R"(<div class="basic"></div>)");
}

TEST_CASE("Div can have multiple attributes")
{
	auto d = Web::Div {
		Web::Attr {
			Web::Class("basic"),
			Web::Id("first")
		}
	};

	auto divText = toString(d);
	REQUIRE(divText == "<div class=\"basic\" id=\"first\"></div>");
}