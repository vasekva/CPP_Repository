#ifndef TOOLS_HPP
#define TOOLS_HPP

#include "main_header.hpp"
#include <set>

typedef const std::vector<std::vector<std::string>> db_info;

std::set<std::string>		convert_to_set(std::vector<std::string> &v);
std::vector<std::string>	get_column(db_info &bd_data, int column);
std::vector<std::string>	get_user_column(db_info &bd_data, std::string UUID, int column);
std::vector<int>			get_sequence_frame(db_info &bd_data, const std::string &UUID);
std::vector<int> 			get_sequence_frame_by_time(db_info &bd_data, const std::string &UUID, int time);
float						get_average_x_by_time(db_info &bd_data, const std::string &UUID, int time);
int							get_y_fraction_part_sum_by_time(db_info &bd_data, const std::string &UUID, int time);

#endif
