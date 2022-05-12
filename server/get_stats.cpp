#include "Session.hpp"
#include "tools.hpp"

static int select_callback(void *p_data, int num_fields, char **p_fields, char **p_col_names)
{
	auto *records = static_cast<std::vector<std::vector<std::string> > *>(p_data);
	try
	{
		records->emplace_back(p_fields, p_fields + num_fields);
	}
	catch (...)
	{
		// abort select on failure, don't let exception propogate thru sqlite3 call-stack
		return (1);
	}
	return (0);
}

static std::vector<std::vector<std::string> > select_stmt(const std::string stmt, sqlite3 *db)
{
	char *errmsg;
	std::vector<std::vector<std::string>> records;

	int ret = sqlite3_exec(db, stmt.c_str(), select_callback, &records, &errmsg);
	if (ret != SQLITE_OK)
		std::cerr << "Error in select statement " << stmt << "[" << errmsg << "]\n";
	else
		std::cerr << records.size() << " records returned.\n";

	return (records);
}

std::string Session::get_stats()
{
	// 0 - UUID // 1 - TIME // 2 - X // 3 - Y
	std::vector<std::vector<std::string> > records = select_stmt("SELECT * FROM " + TABLE_NAME, _db);

	/** Получаю колонку UUID строк */
	std::vector<std::string> vctr_column = get_column(records, 0); // <---- cписок всех UUID
	/** Получаю набор уникальных UUID строк */
	std::set<std::string> uuid_set = convert_to_set(vctr_column); // <---- набор уникальных UUID


	std::set<std::string>::iterator bgn = uuid_set.begin();
	std::cout << "Rows: " << records.size() << std::endl;
	std::cout << "Columns: " << records[0].size() << std::endl;

	/** возвращает первый и последний индекс совпадения */
	std::vector<int> frames;

	std::string statistic; // для отправки статистики
	std::stringstream str_stream;
	for (; bgn != uuid_set.end(); bgn++)
	{
		statistic.append(*bgn) += " ";

		/** получение среднего X и сумму модуля Y за 1 минуту */
		str_stream << get_average_x_by_time(records, *bgn, 1);
		statistic.append(str_stream.str()) += " ";
		statistic.append(std::to_string(get_y_fraction_part_sum_by_time(records, *bgn, 1))) += " ";
		str_stream.str(std::string()); // чистка stringstream

		/** получение среднего X и сумму модуля Y за 5 минут */
		str_stream << get_average_x_by_time(records, *bgn, 5);
		statistic.append(str_stream.str()) += " ";
		statistic.append(std::to_string(get_y_fraction_part_sum_by_time(records, *bgn, 5))) += "\n";
		str_stream.str(std::string()); // чистка stringstream
	}
	return (statistic);
}