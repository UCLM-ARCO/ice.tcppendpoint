#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys, Ice
Ice.loadSlice('Hello.ice')
import UCLM

class HelloI(UCLM.Hello):
    def puts(self,str,current=None):
	print str
        sys.stdout.flush()

class Server(Ice.Application):
    def run(self,args):
	self.shutdownOnInterrupt()
	oa = self.communicator().createObjectAdapter('OA')
	obj = oa.add(HelloI(),self.communicator().stringToIdentity('hello1'))
	oa.activate()
	print self.communicator().proxyToString(obj)
	self.communicator().waitForShutdown()
	print "Done"
	return 0

sys.exit(Server().main(sys.argv))
