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
#include <json-glib/json-glib.h>

const gchar* json_small = "{\"username\": \"bob\", \"password\": \"qwe\", \"persist\": false}";
gchar* json_big;

void perf_s_big_obj()
{
  s_json_is_valid(json_big);

  const gchar* n1 = s_json_path(json_big, "$.statics.method[3].meta.static!boolean");
  g_assert(n1 != NULL);
}

void perf_g_big_obj()
{
  JsonParser *parser = json_parser_new();

  if (json_parser_load_from_data(parser, json_big, -1, NULL))
  {
    JsonNode* root = json_parser_get_root(parser);

    JsonNode* n1 = json_path_query("$.statics.method[3].meta.static", root, NULL);
    g_assert(JSON_NODE_HOLDS_ARRAY(n1));
    JsonArray *n1a = json_node_get_array(n1);
    JsonNode* n1_e = json_array_get_element(n1a, 0);
    g_assert(JSON_NODE_HOLDS_VALUE(n1_e));
  }

  g_object_unref(parser);
}

void perf_s_small_obj()
{
  if (s_json_is_valid(json_small))
  {
    const gchar* name = s_json_path(json_small, ".username!string");
    const gchar* pass = s_json_path(json_small, ".password!string");
    const gchar* persist = s_json_path(json_small, ".persist!boolean");

    if (name && pass && persist) 
    {
      gchar* name_str = s_json_get_string(name);
      gchar* pass_str = s_json_get_string(pass);
      gboolean persist_bool = s_json_get_bool(persist);

      if (!strcmp(name_str, "bob") && !strcmp(pass_str, "qwe"))
      {
        // logged in
      }
      else
      {
        g_assert_not_reached();
      }

      g_free(name_str);
      g_free(pass_str);
    }
    else
    {
      g_assert_not_reached();
    }
  }
  else
  {
    g_assert_not_reached();
  }
}

void perf_g_small_obj()
{
  JsonParser *parser = json_parser_new();

  if (json_parser_load_from_data(parser, json_small, -1, NULL))
  {
    JsonNode* root = json_parser_get_root(parser);
    if (JSON_NODE_HOLDS_OBJECT(root))
    {
      JsonObject* obj = json_node_get_object(root);

      const gchar* name = json_object_get_string_member(obj, "username");
      const gchar* pass = json_object_get_string_member(obj, "password");
      gboolean persist = json_object_get_boolean_member(obj, "persist");

      if (name && pass && !strcmp(name, "bob") && !strcmp(pass, "qwe"))
      {
        // logged in
      }
      else
      {
        g_assert_not_reached();
      }
    }
  }
  else
  {
    g_assert_not_reached();
  }

  g_object_unref(parser);
}

int main()
{
  gint i;
  GTimer* timer;

  g_assert(g_file_get_contents("big.json", &json_big, NULL, NULL));

  g_print("Small:\n");

  timer = g_timer_new();
  for (i = 0; i < 100000; i++)
    perf_s_small_obj();
  g_print("SJSON %g\n", g_timer_elapsed(timer, NULL));

  timer = g_timer_new();
  for (i = 0; i < 100000; i++)
    perf_g_small_obj();
  g_print("GJSON %g\n", g_timer_elapsed(timer, NULL));

  g_print("Big:\n");

  timer = g_timer_new();
  for (i = 0; i < 1000; i++)
    perf_s_big_obj();
  g_print("SJSON %g\n", g_timer_elapsed(timer, NULL));

  timer = g_timer_new();
  for (i = 0; i < 1000; i++)
    perf_g_big_obj();
  g_print("GJSON %g\n", g_timer_elapsed(timer, NULL));

  return 0;
}
