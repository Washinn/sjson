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

Features
--------

  - JSON string validator
  - Basic JSON Path like interface with type checking
  - Direct element/member access for arrays and objects
  - Iterator interface for arrays and objects
  - Prettyfier/compaction formatting functions
  - Format string based JSON builder
  - Iterative JSON string generator


Usage Examples
--------------

```json


{
  "first": "John",
  "last": "Doe",
  "age": 39,
  "sex": "M",
  "salary": 70000,
  "registered": true,
  "interests": [
    "Reading",
    "Mountain Biking",
    "Hacking"
  ],
  "favorites": {
    "color": "Blue",
    "sport": "Soccer",
    "food": "Spaghetti"
  },
  "skills": [
    {
      "category": "Javascript",
      "tests": [
        {
          "name": "One",
          "score": 90
        },
        {
          "name": "Two",
          "score": 96
        }
      ]
    },
    {
      "category": "CouchDB",
      "tests": [
        {
          "name": "One",
          "score": 79
        },
        {
          "name": "Two",
          "score": 84
        }
      ]
    },
    {
      "category": "Node.js",
      "tests": [
        {
          "name": "One",
          "score": 97
        },
        {
          "name": "Two",
          "score": 93
        }
      ]
    }
  ]
}
```

Above JSON string can be processed in various ways by sjson:

```c
const gchar* json = ABOVE_STRING;

if (s_json_is_valid(json))
{
  // JSON path
  const gchar* favs = s_json_path("$.favorites?object");
  const gchar* ints = s_json_path("$.interests?array");
  const gchar* category_of_the_first_skill = s_json_path("$.skills[0].category");

  // Name
  gchar* first = s_json_get_member_string(json, "first");
  gchar* last = s_json_get_member_string(json, "last");

  // Iterative interfaces
  S_JSON_FOREACH_ELEMENT(ints, el)
    if (s_json_get_type(el) == S_JSON_TYPE_STRING)
      g_print("%s\n", s_json_get_string(el));
  S_JSON_FOREACH_END()

  S_JSON_FOREACH_MEMBER(favs, key, value)
    if (s_json_get_type(value) == S_JSON_TYPE_STRING)
      g_print("%s: %s\n", s_json_get_string(key), s_json_get_string(value));
  S_JSON_FOREACH_END()

  // Formatting
  g_print("Pretty: %s\n", s_json_pretty(json));
  g_print("Compact: %s\n", s_json_compact(json));
}

// Building new JSON strings

gchar* json1 = s_json_build("{a:%s}", "test string that will be escaped\n");
gchar* json2 = s_json_build("{a:%J, b:%i}", s_json_build("{a:1, c:[%d]}", (gdouble)1e-5), (gint64)5000);

SJsonGen* gen = s_json_gen_new();
s_json_gen_start_object(gen);
  s_json_gen_member_string(gen, "username", "bob");
  s_json_gen_member_string(gen, "password", "qwe");
  s_json_gen_member_build(gen, "meta", "{is_admin: %b, is_disabled: %b}", TRUE, FALSE);
  s_json_gen_member_array(gen, "roles");
    s_json_gen_string(gen, "role1");
    s_json_gen_string(gen, "role2");
    s_json_gen_string(gen, "role3");
  s_json_gen_end_array(gen);
s_json_gen_end_object(gen);
gchar* json3 = s_json_gen_done(gen);
```


Performance
-----------

For comparison with json-glib, there's a bench program in the tree, that
demonstrates:

  - 10x-50x times faster parsing times (for equivalent operation on JSON data)
  - no heap memory use for intermediate storage

Operations on small JSON objects:

  - sjson 0.045831 s
  - json-glib 0.533595 s

Operations on large JSON objects (130 kB):

  - sjson 0.157557 s
  - json-glib 5.10273 s


License
-------

sjson library is covered by GPL


Author
------

Ondřej Jirman <megous@megous.com>, 2013
