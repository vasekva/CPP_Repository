#include <iostream>

#include <random>

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
	std::cout << deserialized_obj->m_data;
	if (deserialized_obj->m_rand)
		std::cout << " " << deserialized_obj->m_rand->m_data;
	std::cout << std::endl;
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


	const int LST_SIZE = 2;
	/** Creating a linkedList with N nodes */
	create_list(&tmp_ptr, &main_object, LST_SIZE);

	/** Creating additional pointers to another Nodes for other ones */
	make_random_refs(&main_object, LST_SIZE);

/**
=========================================
==  Serialization and  deserialization ==
=========================================
*/
	const std::string file_name = "some_file.txt";

	/** Serialization */
	Serializator serializator = Serializator();
	std::ofstream out_file(file_name);
	if (!out_file)
	{
		puts("Couldn't open the file");
		return (false);
	}
	if (!serializator.serialize_list(main_object, out_file))
	{
		std::cerr << "serialize error!" << std::endl;
		exit(-1);
	}
	out_file.close();

	/** Deserialization */
	std::ifstream in_file(file_name.c_str());
	if (!in_file)
	{
		puts("Couldn't open the file");
		return (false);
	}
	Deserializator deserializator = Deserializator();
	deserialized_obj = deserializator.deserialize_list(in_file);
	if (deserialized_obj == nullptr)
	{
		std::cerr << "deserialize error!" << std::endl;
		exit(1);
	}

	in_file.close();

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