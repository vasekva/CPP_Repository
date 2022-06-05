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

bool Deserializator::add_new_item_to_nodes_info(std::map<std::string, std::string> &nodes_info, const std::string &line)
{
	// TODO:  вынести в отдельный метод
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

//	//TODO: удалить
//	std::cout << addr_from << ":" << addr_to << std::endl;

	nodes_info.emplace(addr_from, addr_to);
	return (true);
}

void Deserializator::deserialize(ListNode **curr,
	ListNode *tail_ptr,
	std::map<unsigned long long, ListNode *> &nodes_map,
	std::map<std::string, std::string> &nodes_info,
	std::ifstream &in_file,
	int &count)
{
	std::string line;
	std::string node_data;

	ListNode *curr_ptr;
	ListNode *tmp_ptr;

//	size_t size = 0;
	while (true)
	{
		in_file >> line;
		if (in_file.eof() || line.empty())
			return;

		node_data = get_node_data_from_line(line);

		/** deserialization */
		if (count == 0)
		{
			tmp_ptr = new ListNode(node_data);
			(*curr) = tmp_ptr;
		}
		else
		{
			curr_ptr = tmp_ptr;
			tmp_ptr->m_next = new ListNode(node_data);
			tmp_ptr = tmp_ptr->m_next;
			tmp_ptr->m_prev = curr_ptr;
		}
//		//TODO: удалить
//		std::cout << tmp_ptr->m_data << std::endl;

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
		nodes_map.emplace(atol(line.substr(0, slash_ind).c_str()), tmp_ptr);
		/** adds information to nodes_info about which address was referenced to which */
		if (!add_new_item_to_nodes_info(nodes_info, line))
		{
			std::cerr << "add_new_item_to_nodes_info error" << std::endl;
			exit(1);
		}

		count++;
	}
	tail_ptr = tmp_ptr;
}

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

ListNode *Deserializator::deserialize_list(std::ifstream &in_file, ListNode *tail_ptr, int &count)
{
	ListNode *new_data = nullptr;
	/** структура данных с адресом предыдущего состояния
	в виде ключа и указателем на звено в значении */
	std::map<unsigned long long, ListNode *>	addrs_with_nodes;
	std::map<std::string, std::string>			nodes_info;

	deserialize(&new_data, tail_ptr, addrs_with_nodes, nodes_info, in_file, count);

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

	make_pointers(addrs_with_nodes, nodes_info);

	return (new_data);
}