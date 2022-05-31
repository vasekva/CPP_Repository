/**
	Write a function to find the longest common prefix string amongst an array of strings.

	If there is no common prefix, return an empty string "".
	Example 1:

	Input: strs = ["flower","flow","flight"]
	Output: "fl"
	Example 2:

	Input: strs = ["dog","racecar","car"]
	Output: ""
	Explanation: There is no common prefix among the input strings.
*/

class Solution
{
	public:
		string longestCommonPrefix(vector<string> &strs)
		{
			std::string result;

			char c = 0;
			for (int i = 0; i < strs[0].size(); i++)
			{
				c = strs[0][i];
				for (auto str : strs)
				{
					if (str[i] != c)
						return (result);
				}
				result += c;
			}
			return (result);
		}
};