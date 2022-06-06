#include <iostream>

#include <random>

#include "ListRand.hpp"
#include "Serializator.hpp"
#include "Deserializator.hpp"

/** Creating a linked list with size N */
static void create_list(ListNode **main_obj, ListNode **tail, const size_t &N)
{
	ListNode *tmp_ptr;
	std::string node_data = "some_data";

	for (size_t i = 0; i < N; i++)
	{
		if (i == 0)
		{
			tmp_ptr = new ListNode(node_data + (std::to_string(i)));
			*main_obj = tmp_ptr;
		}
		else
		{
			//TODO: проверить корректность создания двусвязного списка
			tmp_ptr->m_next = new ListNode(node_data + std::to_string(i));
			tmp_ptr->m_next->m_prev = tmp_ptr;
			tmp_ptr = tmp_ptr->m_next;
		}
	}
	(*tail) = tmp_ptr;
}

/**
 * Creating a new pointer from
 * a node № ind_from to
 * a node № ind_to
 * */
static bool	make_ptr(ListNode *list, const size_t &ind_from, const size_t &ind_to)
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
			{
				std::cout <<
					GREEN << "Created an addition pointer" << NORM  << " from id " <<
		   			YELLOW << from << NORM << " to id " <<
					PURPLE << to << NORM << std::endl;
				refs.emplace(from, to);
				created_pointers++;
			}
		}
	}
	std::cout << CYANE"Number of created pointers: " NORM << created_pointers << std::endl;
	refs.clear();
}

static bool make_serialization(ListNode *head, ListNode *tail, const std::string &file_name)
{
	std::ofstream out_file(file_name);
	if (!out_file)
	{
		puts("Couldn't open the file");
		return (false);
	}

	ListRand list_rand = ListRand(head, tail);
	list_rand.Serialize(out_file);
	out_file.close();
	return (true);
}

static bool make_deserialization(ListNode *head, ListNode *tail, const std::string &file_name)
{
	std::ifstream in_file(file_name.c_str());
	if (!in_file)
	{
		puts("Couldn't open the file");
		return (false);
	}

	ListRand list_rand = ListRand(head, tail);
	list_rand.Deserialize(in_file);
	in_file.close();
	return (true);
}

int main(void)
{
	ListNode 	*head = nullptr;
	ListNode 	*tail = nullptr;

	// размер списка
	const int LST_SIZE = 13;
	/** Creating a linkedList with N nodes */
	create_list(&head, &tail, LST_SIZE);

	/** Creating additional pointers to another Nodes for other ones */
	make_random_refs(&head, LST_SIZE);

	const std::string file_name = "some_file.txt";

	/** Serialization */
	make_serialization(head, tail, file_name);

	head = nullptr;
	tail = nullptr;
	/** Deserialization */
	make_deserialization(head, tail, file_name);
}