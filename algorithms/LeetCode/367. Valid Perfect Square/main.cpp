/**
	Given a positive integer num, write a function which returns
 	True if num is a perfect square else False.
	Follow up: Do not use any built-in library function such as sqrt.

	Example 1:
	Input: num = 16
	Output: true

	Example 2:
	Input: num = 14
	Output: false
*/

class Solution
{
	public:
		bool is_perfect_square(int num)
		{
			size_t low = 1, high = num, mid;

			while (low <= high)
			{
				mid = (high + low) / 2;

				if (mid * mid == num)
					return (true);
				else if (mid * mid > num)
					high = mid - 1;
				else
					low = mid + 1;
			}
			return (false);
		}
};