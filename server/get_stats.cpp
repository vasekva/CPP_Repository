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
	std::vector<std::vector<std::string> > records;
	char *errmsg;

	int ret = sqlite3_exec(db, stmt.c_str(), select_callback, &records, &errmsg);
	if (ret != SQLITE_OK)
	{
		std::cerr << "Error in select statement " << stmt << "[" << errmsg << "]\n";
	}
	else
	{
		std::cerr << records.size() << " records returned.\n";
	}

	return (records);
}

// TODO: метод для реализации функционала статистики
std::string Session::get_stats()
{
	std::cout << "OH YEAH STATS" << std::endl;
	std::vector<std::vector<std::string> > records = select_stmt("SELECT * FROM " + TABLE_NAME, _db);
	// 0 - UUID
	// 1 - TIME
	// 2 - X
	// 3 - Y



	/** Получаю полный список UUID строк*/
	std::vector<std::string> vctr_column = get_column(records, 0); // <---- cписок всех UUID
	/** Получаю набор уникальных UUID строк */
	std::set<std::string> uuid_set = convert_to_set(vctr_column); // <---- набор уникальных UUID

//	/** Получаю список X значений по UUID[0] */
//	std::vector<std::string> uuids_x = get_user_column(records, (*uuid_set.begin()), 2);
//	std::cout << "X Size: " << uuids_x.size() << std::endl;
//	for (const std::string &str : uuids_x)
//		std::cout << str << std::endl;
//
//	/** Получаю список Y значений по UUID[0] */
//	std::vector<std::string> uuids_y = get_user_column(records, (*uuid_set.begin()), 3);
//	std::cout << "Y Size: " << uuids_y.size() << std::endl;
//	for (const std::string &str : uuids_y)
//		std::cout << str << std::endl;
//
//	/** Получаю список значений времени по UUID[0] */
//	std::vector<std::string> uuids_time = get_user_column(records, (*uuid_set.begin()), 1);
//	std::cout << "Time Size: " << uuids_y.size() << std::endl;
//	for (const std::string &str : uuids_time)
//		std::cout << str << std::endl;

	std::set<std::string>::iterator bgn = uuid_set.begin();
	std::cout << "SIZE records Y: " << records.size() << std::endl;
	std::cout << "SIZE records X: " << records[0].size() << std::endl;

	/** возвращает первый и последний индекс совпадения */
	std::vector<int> frames;

	std::cout << "YEAAAAH, LET's GOOOOOOO" << std::endl;

	int ind = 0; //TODO: <---- удалить
	for (; bgn != uuid_set.end(); bgn++)
	{
		frames = get_sequence_frame(records, *bgn);
		for (int row = frames[0]; row != frames[1]; row++)
		{
			std::cout << ind++ << ": " <<
				records[row][1] << " " <<
				records[row][2] << " " <<
				records[row][3] << " " <<
				std::endl;
		}
		std::cout << "========================================" << std::endl;
	}

//	std::vector<int> idxs = get_sequence_frame(records, vctr_column[0]);


	// Получить список UUID
	// Сделать цикл по UUID
	// Получение последнего сообщения связанного с UUID
	// 1) Выборка и суммированние всех X значений данного пользователя,
	// где время в промежутке (lst.msg time - 1min)
	// 2) Выборка и суммированние всех Y значений данного пользователя,
	// где время в промежутке (lst.msg time - 1min)
	// 3) Выборка и суммированние всех X значений данного пользователя,
	// где время в промежутке (lst.msg time - 5min)
	// 4) Выборка и суммированние всех Y значений данного пользователя,
	// где время в промежутке (lst.msg time - 5min)

	return ("");
}

/**
* 1) Получил двумерный массив векторов строк информации из базы данных
*/