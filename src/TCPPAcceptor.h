// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

#ifndef ICE_TCPP_ACCEPTOR_H
#define ICE_TCPP_ACCEPTOR_H

#include <Ice/TransceiverF.h>
#include <InstanceF.h>
#include <Ice/TraceLevelsF.h>
#include <Ice/LoggerF.h>
#include <Ice/Acceptor.h>

#ifndef _WIN32
#   include <sys/socket.h> // For struct sockaddr_storage
#endif

namespace IceTCPP
{

  class TCPPEndpoint;

  class TCPPAcceptor : public IceInternal::Acceptor
  {
  public:
    
    virtual SOCKET fd();
    virtual void close();
    virtual void listen();
    virtual IceInternal::TransceiverPtr accept();
    virtual std::string toString() const;
    
    int effectivePort() const;
    
  private:
    
    TCPPAcceptor(const InstancePtr&, const std::string&, int);
    virtual ~TCPPAcceptor();
    friend class TCPPEndpointI;
    
    const InstancePtr _instance;
    const std::string _adapterName;
    //IceInternal::TraceLevelsPtr _traceLevels;
    Ice::LoggerPtr _logger;
    SOCKET _fd;
    int _backlog;
    struct sockaddr_storage _addr;
  }; 
}

#endif
