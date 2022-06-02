/**
	Given a string s containing just the characters '(', ')', '{', '}', '[' and ']',
 	determine if the input string is valid.

	An input string is valid if:

	Open brackets must be closed by the same type of brackets.
	Open brackets must be closed in the correct order.


	Example 1:
	Input: s = "()"
	Output: true

	Example 2:
	Input: s = "()[]{}"
	Output: true

	Example 3:
	Input: s = "(]"
	Output: false
*/

class Solution
{
	public:
		bool isValid(string s)
		{
			if (s.size() % 2 != 0 || s.size() == 0)
				return (false);

			std::stack<char> stack_prnts;
			for (char c : s)
			{
				if (c == '(' || c == '[' || c == '{')
					stack_prnts.emplace(c);
				else if (stack_prnts.empty())
					return (false);
				else
				{
					switch (c)
					{
						case ')':
							if (stack_prnts.top() != '(') return (false);
							else stack_prnts.pop(); break;
						case ']':
							if (stack_prnts.top() != '[') return (false);
							else stack_prnts.pop(); break;
						case '}':
							if (stack_prnts.top() != '{') return (false);
							else stack_prnts.pop(); break;
					}
				}
			}
			if (stack_prnts.empty())
				return (true);
			else
				return (false);
		}
};