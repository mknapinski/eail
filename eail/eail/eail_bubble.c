/*
 * Copyright (c) 2013 Samsung Electronics Co., Ltd.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

/**
 * @file eail_bubble.c
 * @brief EailBubble implementation
 */

#include <Elementary.h>

#include "eail_bubble.h"
#include "eail_utils.h"

static void atk_action_interface_init(AtkActionIface *iface);
static void atk_text_interface_init(AtkTextIface *iface);

/**
 * @brief Definition of EailBubble type
 */
G_DEFINE_TYPE_WITH_CODE(EailBubble,
                        eail_bubble,
                        EAIL_TYPE_TEXT,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_TEXT,
                                              atk_text_interface_init)
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_ACTION,
                                              atk_action_interface_init));

/*
 * Implementation of the *AtkObject* interface
 */

/**
 * @brief EailBubble object initialization
 *
 * @param obj EailBubble instance
 * @param data user set additional initialization data
 */
static void
eail_bubble_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_bubble_parent_class)->initialize(obj, data);

   obj->role = ATK_ROLE_FILLER;
}

/**
 * @brief Class destructor
 *
 * @param object GObject instance
 */
static void
eail_bubble_finalize(GObject *object)
{
   EailBubble *bubble = EAIL_BUBBLE(object);

   if (bubble->click_description) free(bubble->click_description);

   G_OBJECT_CLASS(eail_bubble_parent_class)->finalize(object);
}

/**
 * @brief EailBubble instance initialization
 *
 * @param bubble EailBubble instance
 */
static void
eail_bubble_init(EailBubble *bubble)
{
   bubble->click_description = NULL;
}

/**
 * @brief Gets the list of widget's accessible children
 *
 * @param widget EailWidget instance
 *
 * @return Eina_List representing the list of accessible children
 * or NULL if widget has no children
 */
static Eina_List *
eail_bubble_get_widget_children(EailWidget *widget)
{
   Eina_List *list = NULL;
   Evas_Object *child, *obj;

   obj = eail_widget_get_widget(EAIL_WIDGET(widget));

   if (obj)
     {
        child = elm_object_part_content_get(obj, "default");
        if (child && elm_object_widget_check(child))
          list = eina_list_append(list, child);

        child = elm_object_part_content_get(obj, "icon");
        if (child && elm_object_widget_check(child))
          list = eina_list_append(list, child);
     }

   return list;
}

/**
 * @brief GObject type initialization function
 *
 * @param klass EailBubbleClass instance
 */
static void
eail_bubble_class_init(EailBubbleClass *klass)
{
   AtkObjectClass *atk_class = ATK_OBJECT_CLASS(klass);
   EailWidgetClass *widget_class = EAIL_WIDGET_CLASS(klass);
   GObjectClass *gobject_class = G_OBJECT_CLASS(klass);

   atk_class->initialize = eail_bubble_initialize;
   widget_class->get_widget_children = eail_bubble_get_widget_children;
   gobject_class->finalize = eail_bubble_finalize;
}

/*
 * Implementation of the *AtkAction* interface
 */

/**
 * @brief Gets the number of accessible actions available on the object
 *
 * Implementation of get_n_actions from AtkAction interface.
 *
 * @param action EailBubble instance
 *
 * @returns integer representing the number of available actions
 */
static int
eail_bubble_n_actions_get(AtkAction *action)
{
   return 1;
}

/**
 * @brief Gets the description string of the specified action
 *
 * Implementation of get_description from AtkAction interface.
 *
 * @param action EailBubble instance
 * @param i action index
 *
 * @return string representing the specified action's description
 */
static const char*
eail_bubble_description_get(AtkAction *action,
                            gint i)
{
   EailBubble *bubble;
   const char *action_description;

   bubble = EAIL_BUBBLE(action);
   if (!bubble) return NULL;

   switch (i)
     {
      case 0:
         action_description = bubble->click_description;
         break;
      default:
         action_description = NULL;
         break;
     }

   return action_description;
}

/**
 * @brief Sets a description of the specified action of the object
 *
 * Implementation of set_description from AtkAction interface.
 *
 * @param action AtkAction instance
 * @param i action index
 * @param description action description
 *
 * @return TRUE on success, FALSE otherwise
 */
static gboolean
eail_bubble_description_set(AtkAction *action,
                            gint i,
                            const char *description)
{
   EailBubble *bubble;
   char **value;

   bubble = EAIL_BUBBLE(action);
   if (!bubble) return FALSE;

   switch (i)
     {
      case 0:
         value = &bubble->click_description;
         break;
      default:
         value = NULL;
         break;
     }

   if (value)
     {
        free(*value);
        *value = g_strdup(description);
        return TRUE;
     }

   return FALSE;
}

/**
 * @brief Gets the name string of the specified action
 *
 * Implementation of get_name from AtkAction interface.
 *
 * @param action AtkAction instance
 * @param i action index
 *
 * @return string representing the specified action's name
 */
static const char*
eail_bubble_action_name_get(AtkAction *action,
                            int i)
{
   const char* action_name;

   switch (i)
     {
      case 0:
         action_name = "click";
         break;
      default:
         action_name = NULL;
         break;
     }

   return action_name;
}

/**
 * @brief Performs the specified action on the object
 *
 * Implementation of do_action from AtkAction interface.
 *
 * @param action AtkAction instance
 * @param i action index
 *
 * @return TRUE on success, FALSE otherwise
 */
static gboolean
eail_bubble_do_action(AtkAction *action,
                      int i)
{
   Evas_Object *widget;

   widget = eail_widget_get_widget(EAIL_WIDGET(action));
   if (!widget) return FALSE;

   if ((elm_object_disabled_get(widget)) || (!evas_object_visible_get(widget)))
     return FALSE;

   const char *action_name = atk_action_get_name(action, i);
   if (!action_name) return FALSE;

   evas_object_smart_callback_call(widget, "clicked", NULL);

   return TRUE;
}

/**
 * @brief AtkAction interface initializer
 *
 * @param iface action interface to be filled
 **/
static void
atk_action_interface_init(AtkActionIface *iface)
{
   g_return_if_fail(iface != NULL);

   iface->get_n_actions   = eail_bubble_n_actions_get;
   iface->get_description = eail_bubble_description_get;
   iface->set_description = eail_bubble_description_set;
   iface->get_name        = eail_bubble_action_name_get;
   iface->do_action       = eail_bubble_do_action;
}


/**
 * @brief Helper func to get Textblock form bubble widget
 *
 * @param text AtkText instance
 * @returns Textblock part of bubble widget
 */
static const Evas_Object *
_eail_get_textblock(AtkText *text)
{
   Evas_Object *widget= NULL;
   const Evas_Object *textblock = NULL;
   Evas_Object *label = NULL;
   Evas_Object *label_edje_layer = NULL;
   Evas_Object *bubble_edje_layer = NULL;

   widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget) return NULL;

   bubble_edje_layer = elm_layout_edje_get(widget);
   if (!bubble_edje_layer) return NULL;

   label= edje_object_part_swallow_get(bubble_edje_layer, "elm.swallow.content");
   if (!label) return NULL;

   label_edje_layer = elm_layout_edje_get(label);
   if (!label_edje_layer) return NULL;

   textblock = edje_object_part_object_get(label_edje_layer, "elm.text");
   if (!textblock) return NULL;

   return textblock;
}

/**
 * @brief Gets text bounded by start_offset and end_offset
 *
 * Use g_free() to free the returned string
 *
 * @param text AtkText instance
 * @param start_offset start position
 * @param end_offset end position, -1 for the end of the string
 * @return string containing text from start_offset up to, but not including
 * end_offset
 */
static gchar*
eail_bubble_get_text(AtkText   *text,
                     gint       start_offset,
                     gint       end_offset)
{
   gchar *string = NULL;
   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(text));
   Evas_Object *label = NULL;

   if (!widget)
      return NULL;
   label = elm_object_content_get(widget);

   if (label)
     string = (gchar *)elm_object_text_get(label);

   return eail_get_substring(string, start_offset, end_offset);
}

/**
 * @brief Gets the character at offset
 *
 * @param text AtkText instance
 * @param offset character offset
 * @return char representing the character at offset
 */
static gunichar
eail_bubble_get_character_at_offset(AtkText    *text,
                                  gint        offset)
{
   gunichar character = '\0';
   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(text));
   Evas_Object *label = NULL;

   if (!widget)
      return character;

   label = elm_object_content_get(widget);

   if (label)
     character = g_utf8_get_char(
         g_utf8_offset_to_pointer(elm_object_text_get(label), offset));

   return character;
}

/**
 * @brief Gets the text's length
 *
 * @param text AtkText instance
 * @return integer representing the text length
 */
static gint
eail_bubble_get_character_count(AtkText *text)
{
   gint count = 0;
   const gchar *string_text = NULL;

   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(text));
   Evas_Object *label = NULL;

   if (!widget) return count;

   label = elm_object_content_get(widget);
   if (!label) return count;

   string_text = elm_object_text_get(label);
   if (!string_text) return count;

   count = g_utf8_strlen(string_text, -1);

   return count;
}


/**
 * @brief Get the bounding box containing the glyph
 *  representing the character at a particular text offset.
 *
 * @param text AtkText instance
 * @param offset The offset of the text character for which
 * bounding information is required.
 * @param x Pointer for the x cordinate of the bounding box
 * @param y Pointer for the y cordinate of the bounding box
 * @param width Pointer for the width of the bounding box
 * @param height Pointer for the height of the bounding box
 * @param coords specify whether coordinates are relative to the
 * screen or widget window
 *
 */
static void
eail_bubble_get_character_extents(AtkText *text,
                                  gint offset,
                                  gint *x,
                                  gint *y,
                                  gint *width,
                                  gint *height,
                                  AtkCoordType coords)
{
   int result = -1;
   const Evas_Object *textblock = NULL;
   Evas_Textblock_Cursor *cur = NULL;

   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget) return;

   textblock = _eail_get_textblock(text);
   if (!textblock) return;

   cur = evas_object_textblock_cursor_new(textblock);
   if (!cur) return;

   evas_textblock_cursor_pos_set(cur, offset);

   result = evas_textblock_cursor_char_geometry_get(cur, x, y, width, height);

   evas_textblock_cursor_free(cur);

   if (-1 == result) return;

   if (coords == ATK_XY_SCREEN)
   {
      int ee_x, ee_y;
      Ecore_Evas *ee= ecore_evas_ecore_evas_get(evas_object_evas_get(widget));

      ecore_evas_geometry_get(ee, &ee_x, &ee_y, NULL, NULL);
      *x += ee_x;
      *y += ee_y;
    }
}

/**
 * @brief Gets the specified text after offset
 *
 * Use g_free() to free the returned string.
 *
 * @param text AtkText instance
 * @param offset character offset
 * @param boundary_type AtkTextBoundary instance
 * @param [out] start_offset start offset of the returned string
 * @param [out] end_offset offset of the first character after the returned
 * substring
 * @returns newly allocated string containing the text after offset bounded
 * by the specified boundary_type
 */
static gchar *
eail_bubble_get_text_after_offset(AtkText *text,
                                  gint offset,
                                  AtkTextBoundary boundary_type,
                                  gint *start_offset,
                                  gint *end_offset)
{
   const Evas_Object *textblock;

   textblock = _eail_get_textblock(text);
   if (!textblock) return NULL;

   return eail_get_text_after(textblock, offset, boundary_type, start_offset,
                              end_offset);

}

/**
 * @brief Gets the specified text at offset
 *
 * Use g_free() to free the returned string.
 *
 * @param text AtkText instance
 * @param offset character offset
 * @param boundary_type AtkTextBoundary instance
 * @param [out] start_offset start offset of the returned string
 * @param [out] end_offset offset of the first character after the returned
 * substring
 * @returns newly allocated string containing the text after offset bounded
 * by the specified boundary_type
 */
static gchar *
eail_bubble_get_text_at_offset(AtkText *text,
                               gint offset,
                               AtkTextBoundary boundary_type,
                               gint *start_offset,
                               gint *end_offset)
{
   const Evas_Object *textblock;

   textblock = _eail_get_textblock(text);
   if (!textblock) return NULL;

   return eail_get_text_at(textblock, offset, boundary_type, start_offset,
                           end_offset);
}

/**
 * @brief Gets the specified text before offset
 *
 * Use g_free() to free the returned string.
 *
 * @param text AtkText instance
 * @param offset character offset
 * @param boundary_type AtkTextBoundary instance
 * @param [out] start_offset start offset of the returned string
 * @param [out] end_offset offset of the first character after the returned
 * substring
 * @returns newly allocated string containing the text after offset bounded
 * by the specified boundary_type
 */
static gchar *
eail_bubble_get_text_before_offset(AtkText *text,
                                   gint offset,
                                   AtkTextBoundary boundary_type,
                                   gint *start_offset,
                                   gint *end_offset)
{
   const Evas_Object *textblock;

   textblock = _eail_get_textblock(text);
   if (!textblock) return NULL;

   return eail_get_text_before(textblock, offset, boundary_type, start_offset,
                               end_offset);
}

/**
 * @brief Gets the offset of the character located at coordinates x and y.
 *  x and y are interpreted as being relative to the screen or this
 *  widget's window depending on coords.
 *
 * @param text AtkText instance
 * @param screen x-position of character
 * @param screen y-position of character
 * @param specify whether coordinates are relative to the screen or widget window
 *
 * @returns the offset to the character which is located at the specified x and y coordinates.
 */
static gint
eail_bubble_get_offset_at_point(AtkText *text,
                                gint x,
                                gint y,
                                AtkCoordType coords)
{
   int result = -1;
   const Evas_Object *textblock = NULL;
   Evas_Textblock_Cursor *cur = NULL;

   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(text));

   if (!widget) return -1;

   textblock = _eail_get_textblock(text);
   if (!textblock) return -1;

   cur = evas_object_textblock_cursor_new(textblock);

   if (coords == ATK_XY_SCREEN)
   {
      int ee_x, ee_y;
      Ecore_Evas *ee= ecore_evas_ecore_evas_get(evas_object_evas_get(widget));
      ecore_evas_geometry_get(ee, &ee_x, &ee_y, NULL, NULL);
      x -= ee_x;
      y -= ee_y;
    }

   evas_textblock_cursor_char_coord_set( cur, x, y);
   result = evas_textblock_cursor_pos_get( cur );
   evas_textblock_cursor_free(cur);

   return result;
}


/**
 * @brief Initializes AtkTextIface interface
 *
 * @param iface AtkTextIface instance
 */
static void
atk_text_interface_init(AtkTextIface *iface)
{
   iface->get_text = eail_bubble_get_text;
   iface->get_character_at_offset = eail_bubble_get_character_at_offset;
   iface->get_character_count = eail_bubble_get_character_count;
   iface->get_character_extents = eail_bubble_get_character_extents;
   iface->get_text_after_offset = eail_bubble_get_text_after_offset;
   iface->get_text_at_offset = eail_bubble_get_text_at_offset;
   iface->get_text_before_offset = eail_bubble_get_text_before_offset;
   iface->get_offset_at_point = eail_bubble_get_offset_at_point;
}
