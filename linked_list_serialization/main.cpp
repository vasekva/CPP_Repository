//#include "Data.hpp"

#include <iostream>
//#include <fstream>
#include <sstream>

#include <time.h>
#include <random>

#include <map>

#include "includes/ListRand.hpp"
#include "includes/Serializator.hpp"

void recursion_deserialize(ListNode **curr, ListNode **next,
	std::map<unsigned long long, ListNode *> &nodes_dict, std::ifstream &in_file)
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
	(*curr) = new ListNode(node_data);
	/**
	 * key - address of the node before serialization,
	 * value - ptr to the new deserialized node
	 * */
	nodes_dict.emplace(atol(line.substr(0, slash_ind).c_str()), *curr);

	if (next && (*next) != nullptr)
	{
		(*curr)->m_next = *next;
	}
	recursion_deserialize(&(*curr)->m_prev, curr, nodes_dict, in_file);
}


// вектор расположен в убывающем порядке адресов!!!
///**
// * бинарный поиск индекса звена(индекс находится в строке информации о звене)
// * из списка информации всех звеньев
// * */
//size_t find_ind_of_node(const std::vector<std::string> &nodes_info, const unsigned long long &desired_addr)
//{
//	size_t pos;
//	size_t low = 0, high = nodes_info.size() - 1, mid = (low + high) / 2;
//
//	unsigned long long curr_addr = 0; // an address of the node itself
////	unsigned long long rand_addr = 0; // an address of the additional node's pointer
//
//	while (low <= high)
//	{
//		curr_addr = std::atol(nodes_info[mid].c_str());
//
//		if (desired_addr == curr_addr)
//			break;
//		else if (desired_addr > curr_addr)
//			high = mid - 1;
//		else
//			low = mid + 1;
//
//		mid = (high + low) / 2;
//	}
//
//	size_t fst_slash = nodes_info[mid].find("/");
//	// длина числа, вычисляемая путем нахождения индекса второго слеша с позиции первого
//	size_t num_len = std::string(&nodes_info[mid][fst_slash + 1]).find("/");
//
//	//TODO:: могут быть погрехи в вытаскивании индекса из строки
//	pos = atoi(nodes_info[mid].substr(fst_slash + 1, num_len).c_str());
//
//	return (pos);
//}

ListNode *deserialize_list(void)
{
//	std::map<unsigned long long, std::pair<int, std::string>> nodes_info;
	std::map<unsigned long long, ListNode *> nodes_dict;
	ListNode *new_data = nullptr;

	std::ifstream in_file("some_file.txt");
	if (!in_file)
	{
		puts("Couldn't open the file");
		return (nullptr);
	}
	recursion_deserialize(&new_data, nullptr, nodes_dict, in_file);
	in_file.close();


	//TODO: удалить
//	for (auto it = nodes_dict.begin(); it != nodes_dict.end(); ++it)
//	{
//		std::cout << it->first << "/Val:" << it->second->m_data << std::endl;
//	}

/**
===================================
==  filling the map with values  ==
===================================
*/
	std::ifstream file("some_file.txt");
	std::string line;
	std::vector<std::string> nodes_info;
	/**
	 key - address of an additional node, value - index of the node itself
	 example:
	 140317256854240/1/data=some_data1/140317256854384 - key = 140317256854384, value = 1
	*/
	std::map<unsigned long long, int> nodes_with_ptr;

	size_t lst_slash = 0;
	size_t fst_slash = 0;
	size_t num_len = 0;
	unsigned long long key = 0;
	size_t value = 0;

	/**
	 * За один цикл заполняется вектор со всей информацией списка
	 * и map, в котором хранятся значения дополнительных указателей(ссылки) и
	 * индексы нод, которые это дополнение хранят
	 * */
	while (true)
	{
		file >> line;
		if (file.eof() || line.empty())
			break;
		if (line.find("nullptr") == std::string::npos)
		{
			lst_slash = line.find_last_of("/");
			key = atol(line.substr(lst_slash + 1).c_str());
			fst_slash = line.find("/");
			// длина числа, вычисляемая путем нахождения индекса второго слеша с позиции первого
			num_len = std::string(&line[fst_slash + 1]).find("/");
			value = atoi(line.substr(fst_slash + 1, num_len).c_str());
			nodes_with_ptr.emplace(key, value);
		}
		nodes_info.push_back(line);
	}

	size_t node_ind = 0;
	std::string node_info;

	/**
	 * проходимся по структуре с информацией о звеньях с дополнительными указателями
	 * second - индекс ноды которая ссылается на звено под адресом first
	 * */

	unsigned long long addr_from = 0;
	unsigned long long addr_to = 0;
	ListNode *node_with_ptr = nullptr;
	std::cout << "SIZE: " << nodes_dict.size() << std::endl;
	for (auto it = nodes_with_ptr.begin(); it != nodes_with_ptr.end(); ++it)
	{
		addr_to = it->first;
		node_ind = it->second; // индекс ноды которая ссылается на звено под адресом first
		//TODO: может быть проблема (индексы нод и положение в списке противоположны)
		// при размере 5: 0 == 4, 1 == 3
		node_info = nodes_info[nodes_info.size() - node_ind - 1]; // получаем информацию о звене под нужным индексом
//		std::cout << node_info << std::endl;
		addr_from = atol(node_info.c_str()); // получаем числовой адрес звена
		std::cout << "From:" << addr_from << " To: " << addr_to << std::endl;
		node_with_ptr = nodes_dict.at(addr_from); // получаем указатель на это звено
		node_with_ptr->m_rand = nodes_dict.at(addr_to);
////		std::cout << node_info << std::endl;
	}
	std::cout << "SIZE: " << nodes_dict.size() << std::endl;

	return (new_data);
}

/** Creating a linked list with size N */
void create_list(ListNode **tmp_ptr, ListNode **main_obj, const size_t &N)
{
	std::string node_data = "some_data";

	for (size_t i = 0; i < N; i++)
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
	Serializator serializator = Serializator();
	const std::string file_name = "some_file.txt";
	if (!serializator.serialize_list(main_object, file_name))
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



	std::cout << "Its attributes: " << std::endl;
	// TODO: удалить
	if (deserialized_obj->m_prev == nullptr)
		std::cout << "Prev == nullptr\n";
	if (deserialized_obj->m_next == nullptr)
		std::cout << "Next == nullptr\n";


	while (deserialized_obj->m_prev)
	{
		std::cout << deserialized_obj->m_data;
		if (deserialized_obj->m_rand)
			std::cout << " " << deserialized_obj->m_rand->m_data;

		std::cout << std::endl;
		deserialized_obj = deserialized_obj->m_prev;
	}
	std::cout << deserialized_obj->m_data << std::endl;
	std::cout << "==================" << std::endl;
	while (deserialized_obj->m_next)
	{
		std::cout << deserialized_obj->m_data;
		if (deserialized_obj->m_rand)
			std::cout << " " << deserialized_obj->m_rand->m_data;

		std::cout << std::endl;
		deserialized_obj = deserialized_obj->m_next;
	}
	std::cout << deserialized_obj->m_data;
	if (deserialized_obj->m_rand)
		std::cout << " " << deserialized_obj->m_rand->m_data;
	std::cout << std::endl;
// TODO: сделать удаление всего списка
//  delete main_object;
//	 delete deserialized_obj;
}