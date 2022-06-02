/**
	You are given the heads of two sorted linked lists list1 and list2.
	Merge the two lists in a one sorted list. The list should be made by splicing
 	together the nodes of the first two lists.

	Return the head of the merged linked list.

	Example 1:
	Input: list1 = [1,2,4], list2 = [1,3,4]
	Output: [1,1,2,3,4,4]

	Example 2:
	Input: list1 = [], list2 = []
	Output: []

	Example 3:
	Input: list1 = [], list2 = [0]
	Output: [0]
*/

/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution
{
public:
	void push_back(ListNode **list, int val)
	{
		if (!(*list))
		{
			*list = new ListNode(val);
		}
		else
		{
			(*list)->next = new ListNode(val);
			*list = (*list)->next;
		}
	}

	void init(ListNode **list, int val)
	{
	}

	ListNode* merge_two_lists(ListNode *list1, ListNode *list2)
	{
		ListNode *new_list = nullptr;
		ListNode *tmp = new_list;

		int count = 0;
		int value = 0;
		while (list1 || list2)
		{
			if (!list2 || (list1 && (list1->val < list2->val)))
			{
				value = list1->val;
				list1 = list1->next;
			}
			else if (list2)
			{
				value = list2->val;
				list2 = list2->next;
			}
			push_back(&new_list, value);

			++count;
			if (count == 1)
				tmp = new_list;
		}
		return (tmp);
	}
};