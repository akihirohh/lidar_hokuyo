TARGET = example

SRC := src
CC = g++
INCLUDE = -Iinclude -I/usr/local/include/urg_cpp 
LIBS = -pthread -lrt /usr/local/lib/liburg_cpp.a
AUX = $(SRC)/*.cpp

all: $(TARGET)

$(TARGET): $(TARGET).cpp $(AUX)
	$(CC) -fpermissive -o $(TARGET) $(TARGET).cpp $(AUX) $(INCLUDE) $(LIBS)
	
clean:
	$(RM) $(TARGET)
