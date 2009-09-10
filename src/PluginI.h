// **********************************************************************
//
// Copyright (c) 2003-2007 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

#ifndef ICE_TCPP_PLUGIN_I_H
#define ICE_TCPP_PLUGIN_I_H

#include <InstanceF.h>
#include <Ice/CommunicatorF.h>
#include <Ice/Plugin.h>

namespace IceTCPP
{

class PluginI : public Ice::Plugin
{
public:

    PluginI(const Ice::CommunicatorPtr&);

    //
    // From Ice::Plugin.
    //
    virtual void initialize();
    virtual void destroy();

private:

    InstancePtr _instance;
};

}

#endif
