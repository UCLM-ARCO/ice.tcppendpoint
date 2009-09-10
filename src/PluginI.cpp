// **********************************************************************
//
// Copyright (c) 2003-2007 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

#include <PluginI.h>
#include <Instance.h>
#include <TCPPTransceiver.h>
#include <Ice/BuiltinSequences.h>
#include <Ice/Communicator.h>
#include <Ice/LocalException.h>
#include <Ice/Logger.h>
#include <Ice/Properties.h>
#include <IceUtil/StaticMutex.h>

using namespace std;
using namespace Ice;
using namespace IceTCPP;

//
// Plugin factory function.
//
extern "C"
{

Ice::Plugin*
createIceTCPP(const CommunicatorPtr& communicator, const string& name, const StringSeq& args)
{
   PluginI* plugin = new PluginI(communicator);
   return plugin;
}

}

//
// Plugin implementation.
//
IceTCPP::PluginI::PluginI(const Ice::CommunicatorPtr& communicator)
{
  _instance = new Instance(communicator);
}

void
IceTCPP::PluginI::initialize()
{
  _instance->initialize();
}

void
IceTCPP::PluginI::destroy()
{
  _instance->destroy();
  _instance = 0;

}
