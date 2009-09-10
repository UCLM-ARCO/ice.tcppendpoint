// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

#ifndef ICE_TCPP_TRANSCEIVER_H
#define ICE_TCPP_TRANSCEIVER_H

#include <InstanceF.h>
#include <Ice/Plugin.h>
#include <Ice/TraceLevelsF.h>
#include <Ice/LoggerF.h>
#include <Ice/StatsF.h>
#include <Ice/Transceiver.h>

namespace IceTCPP
{

  class TCPPConnector;
  class TCPPAcceptor;
  
  class TCPPTransceiver : public IceInternal::Transceiver
    {
      enum State
      {
        StateNeedConnect,
        StateConnectPending,
        StateConnected
      };
      
    public:
      
      virtual SOCKET fd();
      virtual void close();
      virtual bool write(IceInternal::Buffer&);
      virtual bool read(IceInternal::Buffer&);
      virtual std::string type() const;
      virtual std::string toString() const;
      virtual IceInternal::SocketStatus initialize();
      virtual void checkSendSize(const IceInternal::Buffer&, size_t);
      
      //    ConnectionInfo getConnectionInfo() const;
    private:
      
      TCPPTransceiver(const InstancePtr&, SOCKET, bool);
      virtual ~TCPPTransceiver();
      friend class TCPPConnector;
      friend class TCPPAcceptor;
      
      const InstancePtr _instance;
      //const TraceLevelsPtr _traceLevels;
      const Ice::LoggerPtr _logger;
      const Ice::StatsPtr _stats;
      
      SOCKET _fd;
      
      const std::string _host;
      
      //const bool _incoming;
      const std::string _adapterName;
      
      State _state;
      std::string _desc;
#ifdef _WIN32
      int _maxPacketSize;
#endif
    };
  typedef IceUtil::Handle<TCPPTransceiver> TCPPTransceiverPtr;
}

#endif
