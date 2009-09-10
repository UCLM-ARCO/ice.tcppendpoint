// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

#ifndef ICE_TCPP_INSTANCE_F_H
#define ICE_TCPP_INSTANCE_F_H

#include <IceUtil/Shared.h>

#include <Ice/Handle.h>

namespace IceTCPP
{

class Instance;

}

namespace IceInternal
{

IceUtil::Shared* upCast(IceTCPP::Instance*);

}

namespace IceTCPP
{

typedef IceInternal::Handle<Instance> InstancePtr;

}

#endif
