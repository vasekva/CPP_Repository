#include "includes/Serializator.hpp"
#include "includes/ListRand.hpp"

Serializator::Serializator() {}
Serializator::~Serializator() {}

unsigned long long Serializator::hex_to_digit(const ListNode *const ptr)
{
	std::stringstream str_stream;
	unsigned long long digit = 0;

	str_stream << std::hex << ptr; // from ptr to hex to binary stream
	str_stream >> digit; // from binary to u_longlong

	return (digit);
}

void Serializator::recursion_serialize(const ListNode *const ptr, size_t &ind, std::ofstream &out_file)
{
	std::stringstream	str_stream;
	std::string 		str;

	if (ptr == nullptr)
		return;

	/** '/' - param delimeter */
	str_stream << hex_to_digit(ptr) << "/"; // from het to u_longlong and to serialization stream
	str_stream << ind++ << "/";
	str_stream << "data=" << ptr->m_data << "/";

	if (ptr->m_rand)
	{
		str_stream << hex_to_digit(ptr->m_rand);
		str_stream >> str;
	}
	else
	{
		str_stream << "nullptr";
	}
	str_stream >> str;

	recursion_serialize(ptr->m_next, ind, out_file); // recursion

	str.append("\n"); // data delimeter
	out_file << str;
}

bool Serializator::serialize_list(const ListNode *const ptr, std::ofstream &out_file)
{
	size_t ind = 0;

	recursion_serialize(ptr, ind, out_file); // рекурсивная сериализация списка
	return (true);
}