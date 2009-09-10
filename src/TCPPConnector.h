// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

#ifndef ICE_TCPP_CONNECTOR_H
#define ICE_TCPP_CONNECTOR_H

#include <Ice/TransceiverF.h>
#include <InstanceF.h>
#include <Ice/TraceLevelsF.h>
#include <Ice/LoggerF.h>
#include <Ice/Connector.h>

#ifdef _WIN32
#   include <winsock2.h>
#else
#   include <sys/socket.h>
#endif

namespace IceTCPP
{

  class TCPPConnector : public IceInternal::Connector
{
public:
    
  virtual IceInternal::TransceiverPtr connect();
  
  virtual Ice::Short type() const;
  virtual std::string toString() const;
  
  virtual bool operator==(const Connector&) const;
  virtual bool operator!=(const Connector&) const;
  virtual bool operator<(const Connector&) const;
  
 private:
  
  TCPPConnector(const InstancePtr&, const struct sockaddr_storage&, Ice::Int, const std::string&);
  virtual ~TCPPConnector();
  friend class TCPPEndpointI;
  
  const InstancePtr _instance;
  //const TraceLevelsPtr _traceLevels;
  const ::Ice::LoggerPtr _logger;
  struct sockaddr_storage _addr;
  const Ice::Int _timeout;
  const std::string _connectionId;
 };
  
}

#endif
