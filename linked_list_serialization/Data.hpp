#ifndef DATA_HPP
#define DATA_HPP

#include <string>

class Data
{
	public:
		Data(std::string name);
		~Data();
		Data(const Data &copyObject);
		Data &operator=(const Data &object);
		const std::string &getName() const;
	private:
		std::string	_name;
};

#endif