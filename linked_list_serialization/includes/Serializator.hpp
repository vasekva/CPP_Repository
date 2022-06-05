#ifndef SERIALIZATOR_HPP
#define SERIALIZATOR_HPP

#include "ListRand.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

class ListNode;

class Serializator
{
	public:
		Serializator();
		~Serializator();
		bool serialize_list(ListNode *const ptr, std::ofstream &out_file);
	private:
		std::string hex_to_str(const ListNode *const ptr);
};

#endif
