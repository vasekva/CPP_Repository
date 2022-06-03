/**
	Let's call an array arr a mountain if the following properties hold:

	arr.length >= 3
	There exists some i with 0 < i < arr.length - 1 such that:
	arr[0] < arr[1] < ... arr[i-1] < arr[i]
	arr[i] > arr[i+1] > ... > arr[arr.length - 1]

	Given an integer array arr that is guaranteed to be a mountain, return any i such that
 	arr[0] < arr[1] < ... arr[i - 1] < arr[i] > arr[i + 1] > ... > arr[arr.length - 1].


	Example 1:
	Input: arr = [0,1,0]
	Output: 1

	Example 2:
	Input: arr = [0,2,1,0]
	Output: 1

	Example 3:
	Input: arr = [0,10,5,2]
	Output: 1
*/

class Solution
{
	public:
		int peak_index_in_mountain_array(vector<int> &arr)
		{
			int high = arr.size() - 1, low = 0, mid = (high + low) / 2;

			while (low <= high)
			{
				if (mid == 0)
				{
					if (arr[mid + 1] > arr[mid])
						++mid;
					break;
				}

				if (arr[mid] > arr[mid - 1] && arr[mid] > arr[mid + 1]) // 0 2 10 0 <- example (10 - our desired value)
					break;
				else if(arr[mid] < arr[mid + 1])
					low = mid + 1;
				else
					high = mid - 1;

				mid = (high + low) / 2;
			}
			return (mid);
		}
};