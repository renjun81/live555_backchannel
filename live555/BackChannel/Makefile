INCLUDES = -I../UsageEnvironment/include -I../groupsock/include -I../liveMedia/include -I../BasicUsageEnvironment/include -I../BackChannel/include
PREFIX = /usr/local
LIBDIR = $(PREFIX)/lib
##### Change the following for your environment:
COMPILE_OPTS =		$(INCLUDES) -I. -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.9.sdk/usr/include $(EXTRA_LDFLAGS) -DBSD=1 -O -DSOCKLEN_T=socklen_t -DHAVE_SOCKADDR_LEN=1 -DTIME_BASE=int
C =			c
C_COMPILER =		cc
C_FLAGS =		$(COMPILE_OPTS)
CPP =			cpp
CPLUSPLUS_COMPILER =	c++
CPLUSPLUS_FLAGS =	$(COMPILE_OPTS) -Wall
OBJ =			o
LINK =			c++ -o 
LINK_OPTS =		-L.
CONSOLE_LINK_OPTS =	$(LINK_OPTS)
LIBRARY_LINK =		libtool -s -o 
LIBRARY_LINK_OPTS =	
LIB_SUFFIX =			a
LIBS_FOR_CONSOLE_APPLICATION =
LIBS_FOR_GUI_APPLICATION =
EXE =
##### End of variables to change

NAME = libbackChannel
ALL = $(NAME).$(LIB_SUFFIX)
all:	$(ALL)

.$(C).$(OBJ):
	$(C_COMPILER) -c $(C_FLAGS) $<
.$(CPP).$(OBJ):
	$(CPLUSPLUS_COMPILER) -c $(CPLUSPLUS_FLAGS) $<

BACKCHANNEL_LIB_OBJS = ADTSAudioBufferSource.$(OBJ) ADTSAudioFileSink.$(OBJ) ADTSBackChannelAudioFileServerMediaSubsession.$(OBJ) AudioUtility.$(OBJ)

ADTSAudioBufferSource.$(CPP):	include/ADTSAudioBufferSource.hh
ADTSAudioFileSink.$(CPP):	include/ADTSAudioFileSink.hh
ADTSBackChannelAudioFileServerMediaSubsession.$(CPP):	include/ADTSBackChannelAudioFileServerMediaSubsession.hh 
AudioUtility.$(C):		include/AudioUtility.h

libbackChannel.$(LIB_SUFFIX): $(BACKCHANNEL_LIB_OBJS) \
    $(PLATFORM_SPECIFIC_LIB_OBJS)
	$(LIBRARY_LINK)$@ $(LIBRARY_LINK_OPTS) \
		$(BACKCHANNEL_LIB_OBJS)

clean:
	-rm -rf *.$(OBJ) $(ALL) core *.core *~ include/*~

install: install1 $(INSTALL2)
install1: libbackChannel.$(LIB_SUFFIX)
	  install -d $(DESTDIR)$(PREFIX)/include/backChannel $(DESTDIR)$(LIBDIR)
	  install -m 644 include/*.hh include/*.h $(DESTDIR)$(PREFIX)/include/backChannel
	  install -m 644 libbackChannel.$(LIB_SUFFIX) $(DESTDIR)$(LIBDIR)
install_shared_libraries: libbackChannel.$(LIB_SUFFIX)
	  ln -s libbackChannel.$(LIB_SUFFIX) $(DESTDIR)$(LIBDIR)/libbackChannel.$(SHORT_LIB_SUFFIX)
	  ln -s libbackChannel.$(LIB_SUFFIX) $(DESTDIR)$(LIBDIR)/libbackChannel.so

##### Any additional, platform-specific rules come here:
