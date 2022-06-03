#include "Data.hpp"

Data::Data(std::string name)
{
	this->_name = name;
}

Data::~Data() { }

Data::Data(const Data &copyObject)
{
	*this = copyObject;
}

Data &Data::operator=(const Data &object)
{
	if (this != &object)
	{
		this->_name = object.getName();
	}
	return (*this);
}

const std::string	&Data::getName() const { return (this->_name); }
