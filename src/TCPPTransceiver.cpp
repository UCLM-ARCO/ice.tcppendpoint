// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

#include <TCPPTransceiver.h>
#include <Instance.h>
#include <Ice/TraceLevels.h>
#include <Ice/LoggerUtil.h>
#include <Ice/Stats.h>
#include <Ice/Buffer.h>
#include <Ice/Network.h>
#include <Ice/LocalException.h>
#include <Ice/Communicator.h>

using namespace std;
using namespace Ice;
using namespace IceTCPP;

SOCKET
IceTCPP::TCPPTransceiver::fd()
{
    assert(_fd != INVALID_SOCKET);
    return _fd;
}

void
IceTCPP::TCPPTransceiver::close()
{
  // if(_traceLevels->network >= 1)
  // {
  //     Trace out(_logger, _traceLevels->networkCat);
  //     out << "closing tcp connection\n" << toString();
  // }
  
  assert(_fd != INVALID_SOCKET);
  try
    {
      IceInternal::closeSocket(_fd);
      _fd = INVALID_SOCKET;
    }
  catch(const SocketException&)
    {
      _fd = INVALID_SOCKET;
      throw;
    }
}

bool
IceTCPP::TCPPTransceiver::write(IceInternal::Buffer& buf)
{
  // Its impossible for the packetSize to be more than an Int.
  int packetSize = static_cast<int>(buf.b.end() - buf.i);
  
#ifdef _WIN32
  //
  // Limit packet size to avoid performance problems on WIN32
  //
  if(packetSize > _maxPacketSize)
    { 
      packetSize = _maxPacketSize;
    }
#endif
  
  while(buf.i != buf.b.end())
    {
      assert(_fd != INVALID_SOCKET);
      ssize_t ret = ::send(_fd, reinterpret_cast<const char*>(&*buf.i), packetSize, 0);
      
      if(ret == 0)
	{
	  ConnectionLostException ex(__FILE__, __LINE__);
	  ex.error = 0;
	  throw ex;
	}
      
      if(ret == SOCKET_ERROR)
	{
	  if(IceInternal::interrupted())
	    {
	      continue;
	    }
	  
	  if(IceInternal::noBuffers() && packetSize > 1024)
	    {
	      packetSize /= 2;
	      continue;
	    }
	  
	  if(IceInternal::wouldBlock())
	    {
	      return false;
	    }
	  
	  if(IceInternal::connectionLost())
            {
	      ConnectionLostException ex(__FILE__, __LINE__);
	      ex.error = IceInternal::getSocketErrno();
	      throw ex;
            }
	  else
            {
	      SocketException ex(__FILE__, __LINE__);
	      ex.error = IceInternal::getSocketErrno();
	      throw ex;
            }
        }
      
      // if(_traceLevels->network >= 3)
      // {
      //   Trace out(_logger, _traceLevels->networkCat);
      //   out << "sent " << ret << " of " << packetSize << " bytes via tcp\n" << toString();
      // }
      
      if(_stats)
        {
	  _stats->bytesSent(type(), static_cast<Int>(ret));
        }
      
      buf.i += ret;
      
      if(packetSize > buf.b.end() - buf.i)
        {
	  packetSize = static_cast<int>(buf.b.end() - buf.i);
        }
    }
  
  return true;
}

bool
IceTCPP::TCPPTransceiver::read(IceInternal::Buffer& buf)
{
  // Its impossible for the packetSize to be more than an Int.
  int packetSize = static_cast<int>(buf.b.end() - buf.i);
  
  while(buf.i != buf.b.end())
    {
      assert(_fd != INVALID_SOCKET);
      ssize_t ret = ::recv(_fd, reinterpret_cast<char*>(&*buf.i), packetSize, 0);
      
      if(ret == 0)
        {
	  //
	  // If the connection is lost when reading data, we shut
	  // down the write end of the socket. This helps to unblock
	  // threads that are stuck in send() or select() while
	  // sending data. Note: I don't really understand why
	  // send() or select() sometimes don't detect a connection
	  // loss. Therefore this helper to make them detect it.
	  //
	  //assert(_fd != INVALID_SOCKET);
	  //shutdownSocketReadWrite(_fd);
          
	  ConnectionLostException ex(__FILE__, __LINE__);
	  ex.error = 0;
	  throw ex;
        }
      
      if(ret == SOCKET_ERROR)
	{
	  if(IceInternal::interrupted())
	    {
	      continue;
	    }
	  
	  if(IceInternal::noBuffers() && packetSize > 1024)
	    {
	      packetSize /= 2;
	      continue;
	    }
	  
	  if(IceInternal::wouldBlock())
	    {
	      return false;
	    }
	  
	  if(IceInternal::connectionLost())
	    {
	      //
	      // See the commment above about shutting down the
	      // socket if the connection is lost while reading
	      // data.
	      //
	      //assert(_fd != INVALID_SOCKET);
	      //shutdownSocketReadWrite(_fd);
	      
	      ConnectionLostException ex(__FILE__, __LINE__);
	      ex.error = IceInternal::getSocketErrno();
	      throw ex;
            }
	  else
            {
	      SocketException ex(__FILE__, __LINE__);
	      ex.error = IceInternal::getSocketErrno();
	      throw ex;
            }
        }
      
      // if(_traceLevels->network >= 3)
      // {
      //     Trace out(_logger, _traceLevels->networkCat);
      //     out << "received " << ret << " of " << packetSize << " bytes via tcp\n" << toString();
      // }
      
      if(_stats)
        {
	  _stats->bytesReceived(type(), static_cast<Int>(ret));
        }
      
      buf.i += ret;
      
      if(packetSize > buf.b.end() - buf.i)
        {
	  packetSize = static_cast<int>(buf.b.end() - buf.i);
        }
    }
  
  return true;
}

string
IceTCPP::TCPPTransceiver::type() const
{
    return "tcpp";
}

string
IceTCPP::TCPPTransceiver::toString() const
{
    return _desc;
}

IceInternal::SocketStatus
IceTCPP::TCPPTransceiver::initialize()
{
  if(_state == StateNeedConnect)
    {
      _state = StateConnectPending;
      return IceInternal::NeedConnect;
    }
  else if(_state <= StateConnectPending)
    {
      try
	{
	  IceInternal::doFinishConnect(_fd);
	  _state = StateConnected;
	  _desc = IceInternal::fdToString(_fd);
	}
      catch(const Ice::LocalException& ex)
	{
	  // if(_traceLevels->network >= 2)
	  //   {
	  //     Trace out(_logger, _traceLevels->networkCat);
	  //     out << "failed to establish tcp connection\n" << _desc << "\n" << ex;
	  //   }
	  throw;
	}
      
      // if(_traceLevels->network >= 1)
      //   {
      //     Trace out(_logger, _traceLevels->networkCat);
      //     out << "tcp connection established\n" << _desc;
      //   }
    }
  assert(_state == StateConnected);
  return IceInternal::Finished;
}

void
IceTCPP::TCPPTransceiver::checkSendSize(const IceInternal::Buffer& buf, size_t messageSizeMax)
{
  if(buf.b.size() > messageSizeMax)
    {
      throw MemoryLimitException(__FILE__, __LINE__);
    }
}

IceTCPP::TCPPTransceiver::TCPPTransceiver(const InstancePtr& instance, SOCKET fd, bool connected) :
  //_traceLevels(instance->traceLevels()),
  _logger(instance->communicator()->getLogger()),
  _stats(instance->communicator()->getStats()),
  _fd(fd),
  _state(connected ? StateConnected : StateNeedConnect),
  _desc(IceInternal::fdToString(_fd))
{
#ifdef _WIN32
  //
  // On Windows, limiting the buffer size is important to prevent
  // poor throughput performances when transfering large amount of
  // data. See Microsoft KB article KB823764.
  //
  _maxPacketSize = IceInternal::getSendBufferSize(_fd) / 2;
  if(_maxPacketSize < 512)
    {
      _maxPacketSize = 0;
    }
#endif
}

IceTCPP::TCPPTransceiver::~TCPPTransceiver()
{
    assert(_fd == INVALID_SOCKET);
}
