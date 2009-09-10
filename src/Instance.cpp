// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

#include <Instance.h>
#include <TCPPEndpointI.h>

#include <Ice/Communicator.h>
#include <Ice/LocalException.h>
#include <Ice/Logger.h>
#include <Ice/LoggerUtil.h>
#include <Ice/Properties.h>
#include <Ice/ProtocolPluginFacade.h>

#include <IceUtil/DisableWarnings.h>

using namespace std;
using namespace Ice;
using namespace IceTCPP;

IceUtil::Shared* IceInternal::upCast(IceTCPP::Instance* p) { return p; }

static int instanceCount = 0;

extern "C"
{
  
  IceTCPP::Instance::Instance(const CommunicatorPtr& communicator) :
    _logger(communicator->getLogger()),
    _initialized(false)
  {
    __setNoDelete(true);
    _facade = IceInternal::getProtocolPluginFacade(communicator);
    //
    // Register the endpoint factory. We have to do this now, rather than
    // in initialize, because the communicator may need to interpret
    // proxies before the plug-in is fully initialized.
    //
    _facade->addEndpointFactory(new TCPPEndpointFactory(this));
    __setNoDelete(false);
  }
  
  IceTCPP::Instance::~Instance()
  {

  }

  void
  IceTCPP::Instance::initialize()
  {
    if(_initialized)
      {
        return;
      }
    
    try
      {
        const string propPrefix = "IceTCPP.";
        PropertiesPtr properties = communicator()->getProperties();
	
        //
        // Select protocols.
        //
        StringSeq protocols = properties->getPropertyAsList(propPrefix + "Protocols");
      }
    catch(...)
      {
        throw;
      }
    
    _initialized = true;
  }
  
  CommunicatorPtr
  IceTCPP::Instance::communicator() const
  {
    return _facade->getCommunicator();
  }
  
  IceInternal::EndpointHostResolverPtr
  IceTCPP::Instance::endpointHostResolver() const
  {
    return _facade->getEndpointHostResolver();
  }
  
  IceInternal::ProtocolSupport
  IceTCPP::Instance::protocolSupport() const
  {
    return _facade->getProtocolSupport();
  }
  
  string
  IceTCPP::Instance::defaultHost() const
  {
    return _facade->getDefaultHost();
  }
  
  int
  IceTCPP::Instance::networkTraceLevel() const
  {
    return _facade->getNetworkTraceLevel();
  }
  
  string
  IceTCPP::Instance::networkTraceCategory() const
  {
    return _facade->getNetworkTraceCategory();
  }
  
  void
  IceTCPP::Instance::destroy()
  {
    _facade = 0;
    
  }
}

