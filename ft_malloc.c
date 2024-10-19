
#include "minishell.h"

t_garbage	*new_garbage(void *allocation)
{
	t_garbage	*node;

    node = malloc(sizeof(t_garbage));
    if (!node)
        exit(1);
	node->data = allocation;
	node->is_free = false;
	node->next = NULL;
	return (node);
}

void	free_garbage(t_garbage *collector)
{
    t_garbage   *current;
    t_garbage   *next;

    current = collector;
	while (current)
	{
        next = current->next;
        if (!current->is_free)
            free(current->data);
        free(current);
        current = next;
	}
}

void *ft_ft_malloc(size_t size, char alloc)
{
	static  t_garbage	*collector;
	void				*allocation;

	allocation = NULL;
	if (alloc == 'a')
	{
		allocation = malloc(size);
		if (!allocation)
			exit(1);
		if (!collector)
			collector = new_garbage(allocation);
		else
			collector->next = new_garbage(allocation);
	}
	if (alloc == 'f')
		free_garbage(collector);
	return (allocation);
}
