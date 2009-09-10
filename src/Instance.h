// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

#ifndef ICE_TCPP_INSTANCE_H
#define ICE_TCPP_INSTANCE_H

#include <InstanceF.h>
#include <Ice/CommunicatorF.h>
#include <Ice/LoggerF.h>
#include <Ice/Network.h>
#include <Ice/ProtocolPluginFacadeF.h>
#include <Ice/Plugin.h>
#include <Ice/BuiltinSequences.h>

namespace IceTCPP
{
  
  class Instance : public IceUtil::Shared
    {
    public:
      
      Instance(const Ice::CommunicatorPtr&);
      ~Instance();
      
      void initialize();
      
      Ice::CommunicatorPtr communicator() const;
      IceInternal::EndpointHostResolverPtr endpointHostResolver() const;
      IceInternal::ProtocolSupport protocolSupport() const;
      std::string defaultHost() const;
      int networkTraceLevel() const;
      std::string networkTraceCategory() const;
      
      void destroy();
    private:
       Ice::LoggerPtr _logger;
      IceInternal::ProtocolPluginFacadePtr _facade;
      bool _initialized;
    };
  
}

#endif
