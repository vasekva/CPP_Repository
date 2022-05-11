#include "tools.hpp"

std::set<std::string> convert_to_set(std::vector<std::string> &v)
{
	std::set<std::string> my_set;

	for (std::string &str : v)
		my_set.insert(str);
	return (my_set);
}

std::vector<std::string>	get_column(std::vector<std::vector<std::string>> &bd_data, int ind)
{
	std::vector<std::string> vector_column;

	if (bd_data.empty() || ind < 0 || ind >= bd_data[0].size())
		return (vector_column);

	for (int i = 0; i < bd_data.size(); i++)
		vector_column.push_back(bd_data[i][ind]);
	return (vector_column);
}

/** Возвращает колонку связанную с пользователем по UUID и преобразованную в список строк */
std::vector<std::string> get_user_column(std::vector<std::vector<std::string>> &bd_data, std::string UUID, int column)
{
	std::vector<std::string> vector_column;

	if (bd_data.empty() || column < 2 || column >= bd_data[0].size())
		return (vector_column);

	/** Получаем индексы начала и конца последовательности информации связанной с UUID */
	std::vector<int> indxs = get_sequence_frame(bd_data, UUID);
	if (indxs.empty())
		return (vector_column);

	for (int i = indxs[0]; i != indxs[1]; i++)
	{
		std::cout << "get_user_column(): " << bd_data[i][column] << std::endl;
		vector_column.push_back(bd_data[i][column]);
	}
	return (vector_column);
}


/** Возврвщает вектор, в котором лежат индексы начала и конца последовательности информации UUID */
std::vector<int> get_sequence_frame(std::vector<std::vector<std::string>> &bd_data, std::string UUID)
{
	std::vector<int> indxs;
	std::vector<std::string>::iterator bgn;
	std::vector<std::string>::iterator end;

	/** Получаем колонку */
	std::vector<std::string> vector_column = get_column(bd_data, 0);
	/** Проверяем есть ли такой UUID в списке */
	if ((bgn = std::find(vector_column.begin(), vector_column.end(), UUID)) == vector_column.end())
		return (indxs); // возвращается пустой список

	std::cout << "Ищем: " << UUID << std::endl;
	std::cout << "Нашли: " << *bgn << std::endl;
	if (bgn != vector_column.begin())
		std::cout << "Предыдущее значение от итератора: " << *(bgn - 1) << std::endl;
	else
		std::cout << "Мы в самом начале!: " << *bgn << std::endl;

	/** Итерируемся пока не дойдем до конца, либо пока не дойдем до нового UUID */
	for (end = bgn; end != vector_column.end(), *end == UUID; end++);

	if (end != vector_column.end())
	{
		std::cout << "Нашли: " << *end << std::endl;
		std::cout << "Следующее значение от итератора: " << *(end + 1) << std::endl;
		if (end != vector_column.begin())
			std::cout << "Предыдущее значение от итератора: " << *(end - 1) << std::endl;
		else
			std::cout << "Мы в самом начале!: " << *end << std::endl;
	}
	else
		std::cout << "Мы в самом конце!: " << *end << std::endl;

	indxs.push_back(std::distance(vector_column.begin(), bgn)); // индекс начала
	indxs.push_back(std::distance(vector_column.begin(), end)); // индекс конца

	return (indxs);
}