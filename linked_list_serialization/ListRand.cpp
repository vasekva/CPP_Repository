#include "ListRand.hpp"

void ListRand::clear_list(void)
{
	if (!Head)
		return ;

	while (Head->m_next)
	{
		Head = Head->m_next;
		if (Head->m_prev)
		{
			Head->m_prev->m_rand = nullptr;
			Head->m_prev->m_next = nullptr;
			delete Head->m_prev;
			Head->m_prev = nullptr;
		}
	}
	Head->m_rand = nullptr;
	delete Head;
	Head = nullptr;
	Tail = nullptr;
};

ListRand::ListRand(ListNode *const fst_ptr, ListNode *const lst_ptr)
	: Head(fst_ptr), Tail(lst_ptr)
{
	Count = 0;
}

ListRand::~ListRand()
{
	clear_list();
}

void ListRand::Serialize(std::ofstream &out_file)
{
	Serializator serializator = Serializator();
	if (!serializator.serialize_list(Head, out_file))
	{
		std::cerr << "serialize error!" << std::endl;
		out_file.close();
		// удаляю список, ибо программа выдала ошибку
		clear_list();
		exit(1);
	}
	else
		clear_list(); // удаляю список, предполагая, что сериализация происходит перед закрытием программы
}

void ListRand::Deserialize(std::ifstream &in_file)
{
	Deserializator deserializator = Deserializator();
	Head = deserializator.deserialize_list(in_file, &Tail, Count);
	if (Head == nullptr)
	{
		std::cerr << "deserialize error!" << std::endl;
		exit(1);
	}

	/** Вывод состояния звеньев проходя от хвоста к голове и
	 * от головы к хвосту, делая дополнительно
	 * вывод состояния m_rand, если он есть.
	 *
	 * + Выводится значение атрибута Count(размер десериализованного списка)
	 * */
//	ListNode *tail_ptr = Tail;
//	while (tail_ptr)
//	{
//		std::cout << YELLOW << "Node with data: " << NORM;
//		std::cout << tail_ptr->m_data;
//		if (tail_ptr->m_rand)
//		{
//			std::cout << PURPLE " refers to node with data: " << NORM
//				<< tail_ptr->m_rand->m_data;
//		}
//		std::cout << std::endl;
//		tail_ptr = tail_ptr->m_prev;
//	}
//
//	std::cout << "========================================" << std::endl;
//
//	ListNode *head_ptr = Head;
//	while (head_ptr)
//	{
//		std::cout << YELLOW << "Node with data: " << NORM;
//		std::cout << head_ptr->m_data;
//		if (head_ptr->m_rand)
//		{
//			std::cout << PURPLE " refers to node with data: " << NORM
//			  << head_ptr->m_rand->m_data;
//		}
//		std::cout << std::endl;
//		head_ptr = head_ptr->m_next;
//	}
//	std::cout << CYANE "Count: " NORM << Count << std::endl;
}