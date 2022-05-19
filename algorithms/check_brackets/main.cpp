#include <iostream>

/**
 * В символьной строке помимо прочих символов имеются открывающие "(" и закрывающие ")" круглые скобки.
 * Необходимо проверить, правильно ли они расставлены?
 * В случае, если скобки расставлены верно, программа должна выводить "1", в противном случае "0".
 *
 * Пример корректной строки со скобками:
 * "(((abc) (def) (ghj) ) x y z)"
 *
 * */

int check_brackets(std::string &str)
{
	int check = 0;
	for (int i = 0; i < str[i]; i++)
	{
		if (str[i] == ')')
			--check;
		if (check < 0)
			return (0);
		if (str[i] == '(')
			++check;
	}
	if (check == 0)
		return (1);
	else
		return (0);
}

int main()
{
	std::string str;

	std::getline(std::cin, str);

	std::cout << check_brackets(str) << std::endl;
}