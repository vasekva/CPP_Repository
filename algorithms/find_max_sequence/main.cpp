#include <vector>
#include <iostream>

/**
 * clang++ -std=c++11 ./main.cpp
 *
 * Возвращает размер максимальной последовательности одинаковых чисел из массива
 */

int find_max_sequence(std::vector<int> &nums)
{
	int query = 0;
	int diff = 0;
	int pos = 0;

	if (nums.size() <= 1)
		query = nums.size();
	else
	{
		for (int i = 0; i < nums.size() - 1; i++)
		{
			if (nums[i] != nums[i + 1])
			{
				diff = (i + 1) - pos;
				if (query < diff)
					query = diff;
				pos = i + 1;
			}
			else if (i + 1 == nums.size() - 1)
			{
				if (nums[i] == nums[i + 1])
					query = nums.size() - pos;
			}
		}
	}
	return (query);
}

int main()
{
	std::vector<int> arr = { 0, 1, 2, 4, 4, 4, 6, 1, 5, 5, 5, 5, 5, 2 };

	std::cout << "Result is: " << find_max_sequence(arr) << std::endl;

	return 0;
}
