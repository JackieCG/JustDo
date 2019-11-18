/**
* Definition for singly-linked list.
* struct ListNode {
*     int val;
*     struct ListNode *next;
* };
*/


struct ListNode* reverseList(struct ListNode* head)
{
	struct ListNode *prev, *cur, *next;

	prev = NULL;
	cur = head;
	while (cur != NULL)
	{
		next = cur->next;
		cur->next = prev;
		prev = cur;
		cur = next;
	}

	return prev;
}

bool hasCycle(struct ListNode *head)
{
	struct ListNode *cur, *iter;
	int pos = -1;

	if (head == NULL || head->next == NULL)
		return false;

	cur = head;
	while (cur != NULL)
	{
		for (pos = 0, iter = head; iter != cur && iter != cur->next; iter = iter->next, pos++);

		if (iter == cur->next)
			break;
		else
			pos = -1;

		cur = cur->next;
	}

	if (pos == -1)
		return false;
	else
		return true;
}

struct ListNode* mergeTwoLists(struct ListNode* l1, struct ListNode* l2)
{
	struct ListNode *head = NULL;
	struct ListNode *cur = NULL;
	struct ListNode *next = NULL;

	while (l1 != NULL || l2 != NULL)
	{
		if (l1 == NULL)
		{
			next = l2;
			l2 = NULL;
		}
		else if (l2 == NULL)
		{
			next = l1;
			l1 = NULL;
		}
		else if (l1->val <= l2->val)
		{
			next = l1;
			l1 = l1->next;
		}
		else
		{
			next = l2;
			l2 = l2->next;
		}

		if (head == NULL)
		{
			head = cur = next;
		}
		else
		{
			cur->next = next;
			cur = cur->next;
		}
	}

	return head;
}

struct ListNode* mergeTwoLists2(struct ListNode* l1, struct ListNode* l2)
{
	struct ListNode guard = { 0 };
	struct ListNode *prev = &guard;

	while (l1 != NULL && l2 != NULL)
	{
		if (l1->val <= l2->val)
		{
			prev->next = l1;
			l1 = l1->next;
		}
		else
		{
			prev->next = l2;
			l2 = l2->next;
		}

		prev = prev->next;
	}

	prev->next = l1 != NULL ? l1 : l2;

	return guard.next;
}

struct ListNode* removeNthFromEnd(struct ListNode* head, int n)
{
	if (head == NULL)
		return NULL;

	struct ListNode *cur = head;
	struct ListNode *prev_n = head;
	int pos = 0;
	
	while (cur != NULL)
	{
		pos++;
		if (pos > n + 1)
			prev_n = prev_n->next;

		cur = cur->next;
	}

	if (pos >= n + 1)
	{
		prev_n->next = prev_n->next->next;
	}
	else if (pos == n)
	{
		head = head->next;
	}

	return head;
}

struct ListNode* removeNthFromEnd2(struct ListNode* head, int n)
{
	struct ListNode dummy = { 0 };
	struct ListNode *prev_n, *cur;

	dummy.next = head;
	prev_n = cur = &dummy;

	for (int i = 0; i <= n; i++)
	{
		cur = cur->next;
	}

	while (cur)
	{
		prev_n = prev_n->next;
		cur = cur->next;
	}

	prev_n->next = prev_n->next->next;

	return dummy.next;
}

struct ListNode* middleNode(struct ListNode* head)
{
	struct ListNode *slow, *fast;
	struct ListNode dummy = { 0, head };

	slow = fast = &dummy;

	while (fast != NULL)
	{
		slow = slow->next;
		
		fast = fast->next;
		if (fast != NULL)
		{
			fast = fast->next;
		}
	}

	return slow;
}