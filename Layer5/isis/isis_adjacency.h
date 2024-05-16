#ifndef __IGP_NBRSHIP__
#define __IGP_NBRSHIP__

typedef enum isis_adj_state_ {
	ISIS_ADJ_STATE_UNKNOWN,
	ISIS_ADJ_STATE_DOWN,
	ISIS_ADJ_STATE_INIT,
	ISIS_ADJ_STATE_UP
} isis_adj_state_t;

static inline char *
isis_adj_state_str(isis_adj_state_t adj_state){
	
	switch(adj_state){
		case ISIS_ADJ_STATE_DOWN:
			return "Down";
		case ISIS_ADJ_STATE_INIT:
			return "Init";
		case ISIS_ADJ_STATE_UP:
			return "Up";
		default:;
	}
	return NULL;
}

typedef struct isis_adjacency_ {
	/* back prt to the interface */
	interface_t *intf;
	
	/* Nbr lo0 address*/
	uint32_t nbr_rtr_id;
	
	/* Nbr Device Name */
	unsigned char nbr_name[NODE_NAME_SIZE];
	
	/* Nbr intf IP */
	uint32_t nbr_intf_ip;
	
	/* Nbr if index */
	uint32_t remote_if_index;
	
	/* Hold time in sec reported by nbr */
	uint32_t hold_time;
	
	/* Nbr link cost Value */
	uint32_t cost;
	
	/* Adj state */
	isis_adj_state_t adj_state;
	
	/* uptime */
	time_t uptime;
	
	/* Expiry timer */
	timer_event_handle *expiry_timer;
	
	/* Delete timer */
	timer_event_handle *delete_timer;
	
	/* Nbr mac address*/
	mac_add_t nbr_mac; 
	
} isis_adjacency_t;


//typedef struct isis_intf_info_ {
	
	/* Interface cost */
//	uint32_t cost;
	
	/* Helo time interval */
//	uint32_t hello_interval;
	
	
//} isis_intf_info_t; 

void
isis_update_interface_adjacency_from_hello(interface_t *iff,byte *hello_tlv_buffer,size_t tlb_buff_size);


void
isis_show_adjacency( isis_adjacency_t *adjacency, uint8_t tab_spaces);

static void
isis_adjacency_start_delete_timer(isis_adjacency_t *adjacency);

static void
isis_adjacency_stop_delete_timer(isis_adjacency_t *adjacency);

static void
isis_adjacecy_start_expiry_timer(isis_adjacency_t *adjacency);

static void
isis_adjacency_stop_expiry_timer(isis_adjacency_t *adjacency);

static void
isis_timer_expire_delete_adjacency_cb(void *arg, uint32_t arg_size);

static void
isis_timer_expery_down_adjacency_cb(void *arg, uint32_t arg_size);

void
isis_change_adjacency_state(isis_adjacency_t *adjacency, isis_adj_state_t new_adj_state);

void
isis_adjacency_set_uptime(isis_adjacency_t *adjacency) ;

void
print_current_system_time( void );

void
isis_delete_adjacency(isis_adjacency_t *adjacency) ;

#endif