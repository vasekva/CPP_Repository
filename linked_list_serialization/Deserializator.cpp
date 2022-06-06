#include "Deserializator.hpp"

Deserializator::Deserializator() {}
Deserializator::~Deserializator() {}

std::string Deserializator::hex_to_str(const ListNode *const ptr)
{
	size_t				digit_addr;
	std::stringstream	str_stream;

	str_stream << std::hex << ptr; // from ptr to hex to binary stream
	str_stream >> digit_addr; // from binary to u_longlong

	return (std::to_string(digit_addr));
}

/**
 * Возвращает m_data звена из строки сериализационной информации.
 *
 * @param line - строка из файла, в которой хранится информация о звене
 * перед сериализацией в формате:
 * 'адрес самого звена/индекс/данные звена/адрес рандомного звена из m_rand'.
 *
 * @return Строка информации m_data звена.
 * */
std::string Deserializator::get_node_data_from_line(const std::string &line)
{
	std::string node_data;

	/** Searching for the start and end positions of a node_data line */
	size_t data_ind = line.find("data=") + 5;
	if (data_ind == std::string::npos)
	{
		std::cerr << "Incorrect format of serialization!" << std::endl;
		std::cerr << "Couldn't find a node data!" << std::endl;
		exit(1);
	}

	size_t slash_ind = std::string(&line[data_ind]).find("/");
	if (slash_ind == std::string::npos)
	{
		std::cerr << "Incorrect format of serialization!" << std::endl;
		std::cerr << "Couldn't find a slash after node data!" << std::endl;
		exit(1);
	}

	node_data = line.substr(data_ind, slash_ind);
	return (node_data);
}

/**
 * Заполняет map, в котором хранится информация о структуре указателей звена в формате:
 * 'ссылка самого звена до сериализации' : 'ссылка или nullptr звена, на которое ссылался указатель m_rand'.
 *
 * @param nodes_info - структура данных, в качестве ключа которой в строковом формате хранится адрес звена
 * перед сериализацией (адрес предыдущего состояния), а в качестве значения либо nullptr, либо адрес в
 * строковом формате указателя m_rand.
 *
 * @param line - строка из файла, в которой хранится информация о звене
 * перед сериализацией в формате:
 * 'адрес самого звена/индекс/данные звена/адрес рандомного звена из m_rand'.
 *
 * @return true, если данные корректно сохранены, либо false в ином случае
 * */
bool Deserializator::add_new_item_to_nodes_info(std::map<std::string, std::string> &nodes_info, const std::string &line)
{
	std::string addr_from;
	std::string addr_to;

	size_t slash_pos = line.find("/");
	if (slash_pos == std::string::npos)
	{
		std::cerr << "deserialize error: incorrect find return\n";
		return (false);
	}
	addr_from = line.substr(0, slash_pos);

	slash_pos = line.find_last_of("/");
	if (slash_pos == std::string::npos)
	{
		std::cerr << "deserialize error: incorrect find return\n";
		return (false);
	}
	addr_to = line.substr(slash_pos + 1);

	if (addr_from.empty() || addr_to.empty())
		return (false);

	nodes_info.emplace(addr_from, addr_to);
	return (true);
}

/**
 * Десериализует двусвязный список (без создания третьих указателей),
 * сохраняя необходимую информацию для дальнейших действий в nodes_map
 * и node_info.
 * Благодаря чему после этого в make_pointers() создается третий указатель(если был) у каждого звена.
 *
 * @param curr - главный указатель, от которого создается список
 *
 * @param tail_ptr - указатель, в который сохраняется адресс последнего звена списка
 *
 * @param nodes_map - структура данных, в качестве ключа которой хранится адрес звена перед сериализацией,
 * (адрес предыдущего состояния) и в качестве значения указатель на адрес в памяти новосозданного объекта
 * (нового состояния).
 *
 * @param nodes_info - структура данных, в качестве ключа которой в строковом формате хранится адрес звена
 * перед сериализацией (адрес предыдущего состояния), а в качестве значения либо nullptr, либо адрес в
 * строковом формате указателя m_rand.
 *
 * @param in_file - поток чтения из файла
 *
 * @param count - значение для сохранения размера списка
 * */
void Deserializator::deserialize(ListNode **curr,
	ListNode **tail_ptr,
	std::map<unsigned long long, ListNode *> &nodes_map,
	std::map<std::string, std::string> &nodes_info,
	std::ifstream &in_file,
	int &count)
{
	std::string line;
	std::string node_data;

	ListNode *curr_ptr;
	ListNode *tmp_ptr;

	while (true)
	{
		in_file >> line;
		if (in_file.eof() || line.empty())
			break;
		node_data = get_node_data_from_line(line); // получение данных предыдущего состояния звена

		/** deserialization */
		if (count == 0) // создание стартового звена
		{
			tmp_ptr = new ListNode(node_data);
			(*curr) = tmp_ptr;
		}
		else // добавление нового звена в конец
		{
			curr_ptr = tmp_ptr;
			tmp_ptr->m_next = new ListNode(node_data);
			tmp_ptr = tmp_ptr->m_next;
			tmp_ptr->m_prev = curr_ptr;
		}

		// позиция в строке, на которой заканчивается адрес предыдущего состояния звена
		size_t slash_ind = line.find("/");
		if (slash_ind == std::string::npos)
		{
			std::cerr << "Incorrect format of serialization!" << std::endl;
			std::cerr << "Couldn't find a slash after node data!" << std::endl;
			exit(1);
		}
		/** key - адрес звена до сериализации, value - указатель на новое состояние звена */
		nodes_map.emplace(atol(line.substr(0, slash_ind).c_str()), tmp_ptr);

		/** добавляю новый элемент в map, сохраняя информацию о том,
		 * какое состояние указателя m_rand(value) имело звено под адресом (key) */
		if (!add_new_item_to_nodes_info(nodes_info, line))
		{
			std::cerr << "add_new_item_to_nodes_info error" << std::endl;
			exit(1);
		}

		count++;
	}
	(*tail_ptr) = tmp_ptr;
}

/**
 * Заполняет указатель m_rand адресом другого звена, на которое ранее указывал элемент,
 * если в предыдущем состоянии атрибут m_rand ссылался на другое звено.
 *
 * @param addrs_with_nodes - структура данных, в качестве ключа которой хранится адрес звена перед сериализацией,
 * (адрес предыдущего состояния) и в качестве значения указатель на адрес в памяти новосозданного объекта
 * (нового состояния).
 *
 * @param nodes_info - структура данных, в качестве ключа которой в строковом формате хранится адрес звена
 * перед сериализацией (адрес предыдущего состояния), а в качестве значения либо nullptr, либо адрес в
 * строковом формате указателя m_rand.
 * */
void Deserializator::make_pointers(
	const std::map<unsigned long long, ListNode *> &addrs_with_nodes,
	const std::map<std::string, std::string> &nodes_info)
{
	ListNode *from = nullptr;
	ListNode *to = nullptr;
	std::string address;
	std::string line;

	for (auto it = nodes_info.begin(); it != nodes_info.end(); ++it)
	{
		address = it->second;
		if (address != "nullptr")
		{
			try
			{
				to = addrs_with_nodes.at(atol(address.c_str()));
				from = addrs_with_nodes.at(atol((it->first).c_str()));
				from->m_rand = to;
			}
			catch (...)
			{
				std::cerr << "make_pointers error: incorrect address for key of map\n";
			}
		}
	}
}

ListNode *Deserializator::deserialize_list(std::ifstream &in_file, ListNode **tail_ptr, int &count)
{
	ListNode *new_data = nullptr;
	/** структура данных с адресом предыдущего состояния
	в виде ключа и указателем на звено в значении */
	std::map<unsigned long long, ListNode *>	addrs_with_nodes;
	std::map<std::string, std::string>			nodes_info;

	deserialize(&new_data, tail_ptr, addrs_with_nodes, nodes_info, in_file, count);

	make_pointers(addrs_with_nodes, nodes_info);

	return (new_data);
}