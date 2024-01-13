#include "container.h"
#include <stdio.h>
#include <stdlib.h>
#ifndef DEQUEUE
#define DEQUEUE

#define TYPE int

#ifndef TYPE
#error "Template argument missing"
#endif // !TYPE

struct FN_CREATE(dequeue, TYPE) {
	size_t size;
	size_t capacity;
	TYPE *buffer;
	size_t front;
	size_t back;
};

static void double_capacity(FN_CREATE(dequeue, TYPE) * buf)
{
	TYPE *tmp = (TYPE *)realloc(buf->buffer, 2 * buf->capacity + 1);
	BUFCHK(tmp);
	for (size_t i = buf->front + 1; i < buf->back + 1;
	     i = (i + 1) % buf->size) {
		tmp[i - buf->front] = buf->buffer[i];
	}
	buf->buffer = tmp;
	buf->capacity = 2 * (buf->capacity) + 1;
	buf->front = 0;
	buf->back = buf->size + 1;
};

static struct FN_CREATE(dequeue, TYPE) FN_CREATE(dequeue_create, TYPE)(void)
{
	struct FN_CREATE(dequeue, TYPE) tmp = {
	    .size = 0,
	    .capacity = 2 * 16 + 1,
	    .buffer = (TYPE *)malloc((2 * 16 + 1) * sizeof(TYPE)),
	    .front = 0,
	    .back = 1,
	};
	return tmp;
};

static void FN_CREATE(dequeue_push_back,
		      TYPE)(struct FN_CREATE(dequeue, TYPE) * buf, TYPE value)
{
	if (buf->size == buf->capacity) {
		double_capacity(buf);
	}
	buf->buffer[buf->back] = value;
	buf->back = (buf->back + 1) % buf->capacity;
	buf->size++;
};

static void FN_CREATE(dequeue_push_front,
		      TYPE)(struct FN_CREATE(dequeue, TYPE) * buf, TYPE value)
{
	if (buf->size == buf->capacity) {
		double_capacity(buf);
	}
	buf->buffer[buf->front] = value;
	buf->front = buf->front == 0 ? buf->capacity - 1 : buf->front - 1;
	buf->size++;
};

static TYPE FN_CREATE(dequeue_pop_front,
		      TYPE)(struct FN_CREATE(dequeue, TYPE) * buf)
{
	buf->front = (buf->front + 1) % buf->capacity;
	buf->size--;
	return buf->buffer[buf->front];
};
static TYPE FN_CREATE(dequeue_pop_back,
		      TYPE)(struct FN_CREATE(dequeue, TYPE) * buf)
{
	size_t retval = buf->back;
	buf->back = buf->back == 0 ? buf->capacity - 1 : buf->back - 1;
	buf->size--;
	return buf->buffer[retval];
};

static TYPE *FN_CREATE(dequeue_peek_front,
		       TYPE)(struct FN_CREATE(dequeue, TYPE) * buf)
{
	return buf->buffer + buf->front + 1;
}

static TYPE *FN_CREATE(dequeue_peek_back,
		       TYPE)(struct FN_CREATE(dequeue, TYPE) * buf)
{
	return buf->buffer + buf->back;
}

static inline void FN_CREATE(dequeue_free,
			     TYPE)(struct FN_CREATE(dequeue, TYPE) * q)
{
	free(q->buffer);
};

#undef TYPE
#endif // !DEQUEUE
