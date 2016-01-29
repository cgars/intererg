HEADER_PATH = ../include/relacs/intererg
include ${top_srcdir}/moc4.mk

pluginlibdir = $(pkglibdir)/plugins

pluginlib_LTLIBRARIES = \
    libinterergsimpleerg.la \
    libinterergledarray.la \
    libinterergpatch2led.la \
    libintererglightramp.la \
    libinterergtriggerdetector.la \
    libinterergofframp.la

CXXFLAGS += -lserial

libinterergsimpleerg_la_CPPFLAGS = \
    -I$(top_srcdir)/daq/include \
    -I$(top_srcdir)/numerics/include \
    -I$(top_srcdir)/options/include \
    -I$(top_srcdir)/datafile/include \
    -I$(top_srcdir)/plot/include \
    -I$(top_srcdir)/widgets/include \
    -I$(top_srcdir)/relacs/include \
    -I$(srcdir)/../include \
    $(QT4_CPPFLAGS)

libinterergsimpleerg_la_LDFLAGS = \
    -module -avoid-version \
    $(QT4_LDFLAGS)

libinterergsimpleerg_la_LIBADD = \
    $(top_builddir)/relacs/src/librelacs.la \
    $(QT4_LIBS)

$(libinterergsimpleerg_la_OBJECTS) : moc_simpleerg.cc

libinterergsimpleerg_la_SOURCES = simpleerg.cc

libinterergsimpleerg_la_includedir = $(pkgincludedir)/intererg

libinterergsimpleerg_la_include_HEADERS = $(HEADER_PATH)/simpleerg.h



libinterergledarray_la_CPPFLAGS = \
    -I$(top_srcdir)/daq/include \
    -I$(top_srcdir)/numerics/include \
    -I$(top_srcdir)/options/include \
    -I$(top_srcdir)/datafile/include \
    -I$(top_srcdir)/plot/include \
    -I$(top_srcdir)/widgets/include \
    -I$(top_srcdir)/relacs/include \
    -I$(srcdir)/../include \
     $(QT4_CPPFLAGS)

libinterergledarray_la_LDFLAGS = \
    -module -avoid-version \
    $(QT4_LDFLAGS)

libinterergledarray_la_LIBADD = \
    $(top_builddir)/relacs/src/librelacs.la \
    $(QT4_LIBS)

$(libinterergledarray_la_OBJECTS) : moc_ledarray.cc

libinterergledarray_la_SOURCES = ledarray.cc

libinterergledarray_la_includedir = $(pkgincludedir)/intererg

libinterergledarray_la_include_HEADERS = $(HEADER_PATH)/ledarray.h



libinterergpatch2led_la_CPPFLAGS = \
    -I$(top_srcdir)/daq/include \
    -I$(top_srcdir)/numerics/include \
    -I$(top_srcdir)/options/include \
    -I$(top_srcdir)/datafile/include \
    -I$(top_srcdir)/plot/include \
    -I$(top_srcdir)/widgets/include \
    -I$(top_srcdir)/relacs/include \
    -I$(srcdir)/../include \
    $(QT4_CPPFLAGS)

libinterergpatch2led_la_LDFLAGS = \
    -module -avoid-version \
    $(QT4_LDFLAGS)

libinterergpatch2led_la_LIBADD = \
    $(top_builddir)/relacs/src/librelacs.la \
    $(QT4_LIBS)

$(libinterergpatch2led_la_OBJECTS) : moc_patch2led.cc

libinterergpatch2led_la_SOURCES = patch2led.cc

libinterergpatch2led_la_includedir = $(pkgincludedir)/intererg

libinterergpatch2led_la_include_HEADERS = $(HEADER_PATH)/patch2led.h



libintererglightramp_la_CPPFLAGS = \
    -I$(top_srcdir)/daq/include \
    -I$(top_srcdir)/numerics/include \
    -I$(top_srcdir)/options/include \
    -I$(top_srcdir)/datafile/include \
    -I$(top_srcdir)/plot/include \
    -I$(top_srcdir)/widgets/include \
    -I$(top_srcdir)/relacs/include \
    -I$(srcdir)/../include \
    $(QT4_CPPFLAGS)

libintererglightramp_la_LDFLAGS = \
    -module -avoid-version \
    $(QT4_LDFLAGS)

libintererglightramp_la_LIBADD = \
    $(top_builddir)/relacs/src/librelacs.la \
    $(QT4_LIBS)

$(libintererglightramp_la_OBJECTS) : moc_lightramp.cc

libintererglightramp_la_SOURCES = lightramp.cc

libintererglightramp_la_includedir = $(pkgincludedir)/intererg

libintererglightramp_la_include_HEADERS = $(HEADER_PATH)/lightramp.h



libinterergtriggerdetector_la_CPPFLAGS = \
    -I$(top_srcdir)/daq/include \
    -I$(top_srcdir)/numerics/include \
    -I$(top_srcdir)/options/include \
    -I$(top_srcdir)/datafile/include \
    -I$(top_srcdir)/plot/include \
    -I$(top_srcdir)/widgets/include \
    -I$(top_srcdir)/relacs/include \
    -I$(srcdir)/../include \
    $(QT4_CPPFLAGS)

libinterergtriggerdetector_la_LDFLAGS = \
    -module -avoid-version \
    $(QT4_LDFLAGS)

libinterergtriggerdetector_la_LIBADD = \
    $(top_builddir)/relacs/src/librelacs.la \
    $(QT4_LIBS)

$(libinterergtriggerdetector_la_OBJECTS) : moc_triggerdetector.cc

libinterergtriggerdetector_la_SOURCES = triggerdetector.cc

libinterergtriggerdetector_la_includedir = $(pkgincludedir)/intererg

libinterergtriggerdetector_la_include_HEADERS = $(HEADER_PATH)/triggerdetector.h


libinterergofframp_la_CPPFLAGS = \
    -I$(top_srcdir)/daq/include \
    -I$(top_srcdir)/numerics/include \
    -I$(top_srcdir)/options/include \
    -I$(top_srcdir)/datafile/include \
    -I$(top_srcdir)/plot/include \
    -I$(top_srcdir)/widgets/include \
    -I$(top_srcdir)/relacs/include \
    -I$(srcdir)/../include \
    $(QT4_CPPFLAGS)

libinterergofframp_la_LDFLAGS = \
    -module -avoid-version \
    $(QT4_LDFLAGS)

libinterergofframp_la_LIBADD = \
    $(top_builddir)/relacs/src/librelacs.la \
    $(QT4_LIBS)

$(libinterergofframp_la_OBJECTS) : moc_offramp.cc

libinterergofframp_la_SOURCES = offramp.cc

libinterergofframp_la_includedir = $(pkgincludedir)/intererg

libinterergofframp_la_include_HEADERS = $(HEADER_PATH)/offramp.h



check_PROGRAMS = \
    linktest_libinterergsimpleerg_la \
    linktest_libinterergledarray_la \
    linktest_libinterergpatch2led_la \
    linktest_libintererglightramp_la \
    linktest_libinterergtriggerdetector_la \
    linktest_libinterergofframp_la

linktest_libinterergsimpleerg_la_SOURCES = linktest.cc
linktest_libinterergsimpleerg_la_LDADD = libinterergsimpleerg.la

linktest_libinterergledarray_la_SOURCES = linktest.cc
linktest_libinterergledarray_la_LDADD = libinterergledarray.la

linktest_libinterergpatch2led_la_SOURCES = linktest.cc
linktest_libinterergpatch2led_la_LDADD = libinterergpatch2led.la

linktest_libintererglightramp_la_SOURCES = linktest.cc
linktest_libintererglightramp_la_LDADD = libintererglightramp.la

linktest_libinterergtriggerdetector_la_SOURCES = linktest.cc
linktest_libinterergtriggerdetector_la_LDADD = libinterergtriggerdetector.la

linktest_libinterergofframp_la_SOURCES = linktest.cc
linktest_libinterergofframp_la_LDADD = libinterergofframp.la

TESTS = $(check_PROGRAMS)
