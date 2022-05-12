#include "tools.hpp"

std::set<std::string> convert_to_set(std::vector<std::string> &v)
{
	std::set<std::string> my_set;

	for (std::string &str : v)
		my_set.insert(str);
	return (my_set);
}

std::vector<std::string>	get_column(const std::vector<std::vector<std::string>> &bd_data, int ind)
{
	std::vector<std::string> vector_column;

	if (bd_data.empty() || ind < 0 || ind >= bd_data[0].size())
		return (vector_column);

	for (int i = 0; i < bd_data.size(); i++)
		vector_column.push_back(bd_data[i][ind]);
	return (vector_column);
}

/** Возвращает колонку связанную с пользователем по UUID и преобразованную в список строк */
std::vector<std::string> get_user_column(const std::vector<std::vector<std::string>> &bd_data, std::string UUID, int column)
{
	std::vector<std::string> vector_column;

	if (bd_data.empty() || column < 2 || column >= bd_data[0].size())
		return (vector_column);

	/** Получаем индексы начала и конца последовательности информации связанной с UUID */
	std::vector<int> indxs = get_sequence_frame(bd_data, UUID);
	if (indxs.empty())
		return (vector_column);

//	std::cout << "get_user_column() indxs: " << indxs[0] << " " << indxs[1] << std::endl;

	for (int i = indxs[0]; i != indxs[1]; i++)
	{
//		std::cout << "get_user_column(): " << bd_data[i][column] << std::endl;
		vector_column.push_back(bd_data[i][column]);
	}
	return (vector_column);
}


/** Возврвщает вектор, в котором лежат индексы начала и конца последовательности информации пользователя (UUID) */
std::vector<int> get_sequence_frame(const std::vector<std::vector<std::string>> &bd_data, const std::string &UUID)
{
	std::vector<int> indxs;
	std::vector<std::string>::iterator bgn;
	std::vector<std::string>::iterator end;

	/** Получаем колонку */
	std::vector<std::string> vector_column = get_column(bd_data, 0);
	/** Проверяем есть ли такой UUID в списке */
	if ((bgn = std::find(vector_column.begin(), vector_column.end(), UUID)) == vector_column.end())
		return (indxs); // возвращается пустой список

	//TODO: удалить
//	std::cout << "Ищем: " << UUID << std::endl;
//	std::cout << "Нашли: " << *bgn << std::endl;


	/** Итерируемся пока не дойдем до конца, либо пока не дойдем до нового UUID */
	for (end = bgn; end != vector_column.end(), *end == UUID; end++);

	indxs.push_back(std::distance(vector_column.begin(), bgn)); // индекс начала
	indxs.push_back(std::distance(vector_column.begin(), end)); // индекс конца

	return (indxs);
}

/**
 * Возвращает вектор, в котором лежат индексы начала и конца информации пользователя (UUID),
 * за последнее (time) время от времени последнего сообщения пользователя
 * */
std::vector<int> get_sequence_frame_by_time(const std::vector<std::vector<std::string>> &bd_data, const std::string &UUID, int time)
{
	std::vector<int> frames;

	if (time < 1 || time > 60)
	{
		std::cerr << "get_sequence_frame_by_time() error: invalid time" << std::endl;
		return (frames);
	}
	frames = get_sequence_frame(bd_data, UUID);
	if (frames.empty())
		throw std::runtime_error("get_sequence_frame_by_time() error: UUID не найден!!");

	/** минимальное время первого сообщения, которое ищем (последнее сообщение - time) */
	size_t diff_time = std::atoi(bd_data[frames[1] - 1][1].substr(3, 2).c_str()) - time;

	/** время текущего по индексу сообщения сообщения */
	size_t curr_time = std::atoi(bd_data[frames[0]][1].substr(3, 2).c_str());


	//TODO: удалить
//	std::cout << "FST TIME: " << curr_time << std::endl;
//	std::cout << "DIFF: " << diff_time << std::endl;

	int row_i = frames[0];
	if (diff_time > curr_time)
	{
//		std::cout << "WOOOOOOOW" << std::endl;

		for (; row_i != frames[1]; row_i++)
		{
			curr_time = std::atoi(bd_data[row_i][1].substr(3, 2).c_str());
			if (curr_time == diff_time)
				break;
		}
		frames[0] = row_i;
	}
	//TODO: удалить
//	std::cout << "get_sequence_frame_by_time(" << time << ") frames: " <<
//		frames[0] << " " << frames[1] << std::endl;

	return (frames);
}

float get_average_x_by_time(const std::vector<std::vector<std::string>> &bd_data, const std::string &UUID, int time)
{
	float average = 0.0f;
	std::vector<int> frames = get_sequence_frame_by_time(bd_data, UUID, time);
	if (frames.empty())
		throw std::runtime_error("get_average_x_by_time() error: vector 'frames' is empty!!");
	float size = 0.0;
	for (int row = frames[0]; row != frames[1]; row++)
	{
		average += std::stof(bd_data[row][2]);
		size++;
	}
	//TODO: удалить
//	std::cout << "get_average_x_by_time(): " << average / (float)(frames[1] - frames[0]) << std::endl;
	return (average);
}

int get_y_fraction_part_sum_by_time(const std::vector<std::vector<std::string>> &bd_data, const std::string &UUID, int time)
{
	int sum = 0.0f;
	int dot_pos = 0;

	std::vector<int> frames = get_sequence_frame_by_time(bd_data, UUID, time);
	if (frames.empty())
		throw std::runtime_error("get_y_fraction_part_sum_by_time() error: vector 'frames' is empty!!");

	std::string str;
	for (int row = frames[0]; row != frames[1]; row++)
	{
		dot_pos = bd_data[row][2].find('.');
//		std::cout << "dot pos: " << dot_pos << std::endl;
		str = bd_data[row][2].substr(dot_pos + 1, (bd_data[row][2].length() - dot_pos));
//		std::cout << "STR: " << str << std::endl;
		sum += stoi(str);
	}
	//TODO: удалить
//	std::cout << "get_average_x_by_time(): " << average / (float)(frames[1] - frames[0]) << std::endl;
	return (sum);
}