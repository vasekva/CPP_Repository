//#include "Data.hpp"

#include "iostream"
#include "fstream"
#include "sstream"

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

void recursion_serialize(ListNode *ptr, std::ofstream &out_file)
{
	std::stringstream str_stream;
	std::string str;

	if (ptr == nullptr)
		return;

	str_stream << ptr << "/"; // param delimeter
	str_stream << ptr->m_data << "/";
	if (ptr->m_rand)
	{
		str_stream << ptr->m_rand << "/";
		str_stream >> str;
	}
	else
	{
		str_stream << "nullptr/";
	}
	str_stream >> str;
	recursion_serialize(ptr->m_next, out_file); // recursion

	str.append("\n"); // data delimeter

	out_file << str;
}

bool serialize_obj(ListNode *ptr, const std::string &file_name)
{
	std::ofstream out_file("some_file.txt");
	if (!out_file)
	{
		puts("Couldn't open the file");
		return (false);
	}
	recursion_serialize(ptr, out_file); // рекурсивная сериализация списка
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

void create_list(ListNode **tmp_ptr, ListNode **main_obj, size_t N)
{
	std::string name = "Node_#";

	for (int i = 0; i < N; i++)
	{
		if (i == 0)
		{
			*tmp_ptr = new ListNode(name + (std::to_string(i)));
			*main_obj = *tmp_ptr;
		}
		else
		{
			(*tmp_ptr)->m_next = new ListNode(name + std::to_string(i));
			(*tmp_ptr) = (*tmp_ptr)->m_next;
		}
	}
}

int main(void)
{
	ListNode		*main_object;
	ListNode		*deserialized_obj;

	/**
	 * Creating a linkedList with N size
	 */
	ListNode *tmp_ptr;
	create_list(&tmp_ptr, &main_object, 11);

	/**
	 * Opening a file for writing to it
	 */
	const std::string file_name = "some_file.txt";
	serialize_obj(main_object, file_name);

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

	std::cout << "Addr after serialization: " << deserialized_obj << std::endl;
	std::cout << "Its attributes: " << std::endl;

	std::cout << deserialized_obj->m_data;


// TODO: сделать удаление всего списка
//  delete main_object;
//	 delete deserialized_obj;
}