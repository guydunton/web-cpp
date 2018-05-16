#include <iostream>
#include "web.hpp"


int main()
{
	auto para = Web::P{ "hello" };
	std::cout << para << "\n";

	auto div = Web::Div {
		Web::P { "Hello World" }
	};
	std::cout << div << "\n";

	auto doc1 = Web::Html {
		Web::Body {
			Web::H1{ "This is our title" },
			Web::Br{},
			Web::P { "Hello World" }
		}
	};
	std::cout << doc1 << "\n";

	std::cout << "Press enter to close";
	std::cin.get();
	return 0;
}
