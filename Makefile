TARGET = example

SRC := src
SRC_URG = $(SRC)/LIDAR
INCLUDE := include
CC = g++
CFLAGS = -I. -I$(SRC_URG)/cpp -I$(SRC_URG) -I$(SRC) -I$(INCLUDE)
LIBS = -pthread -L$(SRC_URG)/src -L$(SRC_URG) -lrt
AUX = $(SRC)/*.cpp

all: $(TARGET)

$(TARGET): $(TARGET).cpp $(AUX)
	$(CC) -fpermissive -o $(TARGET) $(TARGET).cpp $(AUX) $(SRC_URG)/lidar.cpp $(SRC_URG)/Connection_information.o $(SRC_URG)/src/liburg_cpp.a $(CFLAGS) $(LIBS)
	
clean:
	$(RM) $(TARGET)
