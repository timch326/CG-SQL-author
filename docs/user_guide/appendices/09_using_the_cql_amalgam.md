---
title: "Appendix 9: Using the CQL Amalgam"
weight: 9
---
<!---
-- Copyright (c) Meta Platforms, Inc. and affiliates.
--
-- This source code is licensed under the MIT license found in the
-- LICENSE file in the root directory of this source tree.
-->

This is a brief discussion of the CQL Amalgam and its normal usage patterns.

### What is an Amalgam?  What is it for?

The amalgam is not much more than a concatenation of all of the `.h` files and the `.c` files into one big `cql_amalgam.c` file.
With that files you can trivially make the compiler with a single `cc cql_amalgam.c` command.

Because of this simplicity, the amalgam gives you a convenient way to consume the compiler in a different/unique build
environment and, if desired, strip it down to just the parts you want while controlling its inputs more precisely than
you can with just a command line tool.

If you want to snapshot a particular CQL compiler the easiest way to do so is to use make an amalgam
at that version and then check in the output `cql_amalgam.c`.  Just like with SQLite and its amalgam.

There are many other uses of the amalgam:
* It's possible to tweak the amalgam with a little pre-processing to make a windows binary, last time I attempted this it took about 10 minutes
  * trying to get the whole build to work on Windows is a lot harder
* The amalgam is readily consumed by [emscripten](https://en.wikipedia.org/wiki/Emscripten) to create WASM
  * This along with Lua and SQLite in WASM resulted in [a fully online playground](https://mingodad.github.io/CG-SQL-Lua-playground/)
  * Meta builds a VSCode extension that hosts the actual compiler in WASM in VSCode for error checking
  * You can make any kind of tool of your own that wants to consume the AST or the output parts
  * You can invoke the CQL compiler without launching a new process from inside your environment

Generally the amalgam makes it easier for you to host the CQL compiler in some new environment.

### Creating the Amalgam

The amalgam has to include the results of bison and flex, so a normal build must run first.  The simplest
way to build it starting from the `sources` directory is:

```bash
make
./make_amalgam.sh
```

The result goes in `out/cql_amalgam.c`.  It can then be built using `cc` with whatever flags you might
desire.  With a few `-D` directives it can readily be compiled with Microsoft C and it also works with
Emscripten (`emcc`) basically unchanged.  Clang and Gcc of course also work.

The standard test script `test.sh` builds the amalgam and attempts to compile it as well, which ensures
that the amalgam can at least compile at all times.

### Testing the Amalgam

Of course you can do whatever tests you might like by simply compiling the amalgam as is and then using
it to compile things.  But importantly the test script `test.sh` can test the amalgam build like so:

```bash
test.sh --use_amalgam
```

This runs all the normal tests using the binary built from the amalgam rather than the normal binary.

Normal CQL development practices result in this happening pretty often so the amalgam tends to stay
in good shape. The code largely works in either form with very few affordances for the amalgam build needed.
Most developers don't even think about the amalgam build flavor; to a first approximation "it just works".

### Using the Amalgam

To use the amalgam you'll want to do something like this:

```c
#define CQL_IS_NOT_MAIN 1

// Suppresses a bunch of warnings because the code
// is in an #include context
// PR's to remove these are welcome :D
#pragma clang diagnostic ignored "-Wnullability-completeness"

#include "cql_amalgam.c"

void go_for_it(const char *your_buffer) {
  YY_BUFFER_STATE my_string_buffer = yy_scan_string(your_buffer);

  // Note: "--in" is irrelevant because the scanner is
  // going to read from the buffer above.
  //
  // If you don't use yy_scan_string, you could use "--in"
  // to get data from a file.

  int argc = 4;
  char *argv[] = { "cql", "--cg", "foo.h", "foo.c" };

  cql_main(argc, argv);
  yy_delete_buffer(my_string_buffer);
}
```

So the general pattern is:

* predefine the options you want to use (see below)
* include the amalgam
* add any functions you want that will call the amalgam

Most amalgam functions are `static` to avoid name conflicts. You will want to create your own public functions such as `go_for_it` above that use the amalgam in all the ways you desire.

You'll want to avoid calling any internal functions other than `cql_main` because they are liable to change.

>NOTE: The amalgam is C code not C++ code.  Do not attempt to use it inside of an `extern "C"` block in a C++ file.  It won't build.  If you want a C++ API, expose the C functions you need and write a wrapper class.

### CQL Amalgam Options

The amalgam includes the following useful `#ifdef` options to allow you to customize it.

* CQL_IS_NOT_MAIN
* CQL_NO_SYSTEM_HEADERS
* CQL_NO_DIAGNOSTIC_BLOCK
* cql_emit_error
* cql_emit_output
* cql_open_file_for_write
* cql_write_file

#### CQL_IS_NOT_MAIN

If this symbol is defined then `cql_main` will not be redefined to be `main`.

As the comments in the source say:

```c
#ifndef CQL_IS_NOT_MAIN

// Normally CQL is the main entry point.  If you are using CQL
// in an embedded fashion then you want to invoke its main at
// some other time. If you define CQL_IS_NOT_MAIN then cql_main
// is not renamed to main.  You call cql_main when you want.

  #define cql_main main
#endif
```

Set this symbol so that you own main and cql_main is called at your pleasure.

#### CQL_NO_SYSTEM_HEADERS

The amalgam includes the normal `#include` directives needed to make it compile, things like stdio and such.
In your situation these headers may not be appropriate.  If `CQL_NO_SYSTEM_HEADERS` is defined then the amalgam
will not include anything; you can then add whatever headers you need before you include the amalgam.


#### CQL_NO_DIAGNOSTIC_BLOCK

The amalgam includes a set of recommended directives for warnings to suppress and include.  If you want
to make other choices for these you can suppress the defaults by defining `CQL_NO_DIAGNOSTIC_BLOCK`;
you can then add whatever diagnostic pragmas you want/need.

#### cql_emit_error

The amalgam uses `cql_emit_error` to write its messages to stderr.  The documentation is included in the
code which is attached here.  If you want the error messages to go somewhere else, define `cql_emit_error`
as the name of your error handling function.  It should accept a `const char *` and record that string
however you deem appropriate.

```c
#ifndef cql_emit_error

// CQL "stderr" outputs are emitted with this API.
//
// You can define it to be a method of your choice with
// "#define cql_emit_error your_method" and then your method
// will get the data instead. This will be whatever output the
// compiler would have emitted to stderr.  This includes
// semantic errors or invalid argument combinations.  Note that
// CQL never emits error fragments with this API; you always
// get all the text of one error.  This is important if you
// are filtering or looking for particular errors in a test
// harness or some such.
//
// You must copy the memory if you intend to keep it. "data" will
// be freed.
//
// Note: you may use cql_cleanup_and_exit to force a failure from
// within this API but doing so might result in unexpected cleanup
// paths that have not been tested.

void cql_emit_error(const char *err) {
  fprintf(stderr, "%s", err);
  if (error_capture) {
    bprintf(error_capture, "%s", err);
  }
}

#endif
```

Typically you would `#define cql_emit_error your_error_function` before you include the amalgam and then
define your_error_function elsewhere in that file (before or after the amalgam is included are both fine).

#### cql_emit_output

The amalgam uses `cql_emit_output` to write its messages to stdout.  The documentation is included in the
code which is attached here.  If you want the standard output to go somewhere else, define `cql_emit_output`
as the name of your output handling function.  It should accept a `const char *` and record that string
however you deem appropriate.

```c
#ifndef cql_emit_output

// CQL "stdout" outputs are emitted (in arbitrarily small pieces)
// with this API.
//
// You can define it to be a method of your choice with
// "#define cql_emit_output your_method" and then your method will
// get the data instead. This will be whatever output the
// compiler would have emitted to stdout.  This is usually
// reformated CQL or semantic trees and such -- not the normal
// compiler output.
//
// You must copy the memory if you intend to keep it. "data" will
// be freed.
//
// Note: you may use cql_cleanup_and_exit to force a failure from
// within this API but doing so might result in unexpected cleanup
// paths that have not been tested.

void cql_emit_output(const char *msg) {
  printf("%s", msg);
}

#endif
```

Typically you would `#define cql_emit_output your_output_function` before you include the amalgam and then
define your_error_function elsewhere in that file (before or after the amalgam is included are both fine).

#### cql_open_file_for_write

If you still want normal file i/o for your output but you simply want to control the placement of the output
(such as forcing it to be on some virtual drive) you can replace this function by defining `cql_open_file_for_write`.

If all you need to do is control the origin of the `FILE *` that is written to, you can replace just this function.

```c
#ifndef cql_open_file_for_write

// Not a normal integration point, the normal thing to do is
// replace cql_write_file but if all you need to do is adjust
// the path or something like that you could replace
// this method instead.  This presumes that a FILE * is still ok
// for your scenario.

FILE *_Nonnull cql_open_file_for_write(
  const char *_Nonnull file_name)
{
  FILE *file;
  if (!(file = fopen(file_name, "w"))) {
    cql_error("unable to open %s for write\n", file_name);
    cql_cleanup_and_exit(1);
  }
  return file;
}

#endif
```

Typically you would `#define cql_open_file_for_write your_open_function` before you include the amalgam and then
define your_open_function elsewhere in that file (before or after the amalgam is included are both fine).

#### cql_write_file

The amalgam uses `cql_write_file` to write its compilation outputs to the file system.  The documentation is included in the
code which is attached here.  If you want the compilation output to go somewhere else, define `cql_write_file`
as the name of your output handling function.  It should accept a `const char *` for the file name and another
for the data to be written.  You can then store those compilation results however you deem appropriate.

```c
#ifndef cql_write_file

// CQL code generation outputs are emitted in one "gulp" with this
// API. You can define it to be a method of your choice with
// "#define cql_write_file your_method" and then your method will
// get the filename and the data. This will be whatever output the
// compiler would have emitted to one of it's --cg arguments.
// You can then write it to a location of your choice.
// You must copy the memory if you intend to keep it. "data" will
// be freed.

// Note: you *may* use cql_cleanup_and_exit to force a failure
// from within this API.  That's a normal failure mode that is
// well-tested.

void cql_write_file(
  const char *_Nonnull file_name,
  const char *_Nonnull data)
{
  FILE *file = cql_open_file_for_write(file_name);
  fprintf(file, "%s", data);
  fclose(file);
}

#endif
```

Typically you would `#define cql_write_file your_write_function` before you include the amalgam and then
define your_write_function elsewhere in that file (before or after the amalgam is included are both fine).

### Amalgam LEAN choices

When you include the amalgam, you get everything by default. You may, however, only want some
limited subset of the compiler's functions in your build.

To customize the amalgam, there are a set of configuration pre-processor options.  To opt-in to
configuration, first define `CQL_AMALGAM_LEAN`. You then have to opt-in to the various pieces
you might want. The system is useless without the parser, so you can't remove that; but you can
choose from the list below.

The options are:
* CQL_AMALGAM_LEAN` : enable lean mode; this must be set or you get everything
* `CQL_AMALGAM_CG_C` : C codegen
* `CQL_AMALGAM_CG_COMMON` : common code generator pieces
* `CQL_AMALGAM_GEN_SQL` : the echoing features
* `CQL_AMALGAM_JSON` : JSON schema output
* `CQL_AMALGAM_OBJC` : Objective-C code gen
* `CQL_AMALGAM_QUERY_PLAN` : the query plan creator
* `CQL_AMALGAM_SCHEMA` : the assorted schema output types
* `CQL_AMALGAM_SEM` : semantic analysis (needed by most things)
* `CQL_AMALGAM_TEST_HELPERS` : test helper output
* `CQL_AMALGAM_UNIT_TESTS` : some internal unit tests, which are pretty much needed by nobody

Note that `CQL_AMALGAM_SEM` is necessary for any of the code generation
features to work. Likewise, several generators require `CQL_AMALGAM_CG_COMMON` (e.g., C does).

Pick what you want; stubs are created for what you omit to avoid linkage errors.

### Other Notes

The amalgam will use malloc/calloc for its allocations and it is designed to release all memory it
has allocated when cql_main returns control to you, even in the face of error.

Internal compilation errors result in an `assert` failure leading to an abort.  This is not supposed
to ever happen but there can always be bugs.  Normal errors just prevent later phases of the compiler
from running so you might not see file output, but rather just error output.  In all cases things
should be cleaned up.

The compiler can be called repeatedly with no troubles; it re-initializes on each use. The compiler is
 not multi-threaded so if there is threading you should use some mutex arrangement to keep it safe.
A thread-safe version would require extensive modifications.
