//#include "Data.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

#include <time.h>
#include <random>

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
	str_stream << ind++ << "/";
	str_stream << hex_to_digit(ptr) << "/"; // from het to u_longlong and to serialization stream
	str_stream << ptr->m_data << "/";

	if (ptr->m_rand)
	{
		str_stream << hex_to_digit(ptr->m_rand) << "/";
		str_stream >> str;
	}
	else
	{
		str_stream << "nullptr/";
	}
	str_stream >> str;

	recursion_serialize(ptr->m_next, ind, out_file); // recursion

	str.append("\n"); // data delimeter
	out_file << str;
}

bool serialize_obj(const ListNode *const ptr, const std::string &file_name)
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

ListNode *deserialize(std::ifstream &in_file)
{
	ListNode *new_data = nullptr;
	std::string name;

	in_file >> name;
	new_data = new ListNode(name);

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
			(*tmp_ptr)->m_next = new ListNode(node_data + std::to_string(i));
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


	/** Creating a linkedList with N size */
	const size_t list_size = 5;
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
	if (!serialize_obj(main_object, file_name))
	{
		std::cerr << "serialize error!" << std::endl;
		exit(-1);
	}



	/**
	 * Opening a file for reading from it
	 */
	std::ifstream in_file("some_file.txt");
	if (!in_file)
	{
		puts("Couldn't open the file");
		return (-1);
	}
	deserialized_obj = deserialize(in_file);
	in_file.close();

	/**
	 * Checking and printing deserialized data
	 */
	if (deserialized_obj == nullptr)
	{
		std::cout << "ERROR" << std::endl;
		exit(-1);
	}

	std::cout << "Its attributes: " << std::endl;

	std::cout << deserialized_obj->m_data;


// TODO: сделать удаление всего списка
//  delete main_object;
//	 delete deserialized_obj;
}