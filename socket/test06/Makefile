src=$(wildcard ./*.cpp)
obj=$(patsubst %.cpp, %.o, $(src))
target= server client

app: $(target)
server: server.o wrap.o hqf_log.o -lpthread
	g++ $^ -o $@
client: client.o wrap.o hqf_log.o -lpthread
	g++ $^ -o $@

.PHONY: clean
clean:
	rm -rf $(obj) $(target)
