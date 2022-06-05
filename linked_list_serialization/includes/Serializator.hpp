#ifndef SERIALIZATOR_HPP
#define SERIALIZATOR_HPP

#include "main_libs.hpp"

class ListNode;

class Serializator
{
	public:
		Serializator();
		~Serializator();
		bool serialize_list(const ListNode *const ptr, const std::string &file_name);
	private:
		void recursion_serialize(const ListNode *const ptr, size_t &ind, std::ofstream &out_file);
		unsigned long long hex_to_digit(const ListNode *const ptr);
};

#endif
