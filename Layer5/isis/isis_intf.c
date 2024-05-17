#include "../../tcp_public.h"
#include "isis_intf.h"
#include "isis_rtr.h"
#include "isis_const.h"
#include "isis_pkt.h"
#include "isis_adjacency.h"

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
	isis_init_isis_intf_info(intf);
	
	sprintf(tlb, "%s : protocol is enabled on interface\n", ISIS_CONFIG_TRACE);
	tcp_trace(intf->att_node,intf,tlb);

	
	if(isis_interface_qualify_to_send_hellos(intf)){
		
		isis_start_sending_hellos(intf);
	}

}

void
isis_disable_protocol_on_interface(interface_t *intf){
	
	isis_intf_info_t *intf_info = NULL;
	
	intf_info = ISIS_INTF_INFO(intf);
	
	if(!intf_info) return;
	
	isis_stop_sending_hellos(intf);
	
	/* delete adjacency */
	isis_delete_adjacency(intf_info->adjacency);
	
	free(intf_info);
	
	intf->intf_nw_props.isis_intf_info = NULL;
	
}

void isis_show_intf_protocol_state(interface_t *intf){
	printf("%s %s\n",intf->if_name, isis_node_intf_is_enabled(intf) ? "ENable" : "DIsable");

}

static void
isis_init_isis_intf_info(interface_t *intf){

	isis_intf_info_t *isis_intf_info = ISIS_INTF_INFO(intf);
	memset(isis_intf_info,0,sizeof(isis_intf_info_t));
	isis_intf_info->cost = ISIS_DEFAULT_INTF_COST;
	isis_intf_info->hello_interval = ISIS_DEFAULT_HELLO_INTERVAL;
	isis_intf_info->good_hello_pkt_recvd = 0;
	isis_intf_info->bad_hello_pkt_recvd = 0;
	isis_intf_info->hello_pkt_sent = 0;
	
}


static void
isis_transmit_hello(void *arg, uint32_t arg_size){
	
	if(!arg) return;
	//printf("Sending hello!\n");
	
	isis_timer_data_t *isis_timer_data = (isis_timer_data_t *)arg;
	
	node_t *node = isis_timer_data->node;
	interface_t *intf = isis_timer_data->intf;
	byte *hello_pkt = (byte *)isis_timer_data->data;
	uint32_t pkt_size = isis_timer_data->data_size;
	
	send_pkt_out(hello_pkt, pkt_size,intf);
	
	ISIS_INTF_INCREMENT_STATS(intf, hello_pkt_sent);
	//printf("hello sent!\n");
	
}

void
isis_start_sending_hellos(interface_t *intf){

	//node_t *node;
	//node = intf->att_node;
	uint32_t hello_pkt_size;
	
	assert(ISIS_INTF_HELLO_XMIT_TIMER(intf) == NULL);
	assert(isis_node_intf_is_enabled(intf));
	
	wheel_timer_t *wt = node_get_timer_instance(intf->att_node);
	
	/* calculating hello_pkt_size */
	
	uint32_t eth_hdr_payload_size = sizeof(isis_pkt_hdr_t) + 
		(TLV_OVERHEAD_SIZE * 6) + NODE_NAME_SIZE +
		4 +
		4 +
		4 +
		4 +
		4;
	
	hello_pkt_size = ETH_HDR_SIZE_EXCL_PAYLOAD +
			eth_hdr_payload_size;
	
	byte *hello_pkt = isis_prepare_hello_pkt(intf,&hello_pkt_size);
	
	isis_timer_data_t *isis_timer_data = calloc(1,sizeof(isis_timer_data_t));
	
	isis_timer_data->node = intf->att_node;
	isis_timer_data->intf = intf;
	isis_timer_data->data = (void *)hello_pkt;
	isis_timer_data->data_size = hello_pkt_size;
	
	
	ISIS_INTF_HELLO_XMIT_TIMER(intf) = timer_register_app_event(wt,
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


bool
isis_interface_qualify_to_send_hellos(interface_t *intf){
	
	bool isis_enabled = isis_node_intf_is_enabled(intf);
	bool if_up = IF_IS_UP(intf);
	bool if_l3  = IS_INTF_L3_MODE(intf);
	
	if(isis_enabled && if_up && if_l3){
		return true;
	} else {
		return false;
	}
	
}

void
isis_show_interface_protocol_state(interface_t *intf){
	
	isis_intf_info_t *intf_info = NULL;
	intf_info = ISIS_INTF_INFO(intf);
	
	PRINT_TABS(1);
	printf("hello interval: %d sec, Intf Cost: %d\n", intf_info->hello_interval, intf_info->cost);
	PRINT_TABS(1);
	printf("hello Transmission: %s\n", isis_node_intf_is_enabled(intf) ? "On" : "Off");
	
	PRINT_TABS(1);
	printf("Stats:\n");
	
	PRINT_TABS(2);
	printf("> good_hello_pkt_recvd: %u\n", intf_info->good_hello_pkt_recvd);
	PRINT_TABS(2);
	printf("> bad_hello_pkt_recvd: %u\n", intf_info->bad_hello_pkt_recvd);
	PRINT_TABS(2);
	printf("> hello_pkt_sent: %u\n", intf_info->hello_pkt_sent);
	
	
	
}


