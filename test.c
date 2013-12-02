/**
 * sjson - fast string based JSON parser/generator library
 * Copyright (C) 2013  Ondřej Jirman <megous@megous.com>
 *
 * WWW: https://github.com/megous/sjson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "sjson.h"

void test_is_valid()
{
  // s_json_is_valid checks that string is a single JSON value, object, or
  // array

  // VALID

  // basic types
  g_assert(s_json_is_valid("null"));
  g_assert(s_json_is_valid("true"));
  g_assert(s_json_is_valid("false"));
  g_assert(s_json_is_valid("0"));
  g_assert(s_json_is_valid("1"));
  g_assert(s_json_is_valid("-10"));
  g_assert(s_json_is_valid("1.128879"));
  g_assert(s_json_is_valid("1.457e-10"));
  g_assert(s_json_is_valid("-1.457e+10"));
  g_assert(s_json_is_valid("1.457e10"));
  g_assert(s_json_is_valid("1e10"));
  g_assert(s_json_is_valid("  -1e+10"));
  g_assert(s_json_is_valid("  -1E+10"));
  g_assert(s_json_is_valid(" \"\" "));
  g_assert(s_json_is_valid("\"qweqwe\""));
  g_assert(s_json_is_valid("\"\\\\\\\"\\n\\r\\b\\f\\t\\u1234\""));
  g_assert(s_json_is_valid("\"Červený Kůň\""));

  // arrays
  g_assert(s_json_is_valid("[]"));
  g_assert(s_json_is_valid("[1]"));
  g_assert(s_json_is_valid("[1,2,3]"));
  g_assert(s_json_is_valid("[[1, [2], [[]]], 3]"));
  g_assert(s_json_is_valid("[[1, 2], 3]"));

  // objects
  g_assert(s_json_is_valid("{}"));
  g_assert(s_json_is_valid("{\"a\": 1}\n"));
  g_assert(s_json_is_valid("{\"a\": [1, 2, {}]}"));
  g_assert(s_json_is_valid("{\"a\": [1, 2,\n[{\t}]\n], \"b\": [1, 2, {}]}"));

  // INVALID

  // basic types
  g_assert(!s_json_is_valid(""));
  g_assert(!s_json_is_valid("  \n\t "));
  g_assert(!s_json_is_valid("nullnull"));
  g_assert(!s_json_is_valid("n"));
  g_assert(!s_json_is_valid(":"));
  g_assert(!s_json_is_valid(","));
  g_assert(!s_json_is_valid("truetrue"));
  g_assert(!s_json_is_valid("TRUE"));
  g_assert(!s_json_is_valid("FALSE"));
  g_assert(!s_json_is_valid("00"));
  g_assert(!s_json_is_valid("01"));
  g_assert(!s_json_is_valid("0 1"));
  g_assert(!s_json_is_valid(".1"));
  g_assert(!s_json_is_valid("1e-"));
  g_assert(!s_json_is_valid("."));
  g_assert(!s_json_is_valid("\""));
  g_assert(!s_json_is_valid("\"\"\"\""));
  g_assert(!s_json_is_valid("\"\"\""));
  g_assert(!s_json_is_valid("a:\"\""));
  g_assert(!s_json_is_valid("\"a\":\"\""));

  // arrays
  g_assert(!s_json_is_valid("["));
  g_assert(!s_json_is_valid("]"));
  g_assert(!s_json_is_valid("[,"));
  g_assert(!s_json_is_valid("[0"));
  g_assert(!s_json_is_valid("[0,"));
  g_assert(!s_json_is_valid("[0,"));
  g_assert(!s_json_is_valid("[0,1"));
  g_assert(!s_json_is_valid("[,0]"));
  g_assert(!s_json_is_valid("[]0"));
  g_assert(!s_json_is_valid("[[]0]"));
  g_assert(!s_json_is_valid("[00]"));

  // objects
  g_assert(!s_json_is_valid("{"));
  g_assert(!s_json_is_valid("}"));
  g_assert(!s_json_is_valid("{\"a\":}"));
  g_assert(!s_json_is_valid("{\"a\"}"));
  g_assert(!s_json_is_valid("{\"a\":1}1"));
  g_assert(!s_json_is_valid("{\"a\":1, \"x\": }"));
  g_assert(!s_json_is_valid("{\"a\":1, \"x\": }}"));
  g_assert(!s_json_is_valid("{1:1}"));
  g_assert(!s_json_is_valid("{\"a\":1 1}"));
}

void test_get_type()
{
  // s_json_get_type returns type of an based on the first token found in
  // a string (it doesn't and shouldn't validate)

  g_assert_cmpint(s_json_get_type(""),              ==, S_JSON_TYPE_NONE);
  g_assert_cmpint(s_json_get_type("  \t\n\r"),      ==, S_JSON_TYPE_NONE);
  g_assert_cmpint(s_json_get_type("{}"),            ==, S_JSON_TYPE_OBJECT);
  g_assert_cmpint(s_json_get_type("[]"),            ==, S_JSON_TYPE_ARRAY);
  g_assert_cmpint(s_json_get_type("{"),             ==, S_JSON_TYPE_OBJECT);
  g_assert_cmpint(s_json_get_type("["),             ==, S_JSON_TYPE_ARRAY);
  g_assert_cmpint(s_json_get_type("\"\""),          ==, S_JSON_TYPE_STRING);
  g_assert_cmpint(s_json_get_type("\"abc\""),       ==, S_JSON_TYPE_STRING);
  g_assert_cmpint(s_json_get_type("\"a\\u0030b\""), ==, S_JSON_TYPE_STRING);
  g_assert_cmpint(s_json_get_type("\"\\n\\\"\""),   ==, S_JSON_TYPE_STRING);
  g_assert_cmpint(s_json_get_type("0"),             ==, S_JSON_TYPE_NUMBER);
  g_assert_cmpint(s_json_get_type("1"),             ==, S_JSON_TYPE_NUMBER);
  g_assert_cmpint(s_json_get_type("123"),           ==, S_JSON_TYPE_NUMBER);
  g_assert_cmpint(s_json_get_type("-123"),          ==, S_JSON_TYPE_NUMBER);
  g_assert_cmpint(s_json_get_type("1.02"),          ==, S_JSON_TYPE_NUMBER);
  g_assert_cmpint(s_json_get_type("0.01e10"),       ==, S_JSON_TYPE_NUMBER);
  g_assert_cmpint(s_json_get_type("0.01e+10"),      ==, S_JSON_TYPE_NUMBER);
  g_assert_cmpint(s_json_get_type("0.01e-5"),       ==, S_JSON_TYPE_NUMBER);
  g_assert_cmpint(s_json_get_type("-0.01e-5"),      ==, S_JSON_TYPE_NUMBER);
  g_assert_cmpint(s_json_get_type("-1e-5"),         ==, S_JSON_TYPE_NUMBER);
  g_assert_cmpint(s_json_get_type("true"),          ==, S_JSON_TYPE_BOOL);
  g_assert_cmpint(s_json_get_type("false"),         ==, S_JSON_TYPE_BOOL);
  g_assert_cmpint(s_json_get_type("null"),          ==, S_JSON_TYPE_NULL);
  g_assert_cmpint(s_json_get_type("blah"),          ==, S_JSON_TYPE_INVALID);
  g_assert_cmpint(s_json_get_type("\""),            ==, S_JSON_TYPE_INVALID);
  g_assert_cmpint(s_json_get_type("\"xx"),          ==, S_JSON_TYPE_INVALID);
  g_assert_cmpint(s_json_get_type(":"),             ==, S_JSON_TYPE_INVALID);
  g_assert_cmpint(s_json_get_type(","),             ==, S_JSON_TYPE_INVALID);
  g_assert_cmpint(s_json_get_type("."),             ==, S_JSON_TYPE_INVALID);
}

void test_getters_simple()
{
  // VALID

  g_assert_cmpstr(s_json_get_string("\"\""), ==, "");
  g_assert_cmpstr(s_json_get_string("\"abc\""), ==, "abc");
  g_assert_cmpstr(s_json_get_string("\"\n\""), ==, "\n");
  g_assert_cmpstr(s_json_get_string("\"\\n\\\"\""), ==, "\n\"");
  g_assert_cmpstr(s_json_get_string("\"\\u017d\\u0041\""), ==, "ŽA");
  g_assert_cmpstr(s_json_get_string("\"\\b\\f\\n\\r\\t\\/\\\\\\\"\""), ==, "\b\f\n\r\t/\\\"");
  g_assert_cmpstr(s_json_get_string(s_json_build("%s", "\b\f\n\r\t/\\\"")), ==, "\b\f\n\r\t/\\\"");

  g_assert_cmpint(s_json_get_int("0", -1), ==, 0);
  g_assert_cmpint(s_json_get_int("1", -1), ==, 1);
  g_assert_cmpint(s_json_get_int("100", -1), ==, 100);
  g_assert_cmpint(s_json_get_int("-100", -1), ==, -100);
  g_assert_cmpint(s_json_get_int("100000000000000", -1), ==, 100000000000000l);

  g_assert_cmpint(s_json_get_double("1", -1), ==, 1);
  g_assert_cmpint(s_json_get_double("1.1", -1), ==, 1.1);
  g_assert_cmpint(s_json_get_double("1.1e10", -1), ==, 1.1e10);
  g_assert_cmpint(s_json_get_double("1.1e+10", -1), ==, 1.1e10);
  g_assert_cmpint(s_json_get_double("1.1e-10", -1), ==, 1.1e-10);
  g_assert_cmpint(s_json_get_double("-1.1e10", -1), ==, -1.1e10);

  g_assert(s_json_get_bool("true"));
  g_assert(!s_json_get_bool("false"));

  g_assert(s_json_is_null("null"));

  g_assert_cmpstr(s_json_get("{} {}"), ==, "{}");

  // INVALID OR CORNER CASES

  g_assert(!s_json_is_null("false"));
  g_assert(!s_json_get_bool("null"));
  g_assert(!s_json_get_bool(""));
  g_assert_cmpint(s_json_get_double("bad", -1), ==, -1);
  g_assert_cmpint(s_json_get_int("bad", -1), ==, -1);
  g_assert_cmpstr(s_json_get_string("bad"), ==, NULL);
  g_assert_cmpstr(s_json_get("{"), ==, NULL);
}

void test_getters_array()
{
  const gchar* json_arr = "[\"\", \"abc\"]";
  const gchar *iter, *value, *key;

  // VALID

  // simple use

  iter = s_json_get_element_first(json_arr);
  g_assert_cmpstr(s_json_get_string(iter), ==, "");
  iter = s_json_get_element_next(iter);
  g_assert_cmpstr(s_json_get_string(iter), ==, "abc");
  iter = s_json_get_element_next(iter);
  g_assert(iter == NULL);

  // s_json_get_element
  g_assert_cmpstr(s_json_get_string(s_json_get_element(json_arr, 0)), ==, "");
  g_assert_cmpstr(s_json_get_string(s_json_get_element(json_arr, 1)), ==, "abc");
  g_assert_cmpstr(s_json_get_element(json_arr, 2), ==, NULL);

  // s_json_get_elements
  gchar** els = s_json_get_elements(json_arr);
  g_assert_cmpint(g_strv_length(els), ==, 2);
  g_assert_cmpstr(s_json_get_string(els[0]), ==, "");
  g_assert_cmpstr(s_json_get_string(els[1]), ==, "abc");

  // s_json_get_elements on empty array
  gchar** els_empty = s_json_get_elements("[]");
  g_assert(els_empty != NULL);
  g_assert(els_empty[0] == NULL);

  // INVALID

  // s_json_get_element on invalid json
  //g_assert_cmpstr(s_json_get_element("bad", 0), ==, NULL);

  // s_json_get_elements on invalid string is undefined
  //g_assert(s_json_get_elements("bad") == NULL);
  //g_assert(s_json_get_elements("[") == NULL);
  //g_assert(s_json_get_elements("[1,]") == NULL);
  //g_assert(s_json_get_elements("[2,3 4]") == NULL);
}

void test_getters_object()
{
  const gchar* json = "{\"a\\n\": \"p\", \"ssl\": 1, \"f\": [1, 5, 7], \"b\": true, \"n\": null}";
  const gchar* key, *value;

  // VALID

  g_assert(s_json_is_valid(json));

  key = s_json_get_member_first(json, &value);
  g_assert_cmpstr(s_json_get_string(key), ==, "a\n");
  g_assert_cmpstr(s_json_get_string(value), ==, "p");
  key = s_json_get_member_next(&value);
  g_assert_cmpstr(s_json_get_string(key), ==, "ssl");
  g_assert_cmpint(s_json_get_int(value, 0), ==, 1);
  key = s_json_get_member_next(&value);
  g_assert_cmpstr(s_json_get_string(key), ==, "f");
  g_assert_cmpint(s_json_get_type(value), ==, S_JSON_TYPE_ARRAY);
  key = s_json_get_member_next(&value);
  g_assert_cmpstr(s_json_get_string(key), ==, "b");
  g_assert_cmpint(s_json_get_type(value), ==, S_JSON_TYPE_BOOL);
  key = s_json_get_member_next(&value);
  g_assert_cmpstr(s_json_get_string(key), ==, "n");
  g_assert_cmpint(s_json_get_type(value), ==, S_JSON_TYPE_NULL);
  key = s_json_get_member_next(&value);
  g_assert(key == NULL);

  g_assert_cmpint(s_json_get_type(s_json_get_member(json, "a\n")), ==, S_JSON_TYPE_STRING);
  g_assert_cmpint(s_json_get_type(s_json_get_member(json, "f")), ==, S_JSON_TYPE_ARRAY);

  g_assert_cmpstr(s_json_get_member_string(json, "a\n"), ==, "p");
  g_assert_cmpint(s_json_get_member_int(json, "ssl", -1), ==, 1);
  g_assert_cmpint(s_json_get_member_double(json, "ssl", -1), ==, 1);
  g_assert(s_json_get_member_bool(json, "b"));
  g_assert(!s_json_member_is_null(json, "b"));
  g_assert(s_json_member_is_null(json, "n")); // null member reported as null
  g_assert(s_json_member_is_null(json, "bad")); // non-existing member reported as null

  S_JSON_FOREACH_MEMBER(json, k, v)
    if (s_json_string_match(k, "a\n"))
      g_assert_cmpint(s_json_get_type(v), ==, S_JSON_TYPE_STRING);
  S_JSON_FOREACH_END()

  // INVALID

  g_assert(s_json_get_member(json, "bad") == NULL);
  g_assert(s_json_get_member("{", "a") == NULL);
  g_assert(s_json_get_member("{\"a\"", "a") == NULL);
  g_assert(s_json_get_member("{\"a\" 1", "a") == NULL);
  g_assert(s_json_get_member("{\"b\": {", "a") == NULL);
  g_assert(s_json_get_member("{2: 1", "a") == NULL);
  g_assert(s_json_get_member("bad", "a") == NULL);
  //g_assert(s_json_get_member("{\"a\":", "a") == NULL);
  //g_assert(s_json_get_member("{\"a\",", "a") == NULL);
  //g_assert(s_json_get_member("{\"a\": ,", "a") == NULL);
  //g_assert(s_json_get_member("{\"a\": 7,", "b") == NULL);
  g_assert_cmpint(s_json_get_member_int(json, "bad", -1), ==, -1);
  g_assert_cmpint(s_json_get_member_double(json, "a", -1), ==, -1);
  g_assert_cmpint(s_json_get_member_double(json, "bad", -1), ==, -1);
  g_assert_cmpstr(s_json_get_member_string(json, "bad"), ==, NULL);
  g_assert(!s_json_get_member_bool(json, "bad"));
}

void test_path()
{
  const gchar* json = "[{\"b\": true, \"n\": null, \"i\": 1, \"d\":1.2, \"s\": \"str\", \"o\": {\"x\": []}, \"a\": [1]}]";
  struct {
    gchar* path;
    gchar* json;
  } checks[] = {
    // valid paths without type checks
    { "$[0].i", "1" },
    { "$[0].d", "1.2" },
    { "$[0].s", "\"str\"" },
    { "$[0].a", "[1]" },
    { "$[0].a[0]", "1" },
    { "$[0].n", "null" },
    { "$[0].b", "true" },
    { "$[0].o", "{\"x\": []}" },
    { "$[0].o.x", "[]" },
    { "$[0].o.x[0]", NULL},
    // type checked valid
    { "$[0].i!i", "1" },
    { "$[0].d!n", "1.2" },
    { "$[0].s!s", "\"str\"" },
    { "$[0].o!o", "{\"x\": []}" },
    { "$[0].a!a", "[1]" },
    { "$[0].n?o", "null" },
    { "$[0].b!b", "true" },
    // type checked invalid
    { "$[0].d!i", NULL},
    { "$[0].s!i", NULL},
    { "$[0].b!n", NULL},
    { "$[0].d!s", NULL},
    { "$[0].a!o", NULL},
    { "$[0].o!a", NULL},
    { "$[0].a?o", NULL},
    { "$[0].s!b", NULL},
    // type checked optional valid
    { "$[0].n?i", "null"},
    { "$[0].n?n", "null"},
    { "$[0].n?s", "null"},
    { "$[0].n?o", "null"},
    { "$[0].n?a", "null"},
    { "$[0].n?b", "null"},
    // type checked optional valid undefined
    { "$[0].z?i", "null"},
    { "$[0].z?n", "null"},
    { "$[0].z?s", "null"},
    { "$[0].z?o", "null"},
    { "$[0].z?a", "null"},
    { "$[0].z?b", "null"},
    // type checked optional invalid
    { "$[0].b?i", NULL},
    { "$[0].b?n", NULL},
    { "$[0].b?s", NULL},
    { "$[0].b?o", NULL},
    { "$[0].b?a", NULL},
    { "$[0].s?b", NULL},
    // ignore whitespace
    { " $[0] .i ", "1" },

    // wrong use
    { "!.x",  NULL},
    { "$[1].b?b",  NULL},
    { "$[0][0]",  NULL},
    { "^^^",  NULL},
  };

  gint i;
  for (i = 0; i < G_N_ELEMENTS(checks); i++)
  {
    const gchar* res = s_json_path(json, checks[i].path);
    gchar* from_path = res ? s_json_get(res) : NULL;

    g_assert_cmpstr(from_path, ==, checks[i].json);

    g_free(from_path);
  }
}

void test_builder()
{
  // VALID

  g_assert_cmpstr("10", ==, s_json_build("%i", (gint64)10));
  g_assert_cmpstr("-10", ==, s_json_build("%i", (gint64)-10));
  g_assert_cmpstr("10", ==, s_json_build("%d", (gdouble)10));
  g_assert_cmpstr("1.1e+10", ==, s_json_build("%d", (gdouble)1.1e10));
  g_assert_cmpstr("true", ==, s_json_build("%b", TRUE));
  g_assert_cmpstr("false", ==, s_json_build("%b", FALSE));
  g_assert_cmpstr("\"10\"", ==, s_json_build("%s", "10"));
  g_assert_cmpstr("\"10\"", ==, s_json_build("%j", "\"10\""));
  g_assert_cmpstr("\"10\"", ==, s_json_build("%S", g_strdup("10")));
  g_assert_cmpstr("\"10\"", ==, s_json_build("%J", g_strdup("\"10\"")));
  g_assert_cmpstr("null", ==, s_json_build("%s", NULL));
  g_assert_cmpstr("null", ==, s_json_build("%j", NULL));
  g_assert_cmpstr("null", ==, s_json_build("%S", NULL));
  g_assert_cmpstr("null", ==, s_json_build("%J", NULL));

  // use of unquoted strings for member names
  g_assert_cmpstr("{\"a\":1}", ==, s_json_build("{a:%i}", (gint64)1));
  g_assert_cmpstr("{\"-pre\":1}", ==, s_json_build("{-pre:%i}", (gint64)1));

  // complex options combo
  g_assert_cmpstr("[\"\\\"\", 10, 1.1, \"freed\", null, true]", ==, s_json_build("[%s, %i, %d, %S, %?s, %b]", "\"", (gint64)10, (gdouble)1.1, g_strdup("freed"), TRUE, "qwe", TRUE));

  // ignore % in strings
  g_assert_cmpstr("[\"%i\"]", ==, s_json_build("[\"%i\"]"));
  g_assert_cmpstr("[\"%\"]", ==, s_json_build("[\"%\"]"));
  g_assert_cmpstr("[\"%?\"]", ==, s_json_build("[\"%?\"]"));
  g_assert_cmpstr("[\"%%?\"]", ==, s_json_build("[\"%%?\"]"));

  // null flag
  g_assert_cmpstr("null", ==, s_json_build("%?i", TRUE, (gint64)10));
  g_assert_cmpstr("10", ==, s_json_build("%?i", FALSE, (gint64)10));
  g_assert_cmpstr("null", ==, s_json_build("%?s", TRUE, "str"));
  g_assert_cmpstr("\"str\"", ==, s_json_build("%?s", FALSE, "str"));
  g_assert_cmpstr("null", ==, s_json_build("%?S", TRUE, g_strdup("str")));
  g_assert_cmpstr("\"str\"", ==, s_json_build("%?S", FALSE, g_strdup("str")));

  // unquoted names/strings
  g_assert_cmpstr("{\"a\":\"b\"}", ==, s_json_build("{a:b}"));
  g_assert_cmpstr("{\"a\":\"nullify\"}", ==, s_json_build("{a:nullify}"));
  g_assert_cmpstr("{\"a\":\"trueify\"}", ==, s_json_build("{a:trueify}"));
  g_assert_cmpstr("{\"a\":\"falseify\"}", ==, s_json_build("{a:falseify}"));
  g_assert_cmpstr("{\"a\":false}", ==, s_json_build("{a:false}"));
  g_assert_cmpstr("{\"a\":true}", ==, s_json_build("{a:true}"));
  g_assert_cmpstr("{\"a\":null}", ==, s_json_build("{a:null}"));

  // INVALID

  // s_json_build output is automatically validated
  g_assert_cmpstr(NULL, ==, s_json_build("{%s}", NULL));
  g_assert_cmpstr(NULL, ==, s_json_build("^%&#$", NULL));
}

void test_generator()
{
  SJsonGen* g = s_json_gen_new();

  // VALID

  s_json_gen_start_array(g);

    s_json_gen_start_object(g);
      s_json_gen_member_null(g, "n");
      s_json_gen_member_bool(g, "b", TRUE);
      s_json_gen_member_int(g, "i", 1);
      s_json_gen_member_double(g, "d", 1.1);
      s_json_gen_member_string(g, "es", "\r\n\t\b\f\"\\");
      s_json_gen_member_string(g, "s", "a");
      s_json_gen_member_build(g, "build", "{a: %s}", "build");
      s_json_gen_member_json(g, "json", "[1, 2, 3]");

      s_json_gen_member_array(g, "a");
      s_json_gen_end_array(g);

      s_json_gen_member_object(g, "o");
      s_json_gen_end_object(g);
    s_json_gen_end_object(g);

    s_json_gen_null(g);
    s_json_gen_bool(g, TRUE);
    s_json_gen_int(g, 1);
    s_json_gen_double(g, 1.1);
    s_json_gen_string(g, "\r\n\t\b\f\"\\");
    s_json_gen_string(g, "a");
    s_json_gen_build(g, "{a: %s}", "build");
    s_json_gen_json(g, "[1, 2, 3]");
    s_json_gen_json(g, NULL);
    s_json_gen_string(g, NULL);

  s_json_gen_end_array(g);

  gchar* g_str = s_json_gen_done(g);

  g_assert(s_json_is_valid(g_str));
  g_assert_cmpstr(g_str, ==, "[{\"n\":null,\"b\":true,\"i\":1,\"d\":1.1,\"es\":\"\\r\\n\\t\\b\\f\\\"\\\\\",\"s\":\"a\",\"build\":{\"a\": \"build\"},\"json\":[1, 2, 3],\"a\":[],\"o\":{}},null,true,1,1.1,\"\\r\\n\\t\\b\\f\\\"\\\\\",\"a\",{\"a\": \"build\"},[1, 2, 3],null,null]");

  g_free(g_str);

  // INVALID

  // output should be vaildated
  g = s_json_gen_new();
  s_json_gen_start_object(g);
  s_json_gen_end_array(g);
  g_str = s_json_gen_done(g);

  g_assert(g_str == NULL);
}

void test_formatters()
{
  const gchar* json = "\n\n[{\"a\": 1}, {\"b\":\n 2},\t {\"c\": [{}]}, 1], {} ";
  const gchar* json_compact = "[{\"a\":1},{\"b\":2},{\"c\":[{}]},1]";
  const gchar* json_pretty = "[{\n\t\"a\": 1\n}, {\n\t\"b\": 2\n}, {\n\t\"c\": [{}]\n},\n1\n]";

  // VALID

  g_assert_cmpstr(json_compact, ==, s_json_compact(json));
  g_assert_cmpstr(s_json_pretty(json_compact), ==, s_json_pretty(json));
  g_assert_cmpstr(s_json_pretty(json), ==, json_pretty);

  // INVALID

  g_assert_cmpstr(NULL, ==, s_json_pretty("{"));
  g_assert_cmpstr(NULL, ==, s_json_pretty("^"));
  g_assert_cmpstr(NULL, ==, s_json_compact("{"));
  g_assert_cmpstr(NULL, ==, s_json_compact("^"));
}

void test_util()
{
  g_assert(s_json_string_match("\"\"", ""));
  g_assert(s_json_string_match("\"a\"", "a"));
  g_assert(s_json_string_match("\"abc\"", "abc"));
  g_assert(s_json_string_match("\"\\n\"", "\n"));
  g_assert(s_json_string_match("\"\\u0030\"", "0"));

  g_assert(!s_json_string_match("\"\"", " "));
  g_assert(!s_json_string_match("\" \"", ""));
  g_assert(!s_json_string_match("\"a\"", "a "));
  g_assert(!s_json_string_match("\"a \"", "a"));
  g_assert(!s_json_string_match("\"a\"", "b"));
  g_assert(!s_json_string_match("\"\\n\"", "\\n"));
}

int main(int argc, char **argv)
{
  g_test_init(&argc, &argv, NULL);

  g_test_add_func("/is-valid", test_is_valid);
  g_test_add_func("/get-type", test_get_type);
  g_test_add_func("/getters-simple", test_getters_simple);
  g_test_add_func("/getters-array", test_getters_array);
  g_test_add_func("/getters-object", test_getters_object);
  g_test_add_func("/path", test_path);
  g_test_add_func("/builder", test_builder);
  g_test_add_func("/generator", test_generator);
  g_test_add_func("/formatters", test_formatters);
  g_test_add_func("/util", test_util);

  return g_test_run();
}
