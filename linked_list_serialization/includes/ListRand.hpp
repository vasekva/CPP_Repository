#ifndef LISTRAND_HPP
#define LISTRAND_HPP

#include <string>
#include <fstream>

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

		void Serialize(std::ofstream &out_file)
		{

		}

		void Deserialize(std::ifstream &in_file)
		{

		}
};

#endif
