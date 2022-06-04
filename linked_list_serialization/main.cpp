//#include "Data.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

#include <time.h>
#include <random>

#include <map>

class ListNode
{
	public:
		ListNode	*m_prev;
		ListNode	*m_next;
		ListNode	*m_rand;
		std::string m_data;

		ListNode(std::string data)
		{
			this->m_prev = nullptr;
			this->m_next = nullptr;
			this->m_rand = nullptr;
			this->m_data = data;
		}
		~ListNode() {}
};

unsigned long long hex_to_digit(const ListNode *const ptr)
{
	std::stringstream str_stream;
	unsigned long long digit = 0;

	str_stream << std::hex << ptr; // from ptr to hex to binary stream
	str_stream >> digit; // from binary to u_longlong

	return (digit);
}



void recursion_serialize(const ListNode *const ptr, size_t &ind, std::ofstream &out_file)
{
	std::stringstream	str_stream;
	std::string 		str;

	if (ptr == nullptr)
		return;

	/** '/' - param delimeter */
	str_stream << hex_to_digit(ptr) << "/"; // from het to u_longlong and to serialization stream
	str_stream << ind++ << "/";
	str_stream << "data=" << ptr->m_data << "/";

	if (ptr->m_rand)
	{
		str_stream << hex_to_digit(ptr->m_rand);
		str_stream >> str;
	}
	else
	{
		str_stream << "nullptr";
	}
	str_stream >> str;

	recursion_serialize(ptr->m_next, ind, out_file); // recursion

	str.append("\n"); // data delimeter
	out_file << str;
}

bool serialize_list(const ListNode *const ptr, const std::string &file_name)
{
	size_t ind = 0;

	std::ofstream out_file("some_file.txt");
	if (!out_file)
	{
		puts("Couldn't open the file");
		return (false);
	}
	recursion_serialize(ptr, ind, out_file); // рекурсивная сериализация списка
	out_file.close();
	return (true);
}


void recursion_deserialize(ListNode **ptr, ListNode **next, std::ifstream &in_file)
{
	std::string line;
	std::string node_data;

	if (in_file.eof())
	{
		return;
	}
	in_file >> line;
	if (line.empty())
		return;

	/** Searching for the start and end positions of node_data */
	size_t data_ind = line.find("data=");
	if (data_ind == std::string::npos)
	{
		std::cerr << "Couldn't find a node data!" << std::endl;
		exit(1);
	}

	size_t slash_ind = std::string(&line[data_ind]).find("/");
	node_data = line.substr(data_ind, slash_ind);

	/** deserialization */
	(*ptr) = new ListNode(node_data);
	if (next && (*next) != nullptr)
	{
		(*ptr)->m_next = *next;
	}
	recursion_deserialize(&(*ptr)->m_prev, ptr, in_file);
}


// вектор расположен в убывающем порядке адресов!!!
/**
 * бинарный поиск индекса звена(индекс находится в строке информации о звене)
 * из списка информации всех звеньев
 * */
size_t find_ind_of_node(const std::vector<std::string> &nodes_info, const unsigned long long &desired_addr)
{
	size_t pos;
	size_t low = 0, high = nodes_info.size() - 1, mid = (low + high) / 2;

	unsigned long long curr_addr = 0; // an address of the node itself
//	unsigned long long rand_addr = 0; // an address of the additional node's pointer

	while (low <= high)
	{
		curr_addr = std::atol(nodes_info[mid].c_str());

		if (desired_addr == curr_addr)
			break;
		else if (desired_addr > curr_addr)
			high = mid - 1;
		else
			low = mid + 1;

		mid = (high + low) / 2;
	}

	size_t fst_slash = nodes_info[mid].find("/");
	// длина числа, вычисляемая путем нахождения индекса второго слеша с позиции первого
	size_t num_len = std::string(&nodes_info[mid][fst_slash + 1]).find("/");

	//TODO:: могут быть погрехи в вытаскивании индекса из строки
	pos = atoi(nodes_info[mid].substr(fst_slash + 1, num_len).c_str());

	return (pos);
}

ListNode *deserialize_list(void)
{
//	std::map<unsigned long long, std::pair<int, std::string>> nodes_info;
	ListNode *new_data = nullptr;

	std::ifstream in_file("some_file.txt");
	if (!in_file)
	{
		puts("Couldn't open the file");
		return (nullptr);
	}
	recursion_deserialize(&new_data, nullptr, in_file);
	in_file.close();


/**
===================================
==  filling the map with values  ==
===================================
*/
	std::ifstream file("some_file.txt");
	std::string line;
	std::vector<std::string> nodes_info;

	while (true)
	{
		file >> line;
		if (file.eof() || line.empty())
			break;
		nodes_info.push_back(line);
	}

	for (auto str : nodes_info)
		std::cout << str << std::endl;

	size_t lst_slash = nodes_info[0].find_last_of("/");
	unsigned long long addr = atol(std::string(&nodes_info[0][lst_slash + 1]).c_str());
	find_ind_of_node(nodes_info, addr); // TODO: удалить


//	unsigned long long key = 0;
//	std::pair<int, std::string> value;
//	int delimeter = 0;
//	while (!file.eof())
//	{
//		file >> line;
//		if (line.empty())
//			break;
//
//		delimeter = line.find("/");
//		key = atol(line.substr(0, delimeter).c_str());
//		if (key == 0)
//			break;
//		line.erase(0, delimeter + 1);
//
//		delimeter = line.find("/");
//		value = std::make_pair(atoi(line.substr(0, delimeter).c_str()),
//			std::string(&line[delimeter + 1]));
//		nodes_info.emplace(key, value);
//	}
//	file.close();
//
//
//	std::map<unsigned long long, std::pair<int, std::string>>::const_iterator it = nodes_info.cbegin();
//	unsigned long long map_key = 0;
//	std::string address;
//	size_t		pair_key;
//	std::string pair_value;
//	for (; it != nodes_info.end(); ++it)
//	{
//		/** если у звена есть ссылка на другое звено */
//		if (it->second.second.find("nullptr") == std::string::npos)
//		{
//			map_key = it->first; // сохраняем ключ от nodes_info
//			pair_value = it->second.second; // сохраняем значение пары
//			pair_key = it->second.first; // сохраняем ключ пары
//			address = pair_value.substr(pair_value.find_last_of("/") + 1); // сохраняем адресс другого звена
//			pair_value += '-';
//			size_t ind = nodes_info.at(atol(address.c_str())).first; // получаем индекс звена
//			pair_value.append(std::to_string(ind));
//
//			// заменяем значение в map (добавляем значение индекса звена, адрес которого лежит в m_rand)
//			nodes_info.erase(map_key);
//			std::cout << nodes_info.size() << std::endl;
//			nodes_info.emplace(map_key, std::make_pair(pair_key, pair_value));
//			std::cout << nodes_info.size() << std::endl;
//		}
//	}
//
//	it = nodes_info.cbegin();
//	for (; it != nodes_info.end(); ++it)
//	{
//		std::cout << it->first << ":(" <<
//				  it->second.first << ", " << it->second.second << ")" << std::endl;
//	}

	return (new_data);
}

/** Creating a linked list with size N */
void create_list(ListNode **tmp_ptr, ListNode **main_obj, const size_t &N)
{
	std::string node_data = "some_data";

	for (int i = 0; i < N; i++)
	{
		if (i == 0)
		{
			*tmp_ptr = new ListNode(node_data + (std::to_string(i)));
			*main_obj = *tmp_ptr;
		}
		else
		{
			//TODO: проверить корректность создания двусвязного списка
			(*tmp_ptr)->m_next = new ListNode(node_data + std::to_string(i));
			(*tmp_ptr)->m_next->m_prev = (*tmp_ptr);
			(*tmp_ptr) = (*tmp_ptr)->m_next;
		}
	}
}

/**
 * Creating a new pointer from
 * a node № ind_from to
 * a node № ind_to
 * */
bool	make_ptr(ListNode *list, const size_t &ind_from, const size_t &ind_to)
{
	ListNode *from = list;
	ListNode *to = list;

	for (size_t i = 0; i != ind_from; i++)
	{
		if (from->m_next == nullptr)
			return (false);
		from = from->m_next;
	}
	for (size_t i = 0; i != ind_to; i++)
	{
		if (to->m_next == nullptr)
			return (false);
		to = to->m_next;
	}

	from->m_rand = to;
	return (true);
}

int main(void)
{
	ListNode	*main_object = nullptr;
	ListNode	*deserialized_obj = nullptr;
	ListNode	*tmp_ptr = nullptr;


	const size_t list_size = 5;
	/** Creating a linkedList with N size */
	create_list(&tmp_ptr, &main_object, list_size);

	/** Creating additional pointers to another Nodes for other ones */
	if (make_ptr(main_object, 1, 1))
		std::cout << "Created an addition pointer from 1 to 1\n";
	if (make_ptr(main_object, 2, 4))
		std::cout << "Created an addition pointer from 2 to 4\n";
	if (make_ptr(main_object, 4, 0))
		std::cout << "Created an addition pointer from 4 to 0\n";


	/** Serialization */
	const std::string file_name = "some_file.txt";
	if (!serialize_list(main_object, file_name))
	{
		std::cerr << "serialize error!" << std::endl;
		exit(-1);
	}

	/** Deserialization */
	deserialized_obj = deserialize_list();
	if (deserialized_obj == nullptr)
	{
		std::cerr << "deserialize error!" << std::endl;
		exit(1);
	}

/**
==========================================
==  Printing of deserialization result  ==
==========================================
*/
	/** Checking and printing deserialized data */
	if (deserialized_obj == nullptr)
	{
		std::cout << "ERROR" << std::endl;
		exit(-1);
	}

/*


	std::cout << "Its attributes: " << std::endl;
	// TODO: удалить
	if (deserialized_obj->m_prev == nullptr)
		std::cout << "Prev == nullptr\n";
	if (deserialized_obj->m_next == nullptr)
		std::cout << "Next == nullptr\n";


	while (deserialized_obj->m_prev)
	{
		std::cout << deserialized_obj->m_data << std::endl;
		deserialized_obj = deserialized_obj->m_prev;
	}
	std::cout << deserialized_obj->m_data << std::endl;
	std::cout << "==================" << std::endl;
	while (deserialized_obj->m_next)
	{
		std::cout << deserialized_obj->m_data << std::endl;
		deserialized_obj = deserialized_obj->m_next;
	}
	std::cout << deserialized_obj->m_data << std::endl;
*/
// TODO: сделать удаление всего списка
//  delete main_object;
//	 delete deserialized_obj;
}