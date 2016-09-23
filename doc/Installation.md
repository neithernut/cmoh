Installation
============

Our build/installation setup is based on the autotools, primarily consisting of
[autoconf](https://www.gnu.org/software/autoconf/) and
[automake](https://www.gnu.org/software/automake/).
These tools generate a (portable) `configure` script and a `Makefile` template.
Depending on how you obtained your copy of CMOH, you may be required to generate
those files using an autotools installation or you may proceed using the
`configure` script.

In any case, `make`, `sh` and a number of smaller programs are required for
installation. However, most unixoid system (virtually every system except MS
Windows) should have those installed by default.


Installing CMOH using autotools
-------------------------------

If you obtained CMOH without a pre-generated `configure` script, run

    aclocal
    autoconf
    automake --add-missing

on a machine on which the autotools are installed. If the procedure was
successful, the project's root directory will contain a `configure` script.
From here, proceed with the installation using that script.


Installing CMOH using an existing `configure` script
----------------------------------------------------

If you already have a `configure` script, you can proceed by running

    ./configure
    make install

in the project's root directory.

As the name suggests, `configure` can be used for configuring some details
about the build/installation process. This is done by passing additional
parameters. Run `./configure --help` for a list of possible configurations.

The build-directory can be cleaned by running `make clean`. Running
`make distclean` also removes the generated `Makefile`, which might be of
interest if you want to run `configure` with another set of parameters.


Generating a tarball for distribution
-------------------------------------

Run

    make dist

to generate a tarball containing the CMOH sources along with some build,
prerequisites (most notably a `configure` script and a `Makefile` template).
That tarball can then be re-distributed, e.g. to machines without an autotools
installation.


Additional stuff
----------------

Once you ran `configure`, you can also build the examples (`make examples`) and
the documentation (make `pdf`). Note that [pandoc](http://pandoc.org/) is
required for building the documentation.


Testing
-------

The build system comes with some generic sanity checks. Also the examples do
serve as tests. Run

    make check

to run the tests.


