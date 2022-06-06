#include "includes/Serializator.hpp"
#include "includes/ListRand.hpp"

Serializator::Serializator() {}
Serializator::~Serializator() {}

std::string Serializator::hex_to_str(const ListNode *const ptr)
{
	size_t				digit_addr;
	std::stringstream	str_stream;

	str_stream << std::hex << ptr; // from ptr to hex to binary stream
	str_stream >> digit_addr; // from binary to u_longlong

	return (std::to_string(digit_addr));
}

bool Serializator::serialize_list(ListNode *const ptr, std::ofstream &out_file)
{
	size_t ind = 0;
	ListNode *tmp_ptr = ptr;

	std::string line;
	while (tmp_ptr)
	{
		/** '/' - param delimeter */
		line = hex_to_str(tmp_ptr);
		line += "/";
		line += std::to_string(ind++);
		line += "/";
		line += "data=" + tmp_ptr->m_data;
		line += "/";

		if (tmp_ptr->m_rand)
		{
			line += hex_to_str(tmp_ptr->m_rand);
		}
		else
		{
			line += "nullptr";
		}
		line.append("\n"); // data delimeter
		out_file << line;
		tmp_ptr = tmp_ptr->m_next;
	}
	return (true);
}