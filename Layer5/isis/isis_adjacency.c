#include "../../tcp_public.h"
#include "isis_adjacency.h"
#include "isis_const.h"
#include "isis_intf.h"


void
isis_update_interface_adjacency_from_hello(interface_t *iif,byte *hello_tlv_buffer,size_t tlv_buff_size){
	
	//printf("beginning of isis_update_interfae_adjacency_from_hello\n");
	bool new_adj = false;
	bool nbr_attr_changed = false;
	uint32_t ip_addr_int;
	
	isis_intf_info_t *isis_intf_info = ISIS_INTF_INFO(iif);
	
	isis_adjacency_t *adjacency = isis_intf_info->adjacency;
	
	if(!adjacency){
		
		adjacency = calloc(1, sizeof(isis_adjacency_t));
		adjacency->intf = iif;
		new_adj = true;
		adjacency->adj_state = ISIS_ADJ_STATE_DOWN;
		isis_intf_info->adjacency = adjacency;
	}
	
	byte tlv_type, tlv_len, *tlv_value = NULL;
	
	//printf("iterate begin...\n");
	
	ITERATE_TLV_BEGIN(hello_tlv_buffer, tlv_type, tlv_len, tlv_value, tlv_buff_size){
		//printf("%d\n",tlv_type);
		switch(tlv_type){
			
			case ISIS_TLV_HOSTNAME:
				//printf("ISIS_TLV_HOSTNAME\n");
				if(memcmp(adjacency->nbr_name,tlv_value,tlv_len)){
					nbr_attr_changed = true;
					memcpy(adjacency->nbr_name, tlv_value, tlv_len);
				}
				break;
			case ISIS_TLV_RTR_ID:
				//printf("ISIS_TLV_RTR_ID\n");
				if(adjacency->nbr_rtr_id != *(uint32_t *)(tlv_value)){
					nbr_attr_changed = true;
					adjacency->nbr_rtr_id = *(uint32_t *)(tlv_value);
				}
				break;
			case ISIS_TLV_IF_IP:
				//printf("ISIS_TLV_IF_IP\n");
				memcpy((byte *)&ip_addr_int, tlv_value, sizeof(ip_addr_int));
				if(adjacency->nbr_intf_ip != ip_addr_int){
					nbr_attr_changed = true;
					adjacency->nbr_intf_ip = ip_addr_int;
				}
				break;
			case ISIS_TLV_IF_INDEX:
				//printf("ISIS_TLV_IF_INDEX\n");
				if(adjacency->remote_if_index != *(uint32_t *)tlv_value){
					nbr_attr_changed = true;
					memcpy((byte *)&adjacency->remote_if_index, tlv_value, tlv_len);
				}
				break;
			case ISIS_TLV_HOLD_TIME:
				//printf("ISIS_TLV_HOLD_TIME\n");
				if(adjacency->hold_time != *(uint32_t *)tlv_value){
					nbr_attr_changed = true;
					memcpy((byte *)&adjacency->hold_time, tlv_value, tlv_len);
				}
				break;
			case ISIS_TLV_METRIC_VAL:
				//printf("ISIS_TLV_METRIC_VAL\n");
				if(adjacency->cost != *((uint32_t *)tlv_value)){
					nbr_attr_changed = true;
					adjacency->cost = *((uint32_t *)tlv_value);
				}
				break;
			case ISIS_TLV_IF_MAC:
				//printf("ISIS_TLV_IF_MAC");
                if (memcmp(adjacency->nbr_mac.mac, (byte *)tlv_value, tlv_len)) {
                	nbr_attr_changed = true;
                    memcpy(adjacency->nbr_mac.mac, tlv_value, tlv_len);
                }
            	break;
			default:;
		}
	
	
	}ITERATE_TLV_END(hello_tlv_buffer, tlv_type, tlv_len, tlv_value, tlv_buff_size)
	
}


void
isis_show_adjacency(isis_adjacency_t *adjacency, uint8_t tab_spaces){
	
	//printf("isis_show_adjacency beggining...\n");
	
	char *if_ip;
	char *rtr_id;
	
	PRINT_TABS(tab_spaces);
	rtr_id = tcp_ip_covert_ip_n_to_p(adjacency->nbr_rtr_id, 0);
	printf("Nbr: %s (%s)\n", adjacency->nbr_name, rtr_id);
	
	PRINT_TABS(tab_spaces);
	if_ip = tcp_ip_covert_ip_n_to_p(adjacency->nbr_intf_ip, 0);
	printf("Nbr intf ip: %s ifindex: %d\n", if_ip, adjacency->remote_if_index);
	
	PRINT_TABS(tab_spaces);
	printf("State: %s HT: %d sec Cost: %d\n\n", isis_adj_state_str(adjacency->adj_state), adjacency->hold_time,adjacency->cost);
	
	
}