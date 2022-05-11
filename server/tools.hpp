#ifndef TOOLS_HPP
#define TOOLS_HPP

#include "main_header.hpp"
#include <set>

std::set<std::string>		convert_to_set(std::vector<std::string> &v);
std::vector<std::string>	get_column(std::vector<std::vector<std::string> > &bd_data, int ind);
std::vector<std::string>	get_user_column(std::vector<std::vector<std::string>> &bd_data, std::string UUID, int column);
std::vector<int>			get_sequence_frame(std::vector<std::vector<std::string>> &bd_data, std::string UUID);
#endif
