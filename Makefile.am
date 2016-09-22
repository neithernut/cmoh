#
# CORE INSTALLATION
#

# installation of the headers
#
# this is a bit cumbersome, since we cannot use nobase_ without having a
# Makefile.am in include/ --which we decided against.
cmohdir = $(includedir)/cmoh
cmoh_HEADERS = include/cmoh/*.hpp

cmoh_accessorsdir = $(cmohdir)/accessors/
cmoh_accessors_HEADERS = include/cmoh/accessors/*.hpp

cmoh_accessors_attributedir = $(cmohdir)/accessors/attribute/
cmoh_accessors_attribute_HEADERS = include/cmoh/accessors/attribute/*.hpp

cmoh_accessors_factorydir = $(cmohdir)/accessors/attribute/
cmoh_accessors_factory_HEADERS = include/cmoh/accessors/factory/*.hpp


# pkg-config file
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = cmoh.pc


#
# EXAMPLES
#
example_programs = \
	examples/attributes_example \
	examples/dynamic_key_example \
	examples/string_key_example

examples: $(example_programs)

# we also use examples as tests
TESTS = $(example_programs)
check_PROGRAMS = $(example_programs)

# now we declare how to build the examples:
AM_CPPFLAGS = -I$(top_srcdir)/include

examples_attributes_example_SOURCES = \
	examples/attributes_example.cpp \
	examples/person.cpp

examples_dynamic_key_example_SOURCES = \
	examples/dynamic_key_example.cpp \
	examples/person.cpp

examples_string_key_example_SOURCES = \
	examples/string_key_example.cpp \
	examples/person.cpp

