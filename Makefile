CC=gcc
CFLAGS=-g
TARGET:tcpstack.exe CommandParser/libcli.a
LIBS=-lpthread -L ./CommandParser -lcli -lrt
OBJS=gluethread/glthread.o \
		  Tree/avl.o	   \
		  graph.o 		   \
		  topologies.o	   \
		  net.o			   \
		  comm.o		   \
		  Layer2/layer2.o  \
		  Layer3/layer3.o  \
		  Layer3/netfilter.o \
		  Layer4/layer4.o  \
		  Layer5/layer5.o  \
		  nwcli.o		   \
		  utils.o		   \
		  Layer2/l2switch.o \
          libtimer/WheelTimer.o   \
          libtimer/timerlib.o   \
          Layer5/nbrship_mgmt/nbrship_mgmt.o \
		  Layer5/ddcp/ddcp.o \
		  Layer5/spf_algo/spf.o \
		  tcp_stack_init.o	\
		  tcp_ip_trace.o	\
 		  tcpip_notif.o \
		  notif.o	\
		  EventDispatcher/event_dispatcher.o \
		  pkt_gen.o \
		  Layer2/stp/stp_state_machine.o \
		  Layer2/stp/stp_bpdu.o \
		  Layer2/stp/stp_init.o \

EventDispatcher/event_dispatcher.o:EventDispatcher/event_dispatcher.c
	${CC} ${CFLAGS} -c -I EventDispatcher -I gluethread EventDispatcher/event_dispatcher.c -o EventDispatcher/event_dispatcher.o	

pkt_gen.o:pkt_gen.c
	${CC} ${CFLAGS} -c pkt_gen.c -o pkt_gen.o

tcpstack.exe:testapp.o ${OBJS} CommandParser/libcli.a
	${CC} ${CFLAGS} testapp.o ${OBJS} -o tcpstack.exe ${LIBS}

notif.o:notif.c
	${CC} ${CFLAGS} -c -I gluethread -I . notif.c -o notif.o

tcpip_notif.o:tcpip_notif.c
	${CC} ${CFLAGS} -c -I gluethread -I . tcpip_notif.c -o tcpip_notif.o

testapp.o:testapp.c
	${CC} ${CFLAGS} -c testapp.c -o testapp.o

gluethread/glthread.o:gluethread/glthread.c
	${CC} ${CFLAGS} -c -I gluethread gluethread/glthread.c -o gluethread/glthread.o

Tree/avl.o:Tree/avl.c
	${CC} ${CFLAGS} -c -I Tree Tree/avl.c -o Tree/avl.o

libtimer/WheelTimer.o:libtimer/WheelTimer.c
	${CC} ${CFLAGS} -c -I gluethread -I libtimer libtimer/WheelTimer.c -o libtimer/WheelTimer.o
libtimer/timerlib..o:libtimer/timerlib.c
	${CC} ${CFLAGS} -c -I gluethread -I libtimer libtimer/timerlib.c -o libtimer/timerlib.o

tcp_stack_init.o:tcp_stack_init.c
	${CC} ${CFLAGS} -c tcp_stack_init.c -o tcp_stack_init.o

Layer5/nbrship_mgmt/nbrship_mgmt.o:Layer5/nbrship_mgmt/nbrship_mgmt.c
	${CC} ${CFLAGS} -c -I . Layer5/nbrship_mgmt/nbrship_mgmt.c -o Layer5/nbrship_mgmt/nbrship_mgmt.o

graph.o:graph.c
	${CC} ${CFLAGS} -c -I . graph.c -o graph.o

topologies.o:topologies.c
	${CC} ${CFLAGS} -c -I . topologies.c -o topologies.o

net.o:net.c
	${CC} ${CFLAGS} -c -I . net.c -o net.o

comm.o:comm.c
	${CC} ${CFLAGS} -c -I . comm.c -o comm.o

tcp_ip_trace.o:tcp_ip_trace.c
	${CC} ${CFLAGS} -c -I . tcp_ip_trace.c -o tcp_ip_trace.o

Layer2/layer2.o:Layer2/layer2.c
	${CC} ${CFLAGS} -c -I . Layer2/layer2.c -o Layer2/layer2.o

Layer2/l2switch.o:Layer2/l2switch.c
	${CC} ${CFLAGS} -c -I . Layer2/l2switch.c -o Layer2/l2switch.o

Layer3/layer3.o:Layer3/layer3.c
	${CC} ${CFLAGS} -c -I . Layer3/layer3.c -o Layer3/layer3.o

Layer3/netfilter.o:Layer3/netfilter.c
	${CC} ${CFLAGS} -c -I . Layer3/netfilter.c -o Layer3/netfilter.o

Layer5/spf_algo/spf.o:Layer5/spf_algo/spf.c
	${CC} ${CFLAGS} -c -I . Layer5/spf_algo/spf.c -o Layer5/spf_algo/spf.o

Layer4/layer4.o:Layer4/layer4.c
	${CC} ${CFLAGS} -c -I . Layer4/layer4.c -o Layer4/layer4.o
	
Layer5/layer5.o:Layer5/layer5.c
	${CC} ${CFLAGS} -c -I . Layer5/layer5.c -o Layer5/layer5.o

nwcli.o:nwcli.c
	${CC} ${CFLAGS} -c -I . nwcli.c  -o nwcli.o

utils.o:utils.c
	${CC} ${CFLAGS} -c -I . utils.c -o utils.o

Layer5/ddcp/ddcp.o:Layer5/ddcp/ddcp.c
	${CC} ${CFLAGS} -c -I . -I Layer5/ddcp/ Layer5/ddcp/ddcp.c -o Layer5/ddcp/ddcp.o

# Protocols Specific
# STP
Layer2/stp/stp_state_machine.o:Layer2/stp/stp_state_machine.c
	${CC} ${CFLAGS} -c Layer2/stp/stp_state_machine.c -o Layer2/stp/stp_state_machine.o
Layer2/stp/stp_bpdu.o:Layer2/stp/stp_bpdu.c
	${CC} ${CFLAGS} -c Layer2/stp/stp_bpdu.c -o Layer2/stp/stp_bpdu.o
Layer2/stp/stp_init.o:Layer2/stp/stp_init.c
	${CC} ${CFLAGS} -c Layer2/stp/stp_init.c -o Layer2/stp/stp_init.o

CommandParser/libcli.a:
	(cd CommandParser; make)
clean:
	rm -f *.o
	rm -f gluethread/glthread.o
	rm -f Tree/avl.o
	rm -f *exe
	rm -f Layer2/*.o
	rm -f Layer3/*.o
	rm -f Layer4/*.o
	rm -f Layer5/*.o
	rm -f Layer5/ddcp/*.o
	rm -f Layer5/spf_algo/*.o
	rm -f libtimer/*.o
	rm -f EventDispatcher/*.o
	rm -f Layer5/nbrship_mgmt/*.o
#STP
	rm -f Layer2/stp/*.o
all:
	make

cleanall:
	make clean
	(cd CommandParser; make clean)
