#include "sched.h"
#include "binary_heap.h"

struct task {
	void (*entry)(void *ctx);
	void *ctx;
	int priority;
	int deadline;
	int id;
	int timeout;
};

static struct task taskpool[16];
static int taskpool_n;
binary_heap* heap;
struct task* active_task;
int (*compare) (const void*, const void*);

void sched_new(void (*entrypoint)(void *aspace),
		void *aspace,
		int priority,
		int deadline) {
	struct task *t = &taskpool[taskpool_n++];

	t->entry = entrypoint;
	t->ctx = aspace;
	t->priority = priority;
	t->deadline = deadline;
	t->id = taskpool_n;
	t->timeout = 0;
}

void sched_cont(void (*entrypoint)(void *aspace),
		void *aspace,
		int timeout) {
	int ind = -1;
	active_task->entry = entrypoint;
	active_task->ctx = aspace;
	active_task->timeout = timeout;
	binary_heap_insert(heap, active_task);
}

void sched_time_elapsed(unsigned amount) {
	struct task** active_tasks = binary_heap_get_array(heap);

	for (int i = 0; i < binary_heap_len(heap); i++)
		active_tasks[i]->timeout -= amount;

	while (((struct task*)binary_heap_view_top(heap))->timeout <= 0)
	{
		active_task = binary_heap_extract(heap);
		active_task->entry(active_task->ctx);
	}
}

int timeout_comp(const struct task* x, const struct task* y)
{
	return x->timeout - y->timeout;
}

int fifo_comp(const struct task* x, const struct task* y)
{
	int c = timeout_comp(x, y);
	if (c != 0)
		return c;
	else
		return y->id - x->id;
}

int prio_comp(const struct task* x, const struct task* y)
{
	int c = timeout_comp(x, y);
	if (c != 0)
		return c;
	else
		return x->priority - y->priority;
}

int deadline_comp(const struct task* x, const struct task* y)
{
	int c = timeout_comp(x, y);
	if (c != 0)
		return c;
	else
	{
		if (x->deadline < 0)
			if (y->deadline < 0)
				c = 0;
			else
				c = -1;
		else if (y->deadline < 0)
			c = 1;
		else
			c = y->deadline - x->deadline;

		if (c != 0)
			return c;
		else
			return prio_comp(x, y);
	}
}

void sched_set_policy(enum policy policy) {
	switch (policy)
	{
	case POLICY_FIFO:
		compare = fifo_comp;
		break;
	case POLICY_PRIO:
		compare = prio_comp;
		break;
	case POLICY_DEADLINE:
		compare = deadline_comp;
		break;
	default:
		break;
	}
}

void sched_run(void) {
	heap = binary_heap_init(taskpool_n, compare);
	for (int i = 0; i < taskpool_n; i++)
		binary_heap_insert(heap, &taskpool[i]);
	sched_time_elapsed(0);
}
