H_FILES = \
	TCPPAcceptor.h TCPPConnector.h TCPPTransceiver.h \
	TCPPEndpointI.h PluginI.h Instance.h InstanceF.h
CXX_FILES = \
	TCPPAcceptor.cpp TCPPConnector.cpp TCPPTransceiver.cpp \
	TCPPEndpointI.cpp PluginI.cpp Instance.cpp

noinst_HEADERS = $(H_FILES)
lib_LTLIBRARIES = libIceTCPP.la
libIceTCPP_la_SOURCES = $(H_FILES) $(CXX_FILES)
#libIceBT_la_LDFLAGS = -module -export-dynamic -version-info $(GENERIC_LIBRARY_VERSION) -rpath /usr/local/lib
libIceTCPP_la_LDFLAGS = -version-info $(GENERIC_LIBRARY_VERSION)
libIceTCPP_la_LIBADD =-lIceUtil -lIce
