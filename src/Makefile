#CXXFLAGS =	-O2 -g -Wall -fmessage-length=0
CC = g++ -Wall
COMPLIE = $(CC) -c -g
#CXXFLAGS = -lgcc_s
MAKEDST = $(CC) $(CXXFLAGS)

INC =		-I /usr/local/include -I /usr/include

#OBJS =		OpensslTest.o
OBJS =		CTcpConnection.o CIOCPWorkThread.o threadBase.o CTcpConnectionMgr.o CListener.o CCommonData.o circleBuffer.o Session.o CSyncBuffer.o CSessionMgr.o Epoll.o CircularBuffer.o Typedef.o

LIBS =		-L /usr/local/lib -L /usr/lib64 -lpthread -lrt 

TARGET =	epoll

#$(TARGET):	$(OBJS)
#	$(CXX) -o $(TARGET) $(INC) $(OBJS) $(LIBS)

all:	$(TARGET)

#$(TARGET): $(OBJS)
#	$(CXX) $^ -o $@ $(CXXFLAGS) $(LIBS)
#%.o: %.cc
#	$(CXX) $< -o $(INC) $@
$(TARGET): $(OBJS)
	$(MAKEDST) $^ -o $@ $(LLIB) $(LIBS)
%.o: %.cpp
	$(COMPLIE) $(INC) $< -o $@ $(LLIB)

clean:
	rm -f $(OBJS) $(TARGET)
