#include "Deserializator.hpp"

Deserializator::Deserializator() {}
Deserializator::~Deserializator() {}

void Deserializator::recursion_deserialize(ListNode **curr, ListNode **next,
	std::map<unsigned long long, ListNode *> &nodes_dict, std::ifstream &in_file)
{
	std::string line;
	std::string node_data;

	if (in_file.eof())
		return;

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

void Deserializator::make_pointers(const std::string &file_name,
	const std::map<unsigned long long, ListNode *> &addrs_with_nodes)
{
	std::ifstream file(file_name.c_str());
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
	std::cout << "map 'addrs_with_nodes' SIZE: " << addrs_with_nodes.size() << std::endl;
	for (auto it = nodes_with_ptr.begin(); it != nodes_with_ptr.end(); ++it)
	{
		addr_to = it->first;
		node_ind = it->second; // индекс ноды которая ссылается на звено под адресом first
		//TODO: может быть проблема (индексы нод и положение в списке противоположны)
		// при размере 5: 0 == 4, 1 == 3

		node_info = nodes_info[(nodes_info.size() - node_ind) - 1]; // получаем информацию о звене под нужным индексом
//		std::cout << node_info << std::endl;
		addr_from = atol(node_info.c_str()); // получаем числовой адрес звена
		std::cout << "From:" << addr_from << " To: " << addr_to << std::endl;
		node_with_ptr = addrs_with_nodes.at(addr_from); // получаем указатель на это звено
		node_with_ptr->m_rand = addrs_with_nodes.at(addr_to);
////		std::cout << node_info << std::endl;
	}
	std::cout << "map 'addrs_with_nodes' SIZE: " << addrs_with_nodes.size() << std::endl;
}


ListNode *Deserializator::deserialize_list(void)
{
//	std::map<unsigned long long, std::pair<int, std::string>> nodes_info;
	/** структура данных с адресом в виде ключа и указателем на звено в значении */
	std::map<unsigned long long, ListNode *> addrs_with_nodes;
	ListNode *new_data = nullptr;


	const std::string file_name = "some_file.txt";
	std::ifstream in_file(file_name.c_str());
	if (!in_file)
	{
		puts("Couldn't open the file");
		return (nullptr);
	}
	recursion_deserialize(&new_data, nullptr, addrs_with_nodes, in_file);
	in_file.close();

	make_pointers(file_name, addrs_with_nodes);

	return (new_data);
}