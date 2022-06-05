#ifndef DESERIALIZATOR_HPP
#define DESERIALIZATOR_HPP

#include "ListRand.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

#include <map>
//#include <vector>
#include <deque>

class ListNode;

class Deserializator
{
	public:
		Deserializator();
		~Deserializator();
		ListNode *deserialize_list(std::ifstream &in_file, ListNode *tail_ptr, int &count);
	private:
		bool		add_new_item_to_nodes_info(std::map<std::string, std::string> &nodes_info, const std::string &line);
		std::string get_node_data_from_line(const std::string &line);
		std::string hex_to_str(const ListNode *const ptr);
		void deserialize(ListNode **curr,
			ListNode *tail_ptr,
			std::map<unsigned long long, ListNode *> &nodes_map,
			std::map<std::string, std::string> &nodes_info,
			std::ifstream &in_file, int &count);
		void make_pointers(
			const std::map<unsigned long long, ListNode *> &addrs_with_nodes,
			const std::map<std::string, std::string> &nodes_info);

};


#endif
