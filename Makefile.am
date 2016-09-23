#
# CORE INSTALLATION
#

# installation of the headers
#
# this is a bit cumbersome, since we cannot use nobase_ without having a
# Makefile.am in include/ --which we decided against.
cmoh_includes =

cmohdir = $(includedir)/cmoh
cmoh_HEADERS = include/cmoh/*.hpp
cmoh_includes += $(cmoh_HEADERS)

cmoh_accessorsdir = $(cmohdir)/accessors/
cmoh_accessors_HEADERS = include/cmoh/accessors/*.hpp
cmoh_includes += $(cmoh_accessors_HEADERS)

cmoh_accessors_attributedir = $(cmohdir)/accessors/attribute/
cmoh_accessors_attribute_HEADERS = include/cmoh/accessors/attribute/*.hpp
cmoh_includes += $(cmoh_accessors_attribute_HEADERS)

cmoh_accessors_factorydir = $(cmohdir)/accessors/attribute/
cmoh_accessors_factory_HEADERS = include/cmoh/accessors/factory/*.hpp
cmoh_includes += $(cmoh_accessors_factory_HEADERS)


# pkg-config file
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = cmoh.pc


#
# DOCUMENTATION
#

# the files making up the documentation, in the order they are meant to be read
documentation_files = \
	doc/Introduction.md \
	doc/Motivation.md \
	doc/Usage.md \
	doc/Properties.md \
	doc/Factories.md \
	doc/AccessorBundle.md \
	doc/Utilities.md \
	doc/Installation.md \
	doc/Licensing.md \
	doc/metadata.yaml

userdoc.pdf: $(documentation_files)
	$(PANDOC) -o $@ --toc --chapters $^

pdf: userdoc.pdf


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


#
# DEPENDENCY TESTS
#
cmoh_smoke_test_files = $(addsuffix .smoke,$(wildcard $(cmoh_includes)))
check_PROGRAMS += test/smoke
test_smoke_DEPENDENCIES = $(cmoh_smoke_test_files)

# hack to make the compiler look at each header file
$(cmoh_smoke_test_files): %.smoke: %
	$(CXXCOMPILE) -c -o $@ -x c++ - < $<


#
# MISC
#

# cleaning
MOSTLYCLEANFILES = userdoc.pdf
CLEANFILES = $(cmoh_smoke_test_files)


# things to distribute in a source tarball
EXTRA_DIST = \
	$(documentation_files) \
	Changelog.md \
	LICENSE \
	README.md \
	doc/README.md \
	examples/README.md

