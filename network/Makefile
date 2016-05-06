TCP_SERVER_DEMO_OBJS=	tcp-server-demo.o tcp-server.o

tcp-server-demo:	$(TCP_SERVER_DEMO_OBJS)
			$(CXX) -o tcp-server-demo  $(TCP_SERVER_DEMO_OBJS) -lpthread

clean:
			rm -f tcp-server-demo $(TCP_SERVER_DEMO_OBJS)
