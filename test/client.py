#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys, Ice
Ice.loadSlice('Hello.ice')
import UCLM

class Client(Ice.Application):
    def run(self,args):
	obj = self.communicator().stringToProxy(
            'hello1 -t:' + self.communicator().getProperties().getProperty('OA.Endpoints'))
	hello = UCLM.HelloPrx.uncheckedCast(obj)
	hello.puts('Hola Mundo');
        print 'Mensaje enviado'
	return 0

sys.exit(Client().main(sys.argv))
