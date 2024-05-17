#include <stdbool.h>
#ifndef __ISIS_RTR__
#define __ISIS_RTR__

#define ISIS_NODE_INFO(node_ptr) ((isis_node_info_t *)(node_ptr->node_nw_prop.isis_node_info))

#define ISIS_INCREMENT_NODE_STATS(node_ptr,field) \
		(ISIS_NODE_INFO(node_ptr))->field++;

#define ISIS_DECREMENT_NODE_STATS(node_ptr,field) \
		(ISIS_NODE_INFO(node_ptr))->field--;

typedef struct isis_node_info_{
 // ...
	uint16_t adj_up_count; 
} isis_node_info_t;

typedef struct isis_timer_data_ {

	node_t *node;
	interface_t *intf;
	void *data;
	size_t data_size;
} isis_timer_data_t;

bool
isis_is_protocol_enable_on_node(node_t *node);

void
isis_init(node_t *node);

void
isis_de_init(node_t *node);

void 
isis_show_node_protocol_state(node_t *node);

void
isis_show_node_protocol_stats(node_t *node);

#endif


