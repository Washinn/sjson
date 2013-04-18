Fast JSON parser/generator that doesn't do any memory 
allocations, because it works directly with JSON strings.

This has several advantages:

  - Type simplicity, everything is a string
  - There is no separate parsing step, nor any need for
    intermediate data structures/json value tree
  - No memory allocations are done on the heap
  - Easy access to complex JSON structures, via JSON Path
  - The code is made to be simple to use from C programs
  - For many use cases, JSON string may not need to
    be parsed whole at all. Sjson will parse only as much
    of the input as is necessary to get the information you
    requested

The code works with several assumtpions. CPU time is cheap, 
memory allocations are not. Avoiding heap allocated intermediate 
JSON tree representation means better performance.

Typical use case is to receive a JSON string, extract some data 
from it, and throw it away. Sjson can do this job 10x faster 
than json-glib or PHP's json parser, with much less memory usage.

In addition, sjson is well unit tested, and its code is of 
high quality.

Sjson is very well suited for performance sensitive applications.

Performance
-----------

For comparison with json-glib:

  - 10x times faster parsing (for equivalent code)
  - 10000x times less memory used (or something like that)


License
-------

sjson library is covered by GPL


Author
------

Ondřej Jirman <megous@megous.com>, 2013
