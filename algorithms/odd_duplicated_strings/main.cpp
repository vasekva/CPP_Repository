#include <unordered_map>
#include <iostream> 
#include <vector>

/**
 * 0(n)
 *
 * Напишите функцию, которая принимает на вход массив строк, и
 * возвращает такой же массив, но без тех строк, которых в исходном массиве
 * было чётное количество. Оставшиеся в массиве элементы должны идти в том же
 * порядке друг относительно друга, что и в первоначальном массиве.
 * */

std::vector<std::string> get_odd_repeated(const std::vector<std::string> &arr)
{
    std::vector<std::string> newVector;
    std::unordered_map<std::string, int> map;

    for (int i = 0; i < arr.size(); i++)
		map[arr[i]]++;

    for (int i = 0; i < arr.size(); i++)
    {
        if (map[arr[i]] != -1 && map[arr[i]] % 2 != 0)
        {
           newVector.push_back(arr[i]);
			map[arr[i]] = -1;
        }
    }
    return (newVector);
}
 
int main()
{
    std::vector<std::string> arr = { "one", "two", "three", "three", "two", "three", "what", "1"};
    
    std::vector<std::string> newVec = get_odd_repeated(arr);
    
    for (auto s : newVec)
        std::cout << s << " ";
    std::cout << std::endl;
    
    return 0;
}
