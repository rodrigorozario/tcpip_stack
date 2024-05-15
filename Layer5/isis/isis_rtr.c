#include "../../tcp_public.h"
#include "isis_rtr.h"
#include "isis_pkt.h"
#include "isis_intf.h"
#include "isis_const.h"
#include "isis_adjacency.h"

bool
isis_is_protocol_enable_on_node(node_t *node){

	isis_node_info_t *isis_node_info = ISIS_NODE_INFO(node);

	if(!isis_node_info){
		return false;
	}else{
		return true;
	}

}

void
isis_init(node_t *node) {
	isis_node_info_t *isis_node_info = ISIS_NODE_INFO(node);
	
	if(isis_node_info) return;
	
	isis_node_info = calloc(1,sizeof(isis_node_info_t));
	
	node->node_nw_prop.isis_node_info = isis_node_info;
	
	tcp_stack_register_l2_pkt_trap_rule(node,isis_pkt_trap_rule,isis_pkt_receive);
}

void
isis_de_init(node_t *node){

	/*
	Check if there are interfaces where isis is enabled.
	Disable isis in all interfaces.
	*/
	interface_t *interface = NULL;
	ITERATE_NODE_INTERFACES_BEGIN(node,interface){
		isis_disable_protocol_on_interface(interface);
					
	}ITERATE_NODE_INTERFACES_END(node,interface);
	
	
	isis_node_info_t *isis_node_info = ISIS_NODE_INFO(node);
	
	if(!isis_node_info) return;
	
	free(isis_node_info);

	node->node_nw_prop.isis_node_info = NULL;
	
	tcp_stack_de_register_l2_pkt_trap_rule(node,isis_pkt_trap_rule,isis_pkt_receive);
}

void isis_show_node_protocol_state(node_t *node){
	printf("ISIS Protocol: %s\n", isis_is_protocol_enable_on_node(node) ? "Enable" : "Disable");
	
	interface_t *interface = NULL;
	ITERATE_NODE_INTERFACES_BEGIN(node,interface){
		printf("%s : %s", interface->if_name, isis_node_intf_is_enabled(interface) ? "Enable\n" : "Disable\n");
		isis_show_interface_protocol_state(interface);
		
		isis_intf_info_t *isis_intf_info = ISIS_INTF_INFO(interface);
		isis_adjacency_t *adjacency = isis_intf_info->adjacency;
		
		PRINT_TABS(1)
		printf("Adjacencies:\n");
		isis_show_adjacency(adjacency, 2);
					
	}ITERATE_NODE_INTERFACES_END(node,interface);

}

void
isis_one_time_registration(){
	
	nfc_register_for_pkt_tracing(ISIS_ETH_PKT_TYPE, isis_print_pkt);
}




