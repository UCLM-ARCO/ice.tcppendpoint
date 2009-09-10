// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

#include <TCPPConnector.h>
#include <TCPPTransceiver.h>
#include <TCPPEndpointI.h>
#include <Instance.h>
#include <Ice/TraceLevels.h>
#include <Ice/LoggerUtil.h>
#include <Ice/Network.h>
#include <Ice/Exception.h>
#include <Ice/Communicator.h>

using namespace std;
using namespace Ice;
using namespace IceTCPP;

IceInternal::TransceiverPtr
IceTCPP::TCPPConnector::connect()
{
  // if(_traceLevels->network >= 2)
  // {
  //     Trace out(_logger, _traceLevels->networkCat);
  //     out << "trying to establish tcp connection to " << toString();
  // }
  
  try
    {
      SOCKET fd = IceInternal::createSocket(false, _addr.ss_family);
      IceInternal::setBlock(fd, false);
      IceInternal::setTcpBufSize(fd, _instance->communicator()->getProperties(), _logger);
      bool connected = IceInternal::doConnect(fd, _addr);
      if(connected)
        {
	  // if(_traceLevels->network >= 1)
	  // {
	  //     Trace out(_logger, _traceLevels->networkCat);
	  //     out << "tcp connection established\n" << fdToString(fd);
	  // }
        }
      return new TCPPTransceiver(_instance, fd, connected);
    }
  catch(const Ice::LocalException& ex)
    {
      // if(_traceLevels->network >= 2)
      // {
      //     Trace out(_logger, _traceLevels->networkCat);
      //     out << "failed to establish tcp connection to " << toString() << "\n" << ex;
      // }
      throw;
    }
}

Short
IceTCPP::TCPPConnector::type() const
{
  return IceTCPP::EndpointType;
}

string
IceTCPP::TCPPConnector::toString() const
{
  return IceInternal::addrToString(_addr);
}

bool
IceTCPP::TCPPConnector::operator==(const IceInternal::Connector& r) const
{
  const TCPPConnector* p = dynamic_cast<const TCPPConnector*>(&r);
  if(!p)
    {
      return false;
    }
  
  if(IceInternal::compareAddress(_addr, p->_addr) != 0)
    {
      return false;
    }
  
  if(_timeout != p->_timeout)
    {
      return false;
    }
  
  if(_connectionId != p->_connectionId)
    {
      return false;
    }
  
  return true;
}

bool
IceTCPP::TCPPConnector::operator!=(const IceInternal::Connector& r) const
{
  return !operator==(r);
}

bool
IceTCPP::TCPPConnector::operator<(const IceInternal::Connector& r) const
{
  const TCPPConnector* p = dynamic_cast<const TCPPConnector*>(&r);
  if(!p)
    {
      return type() < r.type();
    }
  
  if(_timeout < p->_timeout)
    {
      return true;
    }
  else if(p->_timeout < _timeout)
    {
      return false;
    }
  
  if(_connectionId < p->_connectionId)
    {
      return true;
    }
  else if(p->_connectionId < _connectionId)
    {
      return false;
    }
  return IceInternal::compareAddress(_addr, p->_addr) == -1;
}

IceTCPP::TCPPConnector::TCPPConnector(const InstancePtr& instance, const struct sockaddr_storage& addr,
				      Ice::Int timeout, const string& connectionId) :
  _instance(instance),
  //_traceLevels(instance->traceLevels()),
  _logger(instance->communicator()->getLogger()),
  _addr(addr),
  _timeout(timeout),
  _connectionId(connectionId)
{
}

IceTCPP::TCPPConnector::~TCPPConnector()
{
}
