Motivation
==========

In this section, I give my personal opinion and motivation for this project. If
you don't want to read my rambling, you may skip it without missing anything
important.

In the last couple of years, I have seen many frameworks and libraries which
either serialize or parse data or provide some kind of RMI. None of those
solutions satisfied me. In the following paragraphs, I describe some of the
usual approaches and explain why I don't like them.


Code generation
---------------

First of all, I'm not particularly fond of code generation. Generated code is
often opaque, hard to debug. Code generators are essentially yet another
dependency, which you have to deal with (also in your build environment), albeit
that dependency is usually fulfilled by the library/framework requiring it.
I also have to admit that there are indeed cases where code generation is the
right thing to do: when a formal, language independent specification exists,
which must be fulfilled (e.g. protobuf). However, the generated code should be
as unintrusive and small in size as possible.

Round trip engineering is, in my opinion, a fulsome solution to the wrong
problem most of the time. The mantra of model driven development is that
generating repetitive code is faster and less error prone than writing  (read
C&P'ing)it by hand every time, which undoubtedly is true. However, it is my
personal opinion that, if you have to write much repetitive code, either you are
doing it wrong, the API/framework author is doing it wrong or you are simply
using the wrong programming language (read: you are doing it wrong).

In that particular category, much personal hatred goes towards the eclipse
foundation, which, as it seems, doesn't even consider the use-case of using
their tools outside an over-bloated IDE. But then again, I avoid Java whenever I
can, anyway.


Manual transformation from/to generic data-model
------------------------------------------------

Some libraries (especially XML or JSON libraries) just present you a
super-dynamic data-structure mirroring a generic AST of the language and require
you to get/set the data yourself manually and take care of every single error
case conceivable. You end up writing lots of repetitive and potentially buggy or
outright broken code, which, as explained above, should be avoided.

Some frameworks generate code for (de)serialization and require you to feed it
every single piece of data manually. This may cause you to depend on a specific
version of a specific code generator and, again, requires the user to write a
lot of repetitive code.


Intrusive solutions
-------------------

Then there are frameworks that either require you to add a super class to the
thing you want to serialize or make use of code-annotations and compiler-plugins
or, again, code-generation. Those frameworks are far too intrusive IMHO.
Consider a class from a 3rd party library, which you cannot trivially extend.
You cannot simply extend that 3rd party library with arbitrary annotations.
Hence, you cannot use such a framework in such a setup, if you don't write
wrappers for every single thing, including keeping them up to date every time
anything changes. Even when you control the classes, you drag in dependencies on
some framework in a place where it is simply unnecessary. Business classes
should not concern themselves with the way they are written to disk or accessed
across the network. In some cases, you don't even /know/ how they should be
serialized.

Much of my personal disdain goes towards the Qt framework, which indeed is one
of the prime examples of overblown, over-marketed, intrusive frameworks which
force you to do everything their way, at least in my opinion. This is in part
because they throw away type safety by doing virtually everything late-binding
in a hyper-dynamical way, so your code ends up working together more
incidentally than naturally. They also infamously have many parts which have
analogous to (classic) STL containers and techniques which are, of course, often
incompatible.  At this point I should note that many of Qt's design points have
made sense at some point, when C++ lacked a lot of those things --back in the
90s.

With their `QObject` hierarchy, they require you to inherit from `QObject`, add
annotations for attributes /and/ they make heavy usage of code-generation/custom
preprocessors.  They also let you pass own classes via some of their subsystems
using their meta-object system, but it requires you to make the type known
generally via a macro and, additionally, to the specific subsystem. This is
somewhat acceptable. However, this does not let you access members of the things
you pass in a generic way. That you have to provide via another way.


Requiring a default constructor
-------------------------------

I once had the "pleasure" to work with Qt D-Bus in conjunction with an horrible
abomination of an interface specification written by three guys working for a
three-lettered German car manufacturer, which seemingly either had no motivation
or no idea of how an usable interface looks like. Of this "specification" came
in the form of a PDF which was clearly converted from a Microsoft Word document
containing cluttered diagrams drawn with Enterprise Architect. Industry
standard.

Apart from the unusable debugging provided by that Qt addon, there was no usable
way of enforcing a specific signature (of individual signal parameters) when
passing a message over the bus. Messages were written out using a custom output
stream. Boundaries of containers or arrays were written or read using special
output manipulators. That bit was actually somewhat acceptable. However, the
system performed some checks on the signature on receiving a message, for which
the signature of all the arguments of a method were needed. Those were not
specified explicitly but (again, dynamically) extracted from a sample argument
which was default-constructed for this purpose.

In my opinion, requiring a default constructor for deserialization is wrong.
Consider for example, a database containing entries of some kind, each tagged
with a UUID. The right thing to do is to set the UUID upon construction of the
object, either with a freshly generated one or, if the object is loaded from
some source, with an existing UUID. You don't want to set it after the
construction. You may not even be trivially able to.


Conclusion
----------

What I always wanted was an unitrusive library which does not impose
dependencies on the data type, is not entirely based on preprocessors, code
generation or compiler-plugins and is type-safe.

I'm an C++-guy. I like to fiddle with templates. So I started this project.
Hopefully this will end up better than the solutions I ranted about.


