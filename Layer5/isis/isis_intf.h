#include <stdbool.h>
#ifndef __ISIS_INTF__
#define __ISIS_INTF__

typedef struct isis_intf_info_ {

	/* Interface cost */
	uint32_t cost;
	
	/* Helo time interval */
	uint32_t hello_interval;
	
	/* Transmission hello timer*/
	timer_event_handle *hello_xmit_timer;
	
	

} isis_intf_info_t;


/* MACROS */

#define ISIS_INTF_INFO(intf_ptr) ( \
	((isis_intf_info_t *)(intf_ptr->intf_nw_props.isis_intf_info)))
	
#define ISIS_INTF_HELLO_XMIT_TIMER(intf_ptr) ( \
	((isis_intf_info_t *)((intf_ptr)->intf_nw_props.isis_intf_info))->hello_xmit_timer)

bool
isis_node_intf_is_enable(interface_t *intf);

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


#endif
