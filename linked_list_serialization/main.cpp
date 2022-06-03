#include "Data.hpp"

#include "iostream"
#include "fstream"
#include "sstream"

void serialize(Data *ptr, std::ofstream &out_file)
{
	std::stringstream str_stream;
	std::string str;

	out_file << ptr->getName();
	out_file.close();
}

Data *deserialize(std::ifstream &in_file)
{
	Data *new_data = nullptr;
	std::string name;

	in_file >> name;
	new_data = new Data(name);

	in_file.close();
	return (new_data);
}

int main(void)
{
	Data		*main_object;
	Data		*deserialized_obj;

	/**
	 * Creating an object
	 */
	main_object = new Data("Petr");
	std::cout << "First addr: " << main_object << std::endl;

	/**
	 * Opening a file for writing to it
	 */
	std::ofstream out_file("some_file.txt");
	if (!out_file)
	{
		puts("Couldn't open the file");
		return (-1);
	}
	serialize(main_object, out_file);

	/**
	 * Opening a file for reading from it
	 */
	std::ifstream in_file("some_file.txt");
	if (!in_file)
	{
		puts("Couldn't open the file");
		return (-1);
	}
	deserialized_obj = deserialize(in_file);

	/**
	 * Checking and printing deserialized data
	 */
	if (deserialized_obj == nullptr)
	{
		std::cout << "ERROR" << std::endl;
		exit(-1);
	}

	std::cout << "Addr after serialization: " << deserialized_obj << std::endl;
	std::cout << "Its attributes: " << std::endl;

	std::cout << deserialized_obj->getName() << std::endl;

	delete main_object;
	delete deserialized_obj;
}