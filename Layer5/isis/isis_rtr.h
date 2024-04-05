#include <stdbool.h>
#ifndef __ISIS_RTR__
#define __ISIS_RTR__

#define ISIS_NODE_INFO(node_ptr) ((isis_node_info_t *)(node_ptr->node_nw_prop.isis_node_info))

typedef struct isis_node_info_{
 // ...

} isis_node_info_t;


bool
isis_is_protocol_enable_on_node(node_t *node);

void
isis_init(node_t *node);

void
isis_de_init(node_t *node);

void 
isis_show_node_protocol_state(node_t *node);

#endif


