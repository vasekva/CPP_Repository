#ifndef TOOLS_HPP
#define TOOLS_HPP

#include "main_header.hpp"
#include <set>

std::set<std::string>		convert_to_set(std::vector<std::string> &v);
std::vector<std::string>	get_column(const std::vector<std::vector<std::string> > &bd_data, int ind);
std::vector<std::string>	get_user_column(const std::vector<std::vector<std::string>> &bd_data, std::string UUID, int column);
std::vector<int>			get_sequence_frame(const std::vector<std::vector<std::string>> &bd_data, const std::string &UUID);
std::vector<int> 			get_sequence_frame_by_time(const std::vector<std::vector<std::string>> &bd_data, const std::string &UUID, int time);
float						get_average_x_by_time(const std::vector<std::vector<std::string>> &bd_data, const std::string &UUID, int time);
#endif
