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
 * @file eail_label.c
 * @brief EailLabel implementation.
 */

#include <Elementary.h>

#include "eail_label.h"
#include "eail_utils.h"

static void atk_text_interface_init(AtkTextIface *iface);

/**
 * @brief EailLabel type definition
 */
G_DEFINE_TYPE_WITH_CODE(EailLabel,
                        eail_label,
                        EAIL_TYPE_WIDGET,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_TEXT,
                                              atk_text_interface_init));

/**
 * @brief EailLabel initializer.
 *
 * @param obj EailLabel instance
 * @param data initialization data
 */
static void
eail_label_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_label_parent_class)->initialize(obj, data);
   obj->role = ATK_ROLE_LABEL;
}

/**
 * @brief EailLabel interface intializer.
 *
 * @param label EailLabel instance
 */
static void
eail_label_init(EailLabel *label)
{
}

/**
 * @brief Gets the state set of the accessible.
 *
 * The caller must unreference it when it is no longer needed.
 *
 * @param accessible AtkObject instance
 * @return AtkStateSet containing the state set of the accessible
 */
static AtkStateSet*
eail_label_ref_state_set(AtkObject *accessible)
{
   AtkStateSet *state_set;
   Evas_Object *widget;

   state_set =
       ATK_OBJECT_CLASS(eail_label_parent_class)->ref_state_set(accessible);

   widget = eail_widget_get_widget(EAIL_WIDGET(accessible));
   if (!widget) return state_set;

   atk_state_set_add_state(state_set, ATK_STATE_MULTI_LINE);

   return state_set;
}

/**
 * @brief Gets obj's name
 *
 * @param obj AtkObject instance
 * @returns string containing obj's name or its text content if the name is not set
 */
static const gchar*
eail_label_get_name(AtkObject *obj)
{
   const gchar *name;
   Evas_Object *widget;

   name = ATK_OBJECT_CLASS(eail_label_parent_class)->get_name(obj);
   if (NULL != name)
     return name;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return NULL;

   name = (const gchar*)elm_object_text_get(widget);
   if (!name) return NULL;

   return name;
}

/**
 * @brief EailLabel class initializer.
 *
 * @param klass EailLabelClass instance
 */
static void
eail_label_class_init(EailLabelClass *klass)
{
   AtkObjectClass *atk_class = ATK_OBJECT_CLASS(klass);

   atk_class->initialize = eail_label_initialize;
   atk_class->ref_state_set = eail_label_ref_state_set;
   atk_class->get_name = eail_label_get_name;
}

/**
 * @brief Gets text bounded by start_offset and end_offset
 *
 * Use g_free() to free the returned string
 *
 * @param text an AtkText
 * @param start_offset start position
 * @param end_offset end position, -1 for the end of the string
 * @return string containing text from start_offset up to, but not including
 * end_offset
 */
static gchar*
eail_label_get_text(AtkText   *text,
                    gint       start_offset,
                    gint       end_offset)
{
   gchar *string = NULL;
   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(text));

   if (widget)
     string = (gchar *)elm_object_text_get(widget);

   if (!string)
     return NULL;

   return eail_get_substring(string, start_offset, end_offset);
}

/**
 * @brief Gets the character at offset
 *
 * @param text AtkText instance
 * @param offset character offset
 * @return char located at the given offset
 */
static gunichar
eail_label_get_character_at_offset(AtkText    *text,
                                   gint        offset)
{
   gunichar character = '\0';
   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(text));

   if (widget)
     character = g_utf8_get_char(
         g_utf8_offset_to_pointer(elm_object_text_get(widget), offset));

   return character;
}

/**
 * @brief Gets the length of text
 *
 * @param text AtkText instance
 * @return integer representing the length of text
 */
static gint
eail_label_get_character_count(AtkText *text)
{
   Evas_Object *widget;
   const char *str;

   widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget)
     return 0;

   str = elm_object_text_get(widget);
   if (!str)
     return 0;
   return g_utf8_strlen(str, -1);
}

/**
 * @brief Creates an AtkAttributeSet which consists of the default values of
 * attributes for the text.
 *
 * This AtkAttributeSet should be freed by a call to
 * atk_attribute_set_free()
 *
 * @param text AtkText instance
 *
 * @returns AtkAttributeSet containing default values of attributes
 * at offset.
 */
static AtkAttributeSet *
eail_label_get_default_attributes(AtkText *text)
{
   AtkAttributeSet *at_set = NULL;

   at_set = eail_utils_text_add_attribute
       (at_set, ATK_TEXT_ATTR_WRAP_MODE,
        atk_text_attribute_get_value(ATK_TEXT_ATTR_WRAP_MODE, 0));

   at_set = eail_utils_text_add_attribute
       (at_set, ATK_TEXT_ATTR_EDITABLE,
        atk_text_attribute_get_value
         (ATK_TEXT_ATTR_EDITABLE, FALSE));

   return at_set;
}

/**
 * @brief Creates an AtkAttributeSet which consists of the attributes
 * explicitly set at the position offset in the text.
 *
 * start_offset and end_offset are set to the start and end of the range around offset
 * where the attributes are invariant.
 *
 * Note that end_offset is the offset of the first character after the range.
 *
 * This AtkAttributeSet should be freed by a call to
 * atk_attribute_set_free()
 *
 * @param text AtkText instance
 * @param offset the offset at which to get the attributes
 * @param [out] start_offset start offset of the range
 * @param [out] end_offset end offset of the range
 *
 * @returns an AtkAttributeSet which contains the attributes explicitly set at
 * offset.
 */
static AtkAttributeSet *
eail_label_get_run_attributes(AtkText *text,
                              gint offset,
                              gint *start_offset,
                              gint *end_offset)
{
   AtkAttributeSet *at_set = NULL;
   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(text));
   gint len = eail_label_get_character_count(ATK_TEXT(text));

   if (!widget || offset >= len)
     {
        *start_offset = -1;
        *end_offset = -1;

        return NULL;
     }

   *start_offset = 0;
   *end_offset = len;

   /* NOTE: Elm_Wrap_Type value is in 100% compatible with ATK wrap modes, so
    * no additional conversion is needed*/
   Elm_Wrap_Type wrap_type = elm_label_line_wrap_get(widget);
   at_set = eail_utils_text_add_attribute
       (at_set, ATK_TEXT_ATTR_WRAP_MODE,
        atk_text_attribute_get_value
         (ATK_TEXT_ATTR_WRAP_MODE, wrap_type));

   at_set = eail_utils_text_add_attribute
       (at_set, ATK_TEXT_ATTR_EDITABLE,
        atk_text_attribute_get_value
         (ATK_TEXT_ATTR_EDITABLE, FALSE));

   return at_set;
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
eail_label_get_text_after_offset(AtkText *text,
                                 gint offset,
                                 AtkTextBoundary boundary_type,
                                 gint *start_offset,
                                 gint *end_offset)
{
   Evas_Object *widget;
   const Evas_Object *textblock;
   Evas_Object *label_edje_layer;

   widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget) return NULL;

   label_edje_layer = elm_layout_edje_get(widget);
   textblock = edje_object_part_object_get(label_edje_layer, "elm.text");

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
eail_label_get_text_at_offset(AtkText *text,
                              gint offset,
                              AtkTextBoundary boundary_type,
                              gint *start_offset,
                              gint *end_offset)
{
   Evas_Object *widget;
   const Evas_Object *textblock;
   Evas_Object *label_edje_layer;

   widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget) return NULL;

   label_edje_layer = elm_layout_edje_get(widget);
   textblock = edje_object_part_object_get(label_edje_layer, "elm.text");

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
eail_label_get_text_before_offset(AtkText *text,
                                  gint offset,
                                  AtkTextBoundary boundary_type,
                                  gint *start_offset,
                                  gint *end_offset)
{
   Evas_Object *widget;
   const Evas_Object *textblock;
   Evas_Object *label_edje_layer;

   widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget) return NULL;

   label_edje_layer = elm_layout_edje_get(widget);
   textblock = edje_object_part_object_get(label_edje_layer, "elm.text");

   return eail_get_text_before(textblock, offset, boundary_type, start_offset,
                               end_offset);
}

/**
 * @brief Gets the offset of the character located at coordinates x and y.
 *  x and y are interpreted as being relative to the screen or this widget's window depending on coords.
 *
 * @param text AtkText instance
 * @param screen x-position of character
 * @param screen y-position of character
 * @param specify whether coordinates are relative to the screen or widget window
 *
 * @returns the offset to the character which is located at the specified x and y coordinates.
 */
static gint
eail_label_get_offset_at_point(AtkText *text,
                                 gint x,
                                 gint y,
                                 AtkCoordType coords)
{
   int result = -1;
   const Evas_Object *textblock = NULL;
   Evas_Textblock_Cursor *cur = NULL;
   Evas_Object *label_edje_layer = NULL;

   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(text));

   if (!widget) return -1;

   label_edje_layer = elm_layout_edje_get(widget);

   if (!label_edje_layer) return-1 ;

   textblock = edje_object_part_object_get(label_edje_layer, "elm.text");
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

/*
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
eail_label_get_character_extents(AtkText *text,
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
   Evas_Object *label_edje_layer = NULL;

   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(text));

   if (!widget) return;

   label_edje_layer = elm_layout_edje_get(widget);

   if (!label_edje_layer) return;

   textblock = edje_object_part_object_get(label_edje_layer, "elm.text");

   cur = evas_object_textblock_cursor_new(textblock);
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
 * @brief Sum all rectangles on list
 *
 * (function for internal use ony)
 *
 * @param list list with Evas_Textblock_Rectangle
 *
 * @return an Evas_Textblock_Rectangle encloses all rectanges from list
 */
Evas_Textblock_Rectangle _internal_sum_rects_on_list(Eina_List *list)
{
    Evas_Textblock_Rectangle result;

    Evas_Coord min_x = 0;
    Evas_Coord min_y = 0;
    Evas_Coord max_x = 0;
    Evas_Coord max_y = 0;
    int first_run = 1;

    Eina_List *l = NULL;
    Evas_Textblock_Rectangle *rectT = NULL;
    EINA_LIST_FOREACH(list, l, rectT)
    {
        if (first_run)
        {
            min_x = rectT->x;
            min_y = rectT->y;
            max_x = rectT->x + rectT->w;
            max_y = rectT->y + rectT->h;
            first_run = 0;
        }
        else
        {
            Evas_Coord t_min_x = rectT->x;
            Evas_Coord t_min_y = rectT->y;
            Evas_Coord t_max_x = rectT->x + rectT->w;
            Evas_Coord t_max_y = rectT->y + rectT->h;


            if ( t_min_x < min_x )
                min_x = t_min_x;
            if ( t_min_y < min_y )
                min_y = t_min_y;
            if ( t_max_x > max_x )
                max_x = t_max_x;
            if ( t_max_y > max_y )
                max_y = t_max_y;
        }
    }

    result.x = min_x;
    result.y = min_y;
    result.w = max_x - min_x;
    result.h = max_y - min_y;

    return result;
}

/**
 * Get the bounding box for text within the specified range.
 *
 * @param text an AtkText
 * @param start_offset The offset of the first text character for which boundary information is required.
 * @param end_offset The offset of the text character after the last character for which boundary information is required.
 * @param coord_type Specify whether coordinates are relative to the screen or widget window.
 * @param [out] rect A pointer to a AtkTextRectangle which is filled in by this function.
 */
void eail_label_get_range_extents(AtkText *text,
                                 gint start_offset,
                                 gint end_offset,
                                 AtkCoordType coord_type,
                                 AtkTextRectangle *rect)
{
    if (start_offset >= end_offset)
       return;

    Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(text));
    Evas_Textblock_Cursor *cur1 = NULL;
    Evas_Textblock_Cursor *cur2 = NULL;
    Evas_Object *label_edje_layer = NULL;
    const Evas_Object *textblock = NULL;

    label_edje_layer = elm_layout_edje_get(widget);
    textblock = edje_object_part_object_get(label_edje_layer, "elm.text");
    cur1 = evas_object_textblock_cursor_new(textblock);
    cur2 = evas_object_textblock_cursor_new(textblock);

    evas_textblock_cursor_pos_set(cur1, start_offset);
    evas_textblock_cursor_pos_set(cur2, end_offset);

    Eina_List *list = NULL;
    list = evas_textblock_cursor_range_geometry_get(cur1, cur2);

    if (list)
    {
        Evas_Textblock_Rectangle rect_list = _internal_sum_rects_on_list(list);
        rect->x = rect_list.x;
        rect->y = rect_list.y;
        rect->width = rect_list.w;
        rect->height = rect_list.h;

        if (coord_type == ATK_XY_SCREEN)
        {
            int ee_x, ee_y;
            Ecore_Evas *ee= ecore_evas_ecore_evas_get(evas_object_evas_get(widget));

            ecore_evas_geometry_get(ee, &ee_x, &ee_y, NULL, NULL);
            rect->x += ee_x;
            rect->y += ee_y;
        }
    }

    if (cur1)
        evas_textblock_cursor_free(cur1);
    if (cur2)
        evas_textblock_cursor_free(cur2);
}

/**
 * @brief Initializes AtkTextIface interface
 *
 * @param iface AtkTextIface instance
 */
static void
atk_text_interface_init(AtkTextIface *iface)
{
   iface->get_character_count = eail_label_get_character_count;
   iface->get_character_at_offset = eail_label_get_character_at_offset;
   iface->get_text = eail_label_get_text;
   iface->get_run_attributes = eail_label_get_run_attributes;
   iface->get_default_attributes = eail_label_get_default_attributes;
   iface->get_text_after_offset = eail_label_get_text_after_offset;
   iface->get_text_at_offset = eail_label_get_text_at_offset;
   iface->get_text_before_offset = eail_label_get_text_before_offset;
   iface->get_offset_at_point = eail_label_get_offset_at_point;
   iface->get_character_extents = eail_label_get_character_extents;
   iface->get_range_extents = eail_label_get_range_extents;
}

