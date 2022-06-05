#ifndef DESERIALIZATOR_HPP
#define DESERIALIZATOR_HPP

#include "ListRand.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

#include <map>
//#include <vector>
#include <deque>

class Deserializator
{
	public:
		Deserializator();
		~Deserializator();
		ListNode *deserialize_list(void);
	private:
		bool		add_new_item_to_nodes_info(const std::map<std::string, std::string> &nodes_info, const std::string &line);
		std::string get_node_data_from_line(const std::string &line);
		std::string hex_to_str(const ListNode *const ptr);
		void recursion_deserialize(ListNode **curr, ListNode **next,
			std::map<unsigned long long, ListNode *> &nodes_map,
			std::map<std::string, std::string> &nodes_info, std::ifstream &in_file);
		void make_pointers(const std::string &file_name,
			const std::map<unsigned long long, ListNode *> &addrs_with_nodes,
			const std::map<std::string, std::string> &nodes_info);

};


#endif
