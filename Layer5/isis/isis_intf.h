#include <stdbool.h>
#ifndef __ISIS_INTF__
#define __ISIS_INTF__


typedef struct isis_adjacency_ isis_adjacency_t;
typedef struct isis_intf_info_ {

	/* Interface cost */
	uint32_t cost;
	
	/* Helo time interval */
	uint32_t hello_interval;
	
	/* Transmission hello timer*/
	timer_event_handle *hello_xmit_timer;
	
	/* Pointer to the adjacency object*/
	isis_adjacency_t *adjacency;
	
	/* stats */

    uint32_t good_hello_pkt_recvd;

    uint32_t bad_hello_pkt_recvd;

    uint32_t hello_pkt_sent;
	
	

} isis_intf_info_t;


/* MACROS */

#define ISIS_INTF_INFO(intf_ptr) ( \
	((isis_intf_info_t *)(intf_ptr->intf_nw_props.isis_intf_info)))
	
#define ISIS_INTF_HELLO_XMIT_TIMER(intf_ptr) ( \
	((isis_intf_info_t *)((intf_ptr)->intf_nw_props.isis_intf_info))->hello_xmit_timer)
	
#define ISIS_INTF_INCREMENT_STATS(intf_ptr,field) \
		(ISIS_INTF_INFO(intf_ptr))->field++;

#define ISIS_INTF_DECREMENT_STATS(intf_ptr,field) \
		(ISIS_INTF_INFO(intf_ptr))->field--;

bool
isis_node_intf_is_enabled(interface_t *intf);

void
isis_enable_protocol_on_interface(interface_t *intf);

void
isis_disable_protocol_on_interface(interface_t *intf);

static void
isis_init_isis_intf_info (interface_t *intf);

void
isis_start_sending_hellos(interface_t *intf);

void
isis_stop_sending_hellos(interface_t *intf);

bool
isis_interface_qualify_to_send_hellos(interface_t *intf);

void 
isis_show_intf_protocol_state(interface_t *intf);

void
isis_show_interface_protocol_state(interface_t *intf); 



#endif
