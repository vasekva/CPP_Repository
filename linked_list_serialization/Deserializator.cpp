#include "Deserializator.hpp"

Deserializator::Deserializator() {}
Deserializator::~Deserializator() {}

std::string Deserializator::hex_to_str(const ListNode *const ptr)
{
	size_t				digit_addr;
	std::stringstream	str_stream;
	std::string 		addr;

	str_stream << std::hex << ptr; // from ptr to hex to binary stream
	str_stream >> digit_addr; // from binary to u_longlong

	return (std::to_string(digit_addr));
}

std::string Deserializator::get_node_data_from_line(const std::string &line)
{
	std::string node_data;

	/** Searching for the start and end positions of a node_data line */
	size_t data_ind = line.find("data=");
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

bool Deserializator::add_new_item_to_nodes_info(const std::string &line)
{
	// TODO:  вынести в отдельный метод
	std::string addr_from;
	std::string addr_to;

	size_t slash_pos = line.find("/");
	if (slash_pos == std::string::npos)
	{
		std::cerr << "deserialize error: incorrect find return\n";
	}
	addr_from = line.substr(0, slash_pos);

	slash_pos = line.find_last_of("/");
	if (slash_pos == std::string::npos)
	{
		std::cerr << "deserialize error: incorrect find return\n";
	}
	addr_to = line.substr(slash_pos + 1);

	//TODO: удалить
	std::cout << addr_from << ":" << addr_to << std::endl;

	nodes_info.emplace(addr_from, addr_to);
}

void Deserializator::recursion_deserialize(ListNode **curr, ListNode **next,
	std::map<unsigned long long, ListNode *> &nodes_map,
	std::map<std::string, std::string> &nodes_info,
	std::ifstream &in_file)
{
	std::string line;
	std::string node_data;

	if (in_file.eof())
		return;

	in_file >> line;
	if (line.empty())
		return;

	node_data = get_node_data_from_line(line);

	/** deserialization */
	(*curr) = new ListNode(node_data);

	size_t slash_ind = line.find("/");  //TODO: check it if there's an error
	if (slash_ind == std::string::npos)
	{
		std::cerr << "Incorrect format of serialization!" << std::endl;
		std::cerr << "Couldn't find a slash after node data!" << std::endl;
		exit(1);
	}
	/**
	 * key - address of the node before serialization,
	 * value - ptr to the new deserialized node
	 * */
	nodes_map.emplace(atol(line.substr(0, slash_ind).c_str()), *curr);
	/** adds information to nodes_info about which address was referenced to which */
	add_new_item_to_nodes_info(nodes_info, line);

	if (next && (*next) != nullptr)
	{
		(*curr)->m_next = *next;
	}
	recursion_deserialize(&(*curr)->m_prev, curr, nodes_map, nodes_info, in_file);
}



void Deserializator::make_pointers(const std::string &file_name,
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
		//TODO: удалить
//		std::cout << "From: \033[34m" << it->first << "\033[0m" <<
//			" To: \033[0;33m" << it->second << "\033[0m" << std::endl;

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

ListNode *Deserializator::deserialize_list(void)
{
//	std::map<unsigned long long, std::pair<int, std::string>> nodes_info;
	ListNode *new_data = nullptr;
	/** структура данных с адресом предыдущего состояния
	в виде ключа и указателем на звено в значении */
	std::map<unsigned long long, ListNode *>	addrs_with_nodes;
	std::map<std::string, std::string>			nodes_info;

	const std::string file_name = "some_file.txt";
	std::ifstream in_file(file_name.c_str());
	if (!in_file)
	{
		puts("Couldn't open the file");
		return (nullptr);
	}
	recursion_deserialize(&new_data, nullptr, addrs_with_nodes, nodes_info, in_file);
	in_file.close();

	//TODO удалить

//	while (new_data) {
//		std::cout << new_data->m_data << std::endl;
//		new_data = new_data->m_prev;
//	}


//	for (auto it = addrs_with_nodes.begin(); it != addrs_with_nodes.end(); ++it)
//	{
//		std::cout << "\033[36m" << it->first << "\033[0m" <<
//				  "/" << it->second->m_data << std::endl;
//	}

	make_pointers(file_name, addrs_with_nodes, nodes_info);

	return (new_data);
}