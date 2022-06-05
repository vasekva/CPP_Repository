#ifndef DESERIALIZATOR_HPP
#define DESERIALIZATOR_HPP

#include "ListRand.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

#include <map>
#include <vector>

class Deserializator
{
	public:
		Deserializator();
		~Deserializator();
		ListNode *deserialize_list(void);
	private:
		void recursion_deserialize(ListNode **curr, ListNode **next,
		std::map<unsigned long long, ListNode *> &nodes_dict, std::ifstream &in_file);
		void make_pointers(const std::string &file_name,
			const std::map<unsigned long long, ListNode *> &addrs_with_nodes);

};


#endif
