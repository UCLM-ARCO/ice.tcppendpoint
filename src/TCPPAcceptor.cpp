// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

#include <TCPPAcceptor.h>
#include <TCPPTransceiver.h>
#include <Instance.h>
#include <Ice/Communicator.h>
#include <Ice/TraceLevels.h>
#include <Ice/LoggerUtil.h>
#include <Ice/Network.h>
#include <Ice/Exception.h>
#include <Ice/Properties.h>

using namespace std;
using namespace Ice;
using namespace IceTCPP;

SOCKET
IceTCPP::TCPPAcceptor::fd()
{
    return _fd;
}

void
IceTCPP::TCPPAcceptor::close()
{
  // if(_traceLevels->network >= 1)
  //   {
  //     Trace out(_logger, _traceLevels->networkCat);
  //     out << "stopping to accept tcp connections at " << toString();
  //   }
  
  SOCKET fd = _fd;
  _fd = INVALID_SOCKET;
  IceInternal::closeSocket(fd);
}

void
IceTCPP::TCPPAcceptor::listen()
{
  try
    {
      IceInternal::doListen(_fd, _backlog);
    }
  catch(...)
    {
      _fd = INVALID_SOCKET;
      throw;
    }
  
  // if(_traceLevels->network >= 1)
  //   {
  //     Trace out(_logger, _traceLevels->networkCat);
  //     out << "accepting tcp connections at " << toString();
  //   }
}

IceInternal::TransceiverPtr
IceTCPP::TCPPAcceptor::accept()
{
  SOCKET fd = IceInternal::doAccept(_fd);
  IceInternal::setBlock(fd, false);
  IceInternal::setTcpBufSize(fd, _instance->communicator()->getProperties(), _logger);
  
  // if(_traceLevels->network >= 1)
  //   {
  //     Trace out(_logger, _traceLevels->networkCat);
  //     out << "accepted tcp connection\n" << fdToString(fd);
  //   }
  
  return new BTTransceiver(_instance, fd, true);
}

string
IceTCPP::TCPPAcceptor::toString() const
{
  return IceInternal::addrToString(_addr);
}

int
IceTCPP::TCPPAcceptor::effectivePort() const
{
  return IceInternal::getPort(_addr);
}

IceTCPP::TCPPAcceptor::TCPPAcceptor(const InstancePtr& instance, const string& host, int port) :
  _instance(instance),
  //_traceLevels(instance->traceLevels()),
  _logger(instance->communicator()->getLogger())
{
#ifdef SOMAXCONN
  _backlog = instance->communicator()->getProperties()->getPropertyAsIntWithDefault("Ice.TCP.Backlog", SOMAXCONN);
#else
  _backlog = instance->communicator()->getProperties()->getPropertyAsIntWithDefault("Ice.TCP.Backlog", 511);
#endif
  
  try
    {
      getAddressForServer(host, port, _addr, _instance->protocolSupport());
      _fd = IceInternal::createSocket(false, _addr.ss_family);
      IceInternal::setBlock(_fd, false);
      IceInternal::setTcpBufSize(_fd, _instance->communicator()->getProperties(), _logger);
#ifndef _WIN32
      //
      // Enable SO_REUSEADDR on Unix platforms to allow re-using the
      // socket even if it's in the TIME_WAIT state. On Windows,
      // this doesn't appear to be necessary and enabling
      // SO_REUSEADDR would actually not be a good thing since it
      // allows a second process to bind to an address even it's
      // already bound by another process.
      //
      // TODO: using SO_EXCLUSIVEADDRUSE on Windows would probably
      // be better but it's only supported by recent Windows
      // versions (XP SP2, Windows Server 2003).
      //
      IceInternal::setReuseAddress(_fd, true);
#endif
      // if(_traceLevels->network >= 2)
      //   {
      // 	  Trace out(_logger, _traceLevels->networkCat);
      // 	  out << "attempting to bind to tcp socket " << toString();
      //   }
      IceInternal::doBind(_fd, _addr);
    }
  catch(...)
    {
      _fd = INVALID_SOCKET;
      throw;
    }
}

IceTCPP::TCPPAcceptor::~TCPPAcceptor()
{
    assert(_fd == INVALID_SOCKET);
}
