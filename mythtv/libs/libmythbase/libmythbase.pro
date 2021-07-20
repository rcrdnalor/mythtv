include ( ../../settings.pro )
#include ( ../../version.pro )

TEMPLATE = lib
TARGET = mythbase-$$LIBVERSION
CONFIG += thread dll
target.path = $${LIBDIR}
INSTALLS = target
QT += xml

QMAKE_CLEAN += $(TARGET) $(TARGETA) $(TARGETD) $(TARGET0) $(TARGET1) $(TARGET2)

# Input
HEADERS += mthread.h mthreadpool.h mythchrono.h
HEADERS += mythsocket.h mythsocket_cb.h
HEADERS += mythbaseexp.h mythdbcon.h mythdb.h mythdbparams.h
HEADERS += verbosedefs.h mythversion.h compat.h mythconfig.h
HEADERS += mythobservable.h mythevent.h
HEADERS += mythtimer.h mythdirs.h exitcodes.h
HEADERS += lcddevice.h mythstorage.h remotefile.h logging.h loggingserver.h
HEADERS += mythcorecontext.h mythsystem.h mythsystemprivate.h
HEADERS += mythlocale.h storagegroup.h
HEADERS += mythcoreutil.h mythdownloadmanager.h mythtranslation.h
HEADERS += unzip2.h iso639.h iso3166.h mythmedia.h
HEADERS += mythmiscutil.h mythhdd.h mythcdrom.h autodeletedeque.h dbutil.h
HEADERS += mythdeque.h mythlogging.h
HEADERS += mythbaseutil.h referencecounter.h referencecounterlist.h
HEADERS += version.h mythcommandlineparser.h
HEADERS += mythscheduler.h filesysteminfo.h hardwareprofile.h serverpool.h
HEADERS += mythbinaryplist.h bswap.h signalhandling.h mythtimezone.h mythdate.h
HEADERS += mythplugin.h mythpluginapi.h housekeeper.h
HEADERS += ffmpeg-mmx.h
HEADERS += mythsystemlegacy.h mythtypes.h
HEADERS += threadedfilewriter.h mythsingledownload.h codecutil.h
HEADERS += mythsession.h
HEADERS += ../../external/qjsonwrapper/qjsonwrapper/Json.h
HEADERS += cleanupguard.h portchecker.h
HEADERS += mythsorthelper.h mythdbcheck.h
HEADERS += mythpower.h

SOURCES += mthread.cpp mthreadpool.cpp
SOURCES += mythsocket.cpp
SOURCES += mythdbcon.cpp mythdb.cpp mythdbparams.cpp
SOURCES += mythobservable.cpp mythevent.cpp
SOURCES += mythtimer.cpp mythdirs.cpp
SOURCES += lcddevice.cpp mythstorage.cpp remotefile.cpp
SOURCES += mythcorecontext.cpp mythsystem.cpp mythlocale.cpp storagegroup.cpp
SOURCES += mythcoreutil.cpp mythdownloadmanager.cpp mythtranslation.cpp
SOURCES += unzip2.cpp iso639.cpp iso3166.cpp mythmedia.cpp mythmiscutil.cpp
SOURCES += mythhdd.cpp mythcdrom.cpp dbutil.cpp
SOURCES += logging.cpp loggingserver.cpp
SOURCES += referencecounter.cpp mythcommandlineparser.cpp
SOURCES += filesysteminfo.cpp hardwareprofile.cpp serverpool.cpp
SOURCES += mythbinaryplist.cpp signalhandling.cpp mythtimezone.cpp mythdate.cpp
SOURCES += mythplugin.cpp housekeeper.cpp
SOURCES += mythsystemlegacy.cpp mythtypes.cpp
SOURCES += threadedfilewriter.cpp mythsingledownload.cpp codecutil.cpp
SOURCES += mythsession.cpp
SOURCES += ../../external/qjsonwrapper/qjsonwrapper/Json.cpp
SOURCES += cleanupguard.cpp portchecker.cpp
SOURCES += mythsorthelper.cpp dbcheckcommon.cpp
SOURCES += mythpower.cpp

HEADERS += http/mythhttpcommon.h
HEADERS += http/mythhttptypes.h
HEADERS += http/mythhttps.h
HEADERS += http/mythhttpdata.h
HEADERS += http/mythhttpinstance.h
HEADERS += http/mythhttpserver.h
HEADERS += http/mythhttpthread.h
HEADERS += http/mythhttpthreadpool.h
HEADERS += http/mythhttpsocket.h
HEADERS += http/mythwebsockettypes.h
HEADERS += http/mythwebsocket.h
HEADERS += http/mythhttpparser.h
HEADERS += http/mythhttprequest.h
HEADERS += http/mythhttpresponse.h
HEADERS += http/mythhttpfile.h
HEADERS += http/mythhttpencoding.h
HEADERS += http/mythhttproot.h
HEADERS += http/mythhttpranges.h
HEADERS += http/mythhttpcache.h
HEADERS += http/mythhttpservice.h
HEADERS += http/mythhttpmetaservice.h
HEADERS += http/mythhttpmetamethod.h
HEADERS += http/mythhttpservices.h
HEADERS += http/mythmimedatabase.h
HEADERS += http/mythmimetype.h
HEADERS += http/mythwsdl.h
HEADERS += http/mythxsd.h
HEADERS += http/serialisers/mythserialiser.h
HEADERS += http/serialisers/mythjsonserialiser.h
HEADERS += http/serialisers/mythxmlserialiser.h
HEADERS += http/serialisers/mythxmlplistserialiser.h
HEADERS += http/serialisers/mythcborserialiser.h
SOURCES += http/mythhttpcommon.cpp
SOURCES += http/mythhttps.cpp
SOURCES += http/mythhttpdata.cpp
SOURCES += http/mythhttpinstance.cpp
SOURCES += http/mythhttpserver.cpp
SOURCES += http/mythhttpthread.cpp
SOURCES += http/mythhttpthreadpool.cpp
SOURCES += http/mythhttpsocket.cpp
SOURCES += http/mythwebsockettypes.cpp
SOURCES += http/mythwebsocket.cpp
SOURCES += http/mythhttpparser.cpp
SOURCES += http/mythhttprequest.cpp
SOURCES += http/mythhttpresponse.cpp
SOURCES += http/mythhttpfile.cpp
SOURCES += http/mythhttpencoding.cpp
SOURCES += http/mythhttproot.cpp
SOURCES += http/mythhttpranges.cpp
SOURCES += http/mythhttpcache.cpp
SOURCES += http/mythhttpservice.cpp
SOURCES += http/mythhttpmetaservice.cpp
SOURCES += http/mythhttpmetamethod.cpp
SOURCES += http/mythhttpservices.cpp
SOURCES += http/mythmimedatabase.cpp
SOURCES += http/mythmimetype.cpp
SOURCES += http/mythwsdl.cpp
SOURCES += http/mythxsd.cpp
SOURCES += http/serialisers/mythserialiser.cpp
SOURCES += http/serialisers/mythjsonserialiser.cpp
SOURCES += http/serialisers/mythxmlserialiser.cpp
SOURCES += http/serialisers/mythxmlplistserialiser.cpp
SOURCES += http/serialisers/mythcborserialiser.cpp

using_qtdbus {
    QT      += dbus
    DEFINES += USING_DBUS
    HEADERS += platforms/mythpowerdbus.h
    SOURCES += platforms/mythpowerdbus.cpp
}

unix {
    SOURCES += mythsystemunix.cpp
    HEADERS += mythsystemunix.h
}

mingw | win32-msvc* {
    SOURCES += mythsystemwindows.cpp
    HEADERS += mythsystemwindows.h
}

# Install headers to same location as libmyth to make things easier
inc.path = $${PREFIX}/include/mythtv/
inc.files += mythdbcon.h mythdbparams.h mythbaseexp.h mythdb.h
inc.files += compat.h mythversion.h mythconfig.h mythconfig.mak version.h
inc.files += mythobservable.h mythevent.h verbosedefs.h
inc.files += mythtimer.h lcddevice.h exitcodes.h mythdirs.h mythstorage.h
inc.files += mythsocket.h mythsocket_cb.h mythlogging.h
inc.files += mythcorecontext.h mythsystem.h storagegroup.h loggingserver.h
inc.files += mythcoreutil.h mythlocale.h mythdownloadmanager.h
inc.files += mythtranslation.h iso639.h iso3166.h mythmedia.h mythmiscutil.h
inc.files += mythcdrom.h autodeletedeque.h dbutil.h mythdeque.h
inc.files += referencecounter.h referencecounterlist.h mythcommandlineparser.h
inc.files += mthread.h mthreadpool.h mythchrono.h
inc.files += filesysteminfo.h hardwareprofile.h bonjourregister.h serverpool.h
inc.files += plist.h bswap.h signalhandling.h ffmpeg-mmx.h mythdate.h
inc.files += mythplugin.h mythpluginapi.h
inc.files += remotefile.h mythsystemlegacy.h mythtypes.h
inc.files += threadedfilewriter.h mythsingledownload.h mythsession.h
inc.files += mythsorthelper.h mythdbcheck.h

# Allow both #include <blah.h> and #include <libmythbase/blah.h>
inc2.path  = $${PREFIX}/include/mythtv/libmythbase
inc2.files = $${inc.files}

INSTALLS += inc inc2

INCLUDEPATH += ../../external/qjsonwrapper/ ../../external/libudfread ./platforms
INCLUDEPATH += ./http ./http/serialisers
DEPENDPATH  +=  ../../external/libudfread ./http ./http/serialisers

DEFINES += RUNPREFIX=\\\"$${RUNPREFIX}\\\"
DEFINES += LIBDIRNAME=\\\"$${LIBDIRNAME}\\\"
DEFINES += MBASE_API

linux:DEFINES += linux

macx {
    HEADERS += mythcdrom-darwin.h
    SOURCES += mythcdrom-darwin.cpp
    HEADERS += platforms/mythpowerosx.h
    SOURCES += platforms/mythpowerosx.cpp
    QMAKE_OBJECTIVE_CFLAGS += $$QMAKE_CXXFLAGS
    QMAKE_OBJECTIVE_CXXFLAGS += $$QMAKE_CXXFLAGS
    OBJECTIVE_HEADERS += platforms/mythcocoautils.h
    OBJECTIVE_SOURCES += platforms/mythcocoautils.mm
    LIBS              += -framework Cocoa -framework IOKit
}

linux {
    !android {
    SOURCES += mythcdrom-linux.cpp
    HEADERS += mythcdrom-linux.h
    }
}

freebsd {
    SOURCES += mythcdrom-freebsd.cpp
    HEADERS += mythcdrom-freebsd.h
    LIBS += -lthr -lc
}

use_hidesyms {
    QMAKE_CXXFLAGS += -fvisibility=hidden
}

using_libdns_sd {
    DEFINES += USING_LIBDNS_SD
    HEADERS += bonjourregister.h
    SOURCES += bonjourregister.cpp
    !macx: LIBS += -ldns_sd
}

using_x11:DEFINES += USING_X11

mingw:LIBS += -lws2_32

win32-msvc* {

    LIBS += -lws2_32
    EXTRA_LIBS += -lzlib

    # we need to make sure version.h is generated.

    versionTarget.target  = version.h
    versionTarget.depends = FORCE
    versionTarget.commands = powershell -noprofile -executionpolicy bypass -File ../../version.ps1 ../..

    PRE_TARGETDEPS += version.h
    QMAKE_EXTRA_TARGETS += versionTarget
}

QT += xml sql network widgets

include ( ../libs-targetfix.pro )

LIBS += -L../../external/libudfread -lmythudfread-$$LIBVERSION
LIBS += $$EXTRA_LIBS $$LATE_LIBS

test_clean.commands = -cd test/ && $(MAKE) -f Makefile clean
clean.depends = test_clean
QMAKE_EXTRA_TARGETS += test_clean clean
test_distclean.commands = -cd test/ && $(MAKE) -f Makefile distclean
distclean.depends = test_distclean
QMAKE_EXTRA_TARGETS += test_distclean distclean
