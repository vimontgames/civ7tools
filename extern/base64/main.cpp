#include "base64.hpp"

#include <iomanip>
#include <iostream>
#include <iterator>

void test(const std::string& what, const std::string& expected)
{
	auto actual = base64::encode(what);

	std::cout << "base64(\"" << what << "\") = " << expected << ": " << (actual == expected) << " | "
	          << (base64::decode(actual) == what) << '\n';
}

int main()
{
	std::cout << std::boolalpha;

	/*auto b = "f";
	auto e = b + 1;

	base64::encode(b, e, std::ostream_iterator<char>(std::cout));*/

	std::string str = "Zm9=";

	base64::decode(str.begin(), str.end(), std::ostream_iterator<char>(std::cout));

	std::cout << std::endl;

	test("", "");
	test("f", "Zg==");
	test("fo", "Zm8=");
	test("foo", "Zm9v");
	test("foob", "Zm9vYg==");
	test("fooba", "Zm9vYmE=");
	test("foobar", "Zm9vYmFy");
}