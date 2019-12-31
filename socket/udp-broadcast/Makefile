target= server client
app:$(target)
server:server.cpp
	g++ server.cpp -o server
client:client.cpp
	g++ client.cpp -o client

.PHONY: clean
clean:
	rm server client
