#include <iostream>

//#include <time.h>
#include <random>

//#include <map>

#include "ListRand.hpp"
#include "Serializator.hpp"
#include "Deserializator.hpp"

#define F_NONE	"\033[37m"
#define RED		"\033[31m"
#define GREEN	"\033[32m"
#define CYANE	"\033[36m"
#define WHITE	"\033[1m"
#define YELLOW	"\033[0;33m"
#define BLUE	"\033[34m"
#define PURPLE  "\033[0;35m"
#define NORM	"\033[0m"

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

/** Creating a linked list with size N */
static void create_list(ListNode **tmp_ptr, ListNode **main_obj, const size_t &N)
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

static void list_print(ListNode	*deserialized_obj)
{
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
//	std::cout << "==================" << std::endl;
//	while (deserialized_obj->m_next)
//	{
//		std::cout << deserialized_obj->m_data;
//		if (deserialized_obj->m_rand)
//			std::cout << " " << deserialized_obj->m_rand->m_data;
//
//		std::cout << std::endl;
//		deserialized_obj = deserialized_obj->m_next;
//	}
//	std::cout << deserialized_obj->m_data;
//	if (deserialized_obj->m_rand)
//		std::cout << " " << deserialized_obj->m_rand->m_data;
//	std::cout << std::endl;
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

static size_t get_random_value(const int MAX)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> dist(0, MAX);

//	for (int i = 0; i < 16; ++i)
//		std::cout << dist(mt) << "\n";

	return (dist(mt));
}

static void make_random_refs(ListNode **list, const int LST_SIZE)
{
	size_t from = 0;
	size_t to = 0;
	size_t created_pointers = 0;

	std::map<int, int> refs;
	for (int size_t = 0; size_t < LST_SIZE / 2; size_t++)
	{
		from = get_random_value(LST_SIZE);
		to = get_random_value(LST_SIZE);
		try
		{
			refs.at(from);
		}
		catch (...)
		{
			if (make_ptr(*list, from, to))
				std::cout << "Created an addition pointer from " <<
						  from << " to " << to << std::endl;
			refs.emplace(from, to);
			created_pointers++;
		}
	}
	std::cout << GREEN"New created pointers: " NORM << created_pointers << std::endl;
	refs.clear();
}

int main(void)
{
	ListNode	*main_object = nullptr;
	ListNode	*deserialized_obj = nullptr;
	ListNode	*tmp_ptr = nullptr;


	const int LST_SIZE = 25;
	/** Creating a linkedList with N nodes */
	create_list(&tmp_ptr, &main_object, LST_SIZE);

	/** Creating additional pointers to another Nodes for other ones */
	make_random_refs(&main_object, LST_SIZE);

/**
=========================================
==  Serialization and  deserialization ==
=========================================
*/

	/** Serialization */
	Serializator serializator = Serializator();
	const std::string file_name = "some_file.txt";
	if (!serializator.serialize_list(main_object, file_name))
	{
		std::cerr << "serialize error!" << std::endl;
		exit(-1);
	}

	/** Deserialization */
	Deserializator deserializator = Deserializator();
	deserialized_obj = deserializator.deserialize_list();
	if (deserialized_obj == nullptr)
	{
		std::cerr << "deserialize error!" << std::endl;
		exit(1);
	}

/**
================
==  Printing  ==
================
*/
	list_print(deserialized_obj);
// TODO: сделать удаление всего списка
//  delete main_object;
//	 delete deserialized_obj;
}