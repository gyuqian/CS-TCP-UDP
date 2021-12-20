BUILD = g++

CRC16 = CRC16
SERVER = SERVER
CLIENT = CLIENT

outputdir = OUTPUT/
objdir = OBJ/

src = $(wildcard ./*/SRC/*.cpp)

srcdir = $(wildcard ./*/SRC/)
source = $(notdir $(src))
obj = $(patsubst %.cpp,$(objdir)%.o,$(source))
inc = $(wildcard ./*/INC/)

CCobj = -I "$(inc)" -c -o

all:$(CRC16) $(SERVER) $(CLIENT)

$(CRC16):crc16_xmodem_main.o
	$(BUILD) $(objdir)crc16_xmodem_main.o -o $(outputdir)$(CRC16)

$(SERVER):Server_TCP_UDP.o
	$(BUILD) $(objdir)Server_TCP_UDP.o -o $(outputdir)$(SERVER)

$(CLIENT):Client_TCP_UDP.o
	$(BUILD) $(objdir)Client_TCP_UDP.o -o $(outputdir)$(CLIENT)

crc16_xmodem_main.o:CRC/SRC/crc16_xmodem_main.cpp
	$(BUILD) CRC/SRC/crc16_xmodem_main.cpp $(CCobj) $(objdir)crc16_xmodem_main.o

Server_TCP_UDP.o:./SERVER/SRC/Server_TCP_UDP.cpp
	$(BUILD) ./SERVER/SRC/Server_TCP_UDP.cpp $(CCobj) $(objdir)Server_TCP_UDP.o

Client_TCP_UDP.o:./CLIENT/SRC/Client_TCP_UDP.cpp
	$(BUILD) ./CLIENT/SRC/Client_TCP_UDP.cpp $(CCobj) $(objdir)Client_TCP_UDP.o

clean:
	rm $(obj) $(outputdir)$(CRC16) $(outputdir)$(SERVER) $(outputdir)$(CLIENT)