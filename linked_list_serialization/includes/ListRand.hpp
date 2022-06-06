#ifndef LISTRAND_HPP
#define LISTRAND_HPP

#include <string>
#include <fstream>

#include "Deserializator.hpp"
#include "Serializator.hpp"
#include "Colors.hpp"

class Serializator;

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

class ListRand
{
	public:
		ListNode *Head;
		ListNode *Tail;
		int Count;

		ListRand(ListNode *const fst_ptr, ListNode *const lst_ptr);
		~ListRand();

		void Serialize(std::ofstream &out_file);
		void Deserialize(std::ifstream &in_file);
		void clear_list(void);
	private:
		ListRand();
};

#endif
