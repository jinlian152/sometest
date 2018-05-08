cmake_minimum_required(VERSION 3.7)
project(src)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        CCommonData.cpp
        CCommonData.h
        CIOCPWorkThread.cpp
        CIOCPWorkThread.h
        circleBuffer.cpp
        circleBuffer.h
        CircularBuffer.cpp
        CircularBuffer.h
        CListener.cpp
        CListener.h
        CSessionMgr.cpp
        CSessionMgr.h
        CSyncBuffer.cpp
        CSyncBuffer.h
        CSyncQueue.h
        CTcpConnection.cpp
        CTcpConnection.h
        CTcpConnectionMgr.cpp
        CTcpConnectionMgr.h
        Epoll.cpp
        Mutex.h
        Session.cpp
        Session.h
        threadBase.cpp
        threadBase.h
        Typedef.cpp
        Typedef.h)

add_executable(src ${SOURCE_FILES})