#include "../../tcp_public.h"
#include "isis_intf.h"
#include "isis_rtr.h"
#include "isis_const.h"
#include "isis_pkt.h"

bool
isis_node_intf_is_enabled(interface_t *intf){
	return !(intf->intf_nw_props.isis_intf_info == NULL);
}

void
isis_enable_protocol_on_interface(interface_t *intf){
	/*
	1. Enable the protocol on interface only when protocol is already enabled
		at node level, else throw an error.
	2. If protocol already enabled on interface, then do nothing.
	3. Enable the protocol on interface.
	*/
	
	isis_intf_info_t *intf_info = NULL;
	
	if(ISIS_NODE_INFO(intf->att_node) == NULL) {
		printf("Error: Enable Protocol on node first\n");
		return;
	}
	
	if(intf_info){
		return;
	}
	
	intf_info = calloc(1,sizeof(isis_intf_info_t));
	intf->intf_nw_props.isis_intf_info = intf_info;
	
	isis_intf_info_t *isis_intf_info = NULL;
	
	isis_intf_info = ISIS_INTF_INFO(intf);
	if(!isis_intf_info){
		isis_intf_info = calloc(1,sizeof(isis_intf_info_t));
		intf->intf_nw_props.isis_intf_info = isis_intf_info;
		isis_init_isis_intf_info(intf);
	}
	

}

void
isis_disable_protocol_on_interface(interface_t *intf){
	
	isis_intf_info_t *intf_info = NULL;
	
	intf_info = ISIS_INTF_INFO(intf);
	
	if(!intf_info) return;
	
	free(intf_info);
	
	intf->intf_nw_props.isis_intf_info = NULL;
	
}

void isis_show_intf_protocol_state(interface_t *intf){
	printf("%s %s\n",intf->if_name, isis_node_intf_is_enabled(intf) ? "Enable" : "Disable");

}

static void
isis_init_isis_intf_info(interface_t *intf){

	isis_intf_info_t *isis_intf_info = ISIS_INTF_INFO(intf);
	memset(isis_intf_info,0,sizeof(isis_intf_info_t));
	isis_intf_info->cost = ISIS_DEFAULT_INTF_COST;
	isis_intf_info->hello_interval = ISIS_DEFAULT_HELLO_INTERVAL;
}


static void
isis_transmit_hello(void *arg, uint32_t arg_size){
	
	if(!arg) return;
	
	isis_timer_data_t *isis_timer_data = (isis_timer_data_t *)arg;
	
	node_t *node = isis_timer_data->node;
	interface_t *intf = isis_timer_data->intf;
	byte *hello_pkt = (byte *)isis_timer_data->data;
	uint32_t pkt_size = isis_timer_data->data_size;
	
	send_pkt_out(hello_pkt, pkt_size,intf);
	
}

void
isis_start_sending_hellos(interface_t *intf){

	node_t *node;
	uint32_t hello_pkt_size;
	
	assert(ISIS_INTF_HELLO_XMIT_TIMER(intf) == NULL);
	assert(isis_node_intf_is_enabled(intf));
	
	wheel_timer_t *wt = node_get_timer_instance(intf->att_node);
	
	byte *hello_pkt = isis_prepare_hello_pkt(intf,&hello_pkt_size);
	
	isis_timer_data_t *isis_timer_data = calloc(1,sizeof(isis_timer_data_t));
	
	isis_timer_data->node = intf->att_node;
	isis_timer_data->intf = intf;
	isis_timer_data->data = (void *)hello_pkt;
	isis_timer_data->data_size = hello_pkt_size;
	
	timer_register_app_event(wt,
							 isis_transmit_hello,
							 (void *)isis_timer_data,
							 sizeof(isis_timer_data_t),
							 ISIS_INTF_HELLO_INTERVAL(intf) * 1000,
							 1);
	
}

void
isis_stop_sending_hellos(interface_t *intf){
	
	timer_event_handle *hello_xmit_timer = NULL;
	
	hello_xmit_timer = ISIS_INTF_HELLO_XMIT_TIMER(intf);
	
	if(!hello_xmit_timer) return;
	
	isis_timer_data_t *isis_timer_data = 
			(isis_timer_data_t *)wt_elem_get_and_set_app_data(hello_xmit_timer,0);
			
	tcp_ip_free_pkt_buffer(isis_timer_data->data,
						   isis_timer_data->data_size);
						   
	free(isis_timer_data);
	
	timer_de_register_app_event(hello_xmit_timer);
	
	ISIS_INTF_HELLO_XMIT_TIMER(intf) = NULL;
	
}



