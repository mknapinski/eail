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
 * @file eail_multibuttonentry.c
 * @brief EailMultibuttonentry implementation
 */

#include <Elementary.h>

#include "eail_multibuttonentry.h"
#include "eail_factory.h"
#include "eail_item_parent.h"
#include "eail_priv.h"
#include "eail_utils.h"
#include "eail_clipboard.h"

static void eail_item_parent_interface_init(EailItemParentIface *iface);
static void atk_editable_text_interface_init(AtkEditableTextIface *iface);
static void atk_text_interface_init(AtkTextIface *iface);

/**
 * @brief EailMultibuttonentry type definition
 */
G_DEFINE_TYPE_WITH_CODE(EailMultibuttonentry,
                        eail_multibuttonentry,
                        EAIL_TYPE_ACTION_WIDGET,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_TEXT,
                                              atk_text_interface_init)
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_EDITABLE_TEXT,
                                              atk_editable_text_interface_init)
                        G_IMPLEMENT_INTERFACE(EAIL_TYPE_ITEM_PARENT,
                                              eail_item_parent_interface_init));

/**
 * @brief Checks if content get is supported
 *
 * Implementation of EailItemParent->is_content_get_supported callback.
 *
 * @param parent EailItemParent instance
 * @param item EailItem instance
 * @returns always FALSE
 */
gboolean
eail_multibuttonentry_content_get_support(EailItemParent   *parent,
                                          EailItem         *item)
{
   return FALSE;
}

/**
 * @brief Gets the name of a multibuttonentry's child
 *
 * Implementation of get_item_name callback of EailItemParent interface.
 *
 * @param parent EailItemParent instance representing a multibuttonentry
 * @param item Multibuttonentry EailItem instance representing a child
 *
 * @returns string representing the name of the child
 */
static const gchar *
eail_multibuttonentry_item_name_get(EailItemParent *parent, EailItem *item)
{
   Elm_Object_Item *it = eail_item_get_item(item);
   /*that will work only for default theme*/
   if (!it) return NULL;

   return elm_object_item_part_text_get(it, NULL);
}

/**
 * @brief EailItemParent interface initializer
 *
 * Initialization of callbacks used by EailItemParent interface.
 *
 * @param iface EailItemParentIface instance
 */
static void
eail_item_parent_interface_init(EailItemParentIface *iface)
{
   iface->is_content_get_supported = eail_multibuttonentry_content_get_support;
   iface->get_item_name = eail_multibuttonentry_item_name_get;
}

/**
 * @brief Helper function for getting multibuttonentry text
 *
 * @param text an AtkText (EailMultibutton object)
 *
 * @return string representation of multibutton entry text
 */
static const gchar*
_eail_multibuttonentry_get_entry_string(AtkText *text)
{
   const gchar *string = NULL;
   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(text));
   Evas_Object *entry = elm_multibuttonentry_entry_get(widget);

   if (entry)
     string = elm_entry_entry_get(entry);

   /* if entry string is empty then trying to return "guide" part*/
   if (!string || 0 == g_strcmp0(string, ""))
     {
        /* If there are some children of widget, then it means that guide
         * is not visible. So returning guide as text only if no children
         * exist */
        if (!elm_multibuttonentry_items_get(widget))
          string = elm_object_part_text_get(widget, "guide");
     }

   return string;
}

/**
 * @brief Gets text bounded by start_offset and end_offset
 *
 * Use g_free() to free the returned string.
 *
 * @param text AtkText instance
 * @param start_offset start position
 * @param end_offset end position or -1 for the end of the string
 * @return string containing text from start_offset up to, but not including
 * end_offset
 */
static gchar*
eail_multibuttonentry_get_text(AtkText   *text,
                   gint       start_offset,
                   gint       end_offset)
{
   const gchar *string = NULL;

   string = _eail_multibuttonentry_get_entry_string(text);

   return eail_get_substring(string, start_offset, end_offset);
}

/**
 * @brief Gets the character at offset
 *
 * @param text AtkText instance
 * @param offset offset to obtain the character from
 * @return char representing the character at given offset
 */
static gunichar
eail_multibuttonentry_get_character_at_offset(AtkText    *text,
                                              gint        offset)
{
   gunichar character = '\0';

   character = g_utf8_get_char(
         g_utf8_offset_to_pointer
                     (_eail_multibuttonentry_get_entry_string(text), offset));

   return character;
}

/**
 * @brief Gets the length of text
 *
 * @param text AtkText instance
 * @return integer representing text's length
 */
static gint
eail_multibuttonentry_get_character_count(AtkText *text)
{
   gint count = 0;
   const gchar *string_text = NULL;

   string_text = _eail_multibuttonentry_get_entry_string(text);
   if (!string_text) return count;

   count = g_utf8_strlen(string_text, -1);

   return count;
}

/**
 * @brief Gets caret offset
 *
 * @param text an AtkText
 * @return caret offset
 */
static gint
eail_multibuttonentry_get_caret_offset(AtkText *text)
{
   Evas_Object *widget;
   Evas_Object *entry;

   widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget)
     return 0;

   entry = elm_multibuttonentry_entry_get(widget);
   if (!entry)
     return 0;

   return elm_entry_cursor_pos_get(entry);
}

/**
 * @brief Sets offset for caret
 *
 * Implementation of AtkTextIface->set_caret_offset callback
 * ATK doc says:
 * Sets the caret (cursor) position to the specified offset.
 *
 * @param text an AtkText
 * @param offset starting position
 *
 * @returns TRUE if success, FALSE otherwise.
 */
static gboolean
eail_multibuttonentry_set_caret_offset (AtkText *text,
                             gint     offset)
{
   Evas_Object *widget;
   Evas_Object *entry;

   widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget)
     return FALSE;

   entry = elm_multibuttonentry_entry_get(widget);
   if (!entry)
     return FALSE;

   elm_entry_cursor_pos_set(entry,offset);

   return TRUE;
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
eail_multibuttonentry_get_character_extents(AtkText *text,
                                            gint offset,
                                            gint *x,
                                            gint *y,
                                            gint *width,
                                            gint *height,
                                            AtkCoordType coords)
{
   int result = -1;
   Evas_Object *textblock = NULL;
   Evas_Textblock_Cursor *cur = NULL;
   Evas_Object *entry = NULL;

   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(text));

   if (!widget) return;

   entry = elm_multibuttonentry_entry_get(widget);

   if (!entry) return;

   textblock = elm_entry_textblock_get(entry);
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
 * @brief Adds a selection bounded by the specified offsets
 *
 * @param text AtkText instance
 * @param start_offset start position of the selection
 * @param end_offset offset of the first character after selection
 * @returns TRUE on success, FALSE otherwise
 */
static gboolean
eail_multibuttonentry_add_selection(AtkText *text,
                         gint start_offset,
                         gint end_offset)
{
   Evas_Object *widget;
   Evas_Object *entry;

   widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget)
     return FALSE;

   entry = elm_multibuttonentry_entry_get(widget);
   if (!entry)
     return FALSE;

   elm_entry_cursor_pos_set(entry, start_offset);
   elm_entry_cursor_selection_begin(entry);
   elm_entry_cursor_pos_set(entry, end_offset);
   elm_entry_cursor_selection_end(entry);

   EAIL_MULTIBUTTONENTRY(text)->selection_start = start_offset;
   EAIL_MULTIBUTTONENTRY(text)->selection_end = end_offset;

   return TRUE;
}

/**
 * @brief Removes text selection
 *
 * This widget supports only one selection
 * so selection_num should always be 0.
 *
 * @param text AtkText instance
 * @param selection_num selection number
 * @return TRUE on success, FALSE otherwise
 */
static gboolean
eail_multibuttonentry_remove_selection(AtkText *text,
                            gint selection_num)
{
   Evas_Object *widget;
   Evas_Object *entry;

   widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget)
     return FALSE;

   entry = elm_multibuttonentry_entry_get(widget);
   if (!entry)
     return FALSE;

   if (selection_num != 0 || !elm_entry_selection_get(entry))
     return FALSE;

   elm_entry_select_none(entry);
   EAIL_MULTIBUTTONENTRY(text)->selection_start = 0;
   EAIL_MULTIBUTTONENTRY(text)->selection_end = 0;

   return TRUE;
}

/**
 * @brief Sets text selection for multibuttonentry
 *
 * This widget supports only one selection
 * so selection_num should always be 0.
 *
 * @param text AtkText instance
 * @param selection_num selection number
 * @param start_pos start position of the selected region
 * @param end_pos end position of the selected region
 *
 * @returns TRUE on success, FALSE otherwise
 */
static gboolean
eail_multibuttonentry_set_selection(AtkText *text,
                         gint     selection_num,
                         gint     start_pos,
                         gint     end_pos)
{
   if (0 != selection_num)
     return FALSE;

   return eail_multibuttonentry_add_selection(text, start_pos, end_pos);
}

/**
 * @brief Gets text selection from multibuttonentry
 *
 * Use g_free() to free the returned string.
 *
 * This widget supports only one selection
 * so selection_num should always be 0.
 *
 * @param text AtkText instance
 * @param selection_num selection number
 * @param start_offset start position of the selected region
 * @param end_offset end position of the selected region
 *
 * @returns newly allocated string containing the selected text
 */
static gchar *
eail_multibuttonentry_get_selection(AtkText *text,
                         gint     selection_num,
                         gint    *start_offset,
                         gint    *end_offset)
{
   const char* selection;
   Evas_Object *widget;
   Evas_Object *entry;

   if (0 != selection_num)
     return g_strdup("");

   widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget)
     return g_strdup("");

   entry = elm_multibuttonentry_entry_get(widget);
   if (!widget)
     return g_strdup("");

   selection = elm_entry_selection_get(entry);
   if (selection)
   {
     *start_offset = EAIL_MULTIBUTTONENTRY(text)->selection_start;
     *end_offset = EAIL_MULTIBUTTONENTRY(text)->selection_end;
     return g_strdup(selection);
   }

   return g_strdup("");
}


/**
 * @brief Gets the number of selected text regions
 *
 * Only one selection is supported by this widget
 * so the returned value is 0 or 1.
 *
 * @param text AtkText instance
 * @returns integer representing the number of
 * selected text regions
 */
static gint
eail_multibuttonentry_get_n_selections(AtkText *text)
{
   Evas_Object *widget;
   Evas_Object *entry;

   widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget)
     return 0;

   entry = elm_multibuttonentry_entry_get(widget);
   if (!entry)
     return 0;

   if (elm_entry_selection_get(entry))
     return 1;

   return 0;
}

/*
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
eail_multibuttonentry_get_text_after_offset(AtkText *text,
                                            gint offset,
                                            AtkTextBoundary boundary_type,
                                            gint *start_offset,
                                            gint *end_offset)
{
   Evas_Object *widget;
   Evas_Object *textblock;
   Evas_Object *entry = NULL;

   widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget) return NULL;

   entry = elm_multibuttonentry_entry_get(widget);
   if (!entry) return NULL;

   textblock = elm_entry_textblock_get(entry);

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
eail_multibuttonentry_get_text_at_offset(AtkText *text,
                                         gint offset,
                                         AtkTextBoundary boundary_type,
                                         gint *start_offset,
                                         gint *end_offset)
{
   Evas_Object *widget;
   Evas_Object *textblock;
   Evas_Object *entry = NULL;

   widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget) return NULL;

   entry = elm_multibuttonentry_entry_get(widget);
   if (!entry) return NULL;

   textblock = elm_entry_textblock_get(entry);

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
eail_multibuttonentry_get_text_before_offset(AtkText *text,
                                             gint offset,
                                             AtkTextBoundary boundary_type,
                                             gint *start_offset,
                                             gint *end_offset)
{
   Evas_Object *widget;
   Evas_Object *textblock;
   Evas_Object *entry = NULL;

   widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget) return NULL;

   entry = elm_multibuttonentry_entry_get(widget);
   if (!entry) return NULL;

   textblock = elm_entry_textblock_get(entry);

   return eail_get_text_before(textblock, offset, boundary_type, start_offset,
                               end_offset);
}

/**
 * @brief Creates an AtkAttributeSet which consists of the default values of
 * attributes for the text
 *
 * The returned AtkAttributeSet should be freed by a call to
 * atk_attribute_set_free().
 *
 * @param text AtkText instance
 *
 * @returns AtkAttributeSet which contains the default values of attributes
 * at offset
 */
static AtkAttributeSet *
eail_multibuttonentry_get_default_attributes(AtkText *text)
{
   AtkAttributeSet *at_set = NULL;

   at_set = eail_utils_text_add_attribute
       (at_set, ATK_TEXT_ATTR_WRAP_MODE,
        atk_text_attribute_get_value(ATK_TEXT_ATTR_WRAP_MODE, 0));

   at_set = eail_utils_text_add_attribute
       (at_set, ATK_TEXT_ATTR_EDITABLE,
        atk_text_attribute_get_value
         (ATK_TEXT_ATTR_EDITABLE, TRUE));

   return at_set;
}

/**
 * @brief Creates an AtkAttributeSet which consists of the attributes
 * explicitly set at the position offset in the text
 *
 * start_offset and end_offset are set to the start and end of the range
 * around offset where the attributes are invariant. Note that end_offset
 * is the offset of the first character after the range.
 *
 * The returned AtkAttributeSet should be freed by a call to
 * atk_attribute_set_free()
 *
 * @param text AtkText instance
 * @param offset offset at which to get the attributes
 * @param [out] start_offset start offset of the range
 * @param [out] end_offset end offset of the range
 *
 * @returns AtkAttributeSet which contains the attributes explicitly set at
 * offset
 */
static AtkAttributeSet *
eail_multibuttonentry_get_run_attributes(AtkText *text,
                                         gint offset,
                                         gint *start_offset,
                                         gint *end_offset)
{
   AtkAttributeSet *at_set = NULL;
   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(text));
   Evas_Object *entry = NULL;

   if (!widget || offset >= eail_multibuttonentry_get_character_count(text))
     {
        *start_offset = -1;
        *end_offset = -1;

        return NULL;
     }

   entry = elm_multibuttonentry_entry_get(widget);
   if (!entry) return NULL;


   *start_offset = 0;
   *end_offset = eail_multibuttonentry_get_character_count(text);

   /* NOTE: Elm_Wrap_Type value is in 100% compatible with ATK wrap modes, so
    * no additional conversion is needed*/
   Elm_Wrap_Type wrap_type = elm_entry_line_wrap_get(entry);
   at_set = eail_utils_text_add_attribute
       (at_set, ATK_TEXT_ATTR_WRAP_MODE,
        atk_text_attribute_get_value
         (ATK_TEXT_ATTR_WRAP_MODE, wrap_type));

   Eina_Bool editable = elm_entry_editable_get(entry);
   at_set = eail_utils_text_add_attribute
       (at_set, ATK_TEXT_ATTR_EDITABLE,
        atk_text_attribute_get_value
         (ATK_TEXT_ATTR_EDITABLE, editable));

   return at_set;
}

/**
 * @brief AktText initialization function
 *
 * @param iface AtkTextIface instance
 */
static void
atk_text_interface_init(AtkTextIface *iface)
{
   iface->get_text = eail_multibuttonentry_get_text;
   iface->get_character_at_offset = eail_multibuttonentry_get_character_at_offset;
   iface->get_character_count = eail_multibuttonentry_get_character_count;
   iface->get_caret_offset = eail_multibuttonentry_get_caret_offset;
   iface->set_caret_offset = eail_multibuttonentry_set_caret_offset;
   iface->get_character_extents = eail_multibuttonentry_get_character_extents;
   iface->add_selection    = eail_multibuttonentry_add_selection;
   iface->remove_selection = eail_multibuttonentry_remove_selection;
   iface->get_selection    = eail_multibuttonentry_get_selection;
   iface->set_selection    = eail_multibuttonentry_set_selection;
   iface->get_n_selections = eail_multibuttonentry_get_n_selections;
   iface->get_text_after_offset = eail_multibuttonentry_get_text_after_offset;
   iface->get_text_at_offset = eail_multibuttonentry_get_text_at_offset;
   iface->get_text_before_offset = eail_multibuttonentry_get_text_before_offset;
   iface->get_run_attributes = eail_multibuttonentry_get_run_attributes;
   iface->get_default_attributes = eail_multibuttonentry_get_default_attributes;
}

/**
 * @brief Sets text contents of text
 *
 * @param text AtkEditableText instance
 * @param string new text to be set
 */
static void
eail_multibuttonentry_set_text_contents(AtkEditableText *text,
                                        const gchar *string)
{
   Evas_Object *entry;
   Evas_Object *widget;

   widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget) return;

   entry = elm_multibuttonentry_entry_get(widget);
   if (!entry) return;

   if (elm_multibuttonentry_editable_get(widget))
     elm_entry_entry_set(entry, string);
}

/**
 * @brief Copies text content from entry to clipboard
 *
 * @param text AtkEditableText instance
 * @param start_pos index of copied text's first character
 * @param end_pos index of last copied text's last character
 */
static void
eail_multibuttonentry_copy_text(AtkEditableText *text,
                     gint             start_pos,
                     gint             end_pos)
{
   Evas_Object *widget;
   Evas_Object *entry;
   const char *entry_text;
   char *tmp;


   widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget) return;

   entry = elm_multibuttonentry_entry_get(widget);
   entry_text = elm_entry_entry_get(entry);
   tmp = eail_get_substring(entry_text, start_pos, end_pos);
   eail_clipboard_set_text(tmp);
   g_free(tmp);
}

/**
 * @brief Cuts text content from entry to clipboard
 *
 * @param text AtkEditableText instance
 * @param start_pos index of cut text's first character
 * @param end_pos index of cut text's last character
 */
static void
eail_multibuttonentry_cut_text(AtkEditableText *text,
                    gint             start_pos,
                    gint             end_pos)
{
   Evas_Object *widget;
   Evas_Object *entry;
   const char *entry_text;
   char *tmp;
   GString *s;


   widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget || !elm_multibuttonentry_editable_get(widget)) return;

   entry = elm_multibuttonentry_entry_get(widget);
   entry_text = elm_entry_entry_get(entry);
   tmp = eail_get_substring(entry_text, start_pos, end_pos);
   eail_clipboard_set_text(tmp);
   g_free(tmp);

   s = g_string_new(entry_text);
   s = g_string_erase(s, start_pos, end_pos - start_pos);

   elm_entry_entry_set(entry, s->str);
   g_string_free(s, TRUE);
}

/**
 * @brief Pastes text content from clipboard into entry
 *
 * @param text AtkEditableText instance
 * @param position index of pasted text's first character
 */
static void
eail_multibuttonentry_paste_text(AtkEditableText *text,
                                 gint position)
{
   Evas_Object *widget;
   Evas_Object *entry = NULL;
   GString *s;
   const char *tmp;

   widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget || !elm_multibuttonentry_editable_get(entry))
     return;

   entry = elm_multibuttonentry_entry_get(widget);
   s = g_string_new(elm_entry_entry_get(entry));
   tmp = eail_clipboard_get_text();
   s = g_string_insert(s, position, tmp);
   elm_entry_entry_set(entry, s->str);
   g_string_free(s, TRUE);
}

/**
 * @brief Deletes text between start_pos and end_pos but not
 * including end_pos
 *
 * @param text AtkEditableText instance
 * @param start_pos start position
 * @param end_pos end position
 */
static void
eail_multibuttonentry_delete_text(AtkEditableText *text,
                       gint start_pos,
                       gint end_pos)
{
   Evas_Object *widget;
   Evas_Object *entry;
   GString *s;

   widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget || !elm_multibuttonentry_editable_get(widget)) return;

   entry = elm_multibuttonentry_entry_get(widget);
   s = g_string_new(elm_entry_entry_get(entry));
   s = g_string_erase(s, start_pos, end_pos-start_pos);
   elm_entry_entry_set(entry, s->str);
   g_string_free(s, TRUE);
}

/**
 * @brief Inserts text at the given position
 *
 * After the call it points at the position after the newly inserted text.
 *
 * @param text AtkEditableText instance
 * @param string string to insert
 * @param length string length
 * @param [out] position position to insert the text
 *
 */
static void
eail_multibuttonentry_insert_text(AtkEditableText *text,
                       const gchar *string,
                       gint length,
                       gint *position)
{
   Evas_Object *widget;
   Evas_Object *entry;
   GString *s;

   widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget || !elm_multibuttonentry_editable_get(widget))
     return;

   entry = elm_multibuttonentry_entry_get(widget);
   s = g_string_new(elm_entry_entry_get(entry));
   s = g_string_insert_len(s, *position, string, length);
   elm_entry_entry_set(entry, s->str);
   g_string_free(s, TRUE);
   *position += length;
}

/**
 * @brief Initialization for AtkEditableTextIface interface
 *
 * Function called upon instance creation.
 *
 * It initializes AtkText interface
 * implementation i.e hooks method pointers in the interface structure
 * to the implementing class's implementation.
 *
 * @param iface AtkEditableTextIface instance
 */
static void
atk_editable_text_interface_init(AtkEditableTextIface *iface)
{
   iface->set_text_contents = eail_multibuttonentry_set_text_contents;
   iface->copy_text = eail_multibuttonentry_copy_text;
   iface->cut_text = eail_multibuttonentry_cut_text;
   iface->paste_text = eail_multibuttonentry_paste_text;
   iface->delete_text = eail_multibuttonentry_delete_text;
   iface->insert_text = eail_multibuttonentry_insert_text;
}

/**
 * @brief 'expand' action callback
 *
 * @param action AtkAction instance
 * @param data user data passed to callback
 *
 * @returns TRUE on action success, FALSE otherwise
 */
static gboolean
eail_multibuttonentry_action_expand_cb(AtkAction *action, void *data)
{
   Evas_Object *widget;

   widget = eail_widget_get_widget(EAIL_WIDGET(action));
   if (!widget) return FALSE;

   if ((elm_object_disabled_get(widget)) || (!evas_object_visible_get(widget)))
     return FALSE;

   if (elm_multibuttonentry_expanded_get(widget)) return FALSE;

   elm_multibuttonentry_expanded_set(widget, EINA_TRUE);
   atk_object_notify_state_change(ATK_OBJECT(action), ATK_STATE_EXPANDED, TRUE);

   return TRUE;
}

/**
 * @brief 'shrink' action callback
 *
 * @param action AtkAction instance
 * @param data user data passed to callback
 *
 * @returns TRUE on action success, FALSE otherwise
 */
static gboolean
eail_multibuttonentry_action_shrink_cb(AtkAction *action, void *data)
{
   Evas_Object *widget;

   widget = eail_widget_get_widget(EAIL_WIDGET(action));
   if (!widget) return FALSE;

   if ((elm_object_disabled_get(widget)) || (!evas_object_visible_get(widget)))
     return FALSE;

   if (!elm_multibuttonentry_expanded_get(widget)) return FALSE;

   elm_multibuttonentry_expanded_set(widget, EINA_FALSE);
   atk_object_notify_state_change(ATK_OBJECT(action), ATK_STATE_EXPANDED, FALSE);

   return TRUE;
}

/**
 * @brief Registers multibuttonentry's actions
 *
 * @param action_widget EailActionWidget instance
 */
static void eail_multibuttonentry_actions_init(EailActionWidget *action_widget)
{
   eail_action_widget_action_append(action_widget, "expand", NULL,
                                    eail_multibuttonentry_action_expand_cb);
   eail_action_widget_action_append(action_widget, "shrink", NULL,
                                    eail_multibuttonentry_action_shrink_cb);
}

/**
 * @brief Handler for "item,added" event, used to notify about multibuttonentry
 * content changes
 *
 * @param data passed to callback
 * @param obj object that raised event
 * @param event_info additional event info (item is passed here)
 */
static void
eail_multibuttonentry_item_handle_added_event(void *data,
                                              Evas_Object *obj,
                                              void *event_info)
{
   Elm_Object_Item *item = (Elm_Object_Item *) event_info;
   AtkObject *atk_item = NULL, *atk_parent = NULL;

   atk_parent = ATK_OBJECT(data);
   if (!atk_parent) return;

   atk_item = eail_factory_get_item_atk_obj
                                         (item, ATK_ROLE_LABEL, atk_parent);

   if (!atk_item) return;

   eail_emit_children_changed_obj(TRUE, atk_parent, atk_item);
}

/**
 * @brief Handler for "item,deleted" event, used to notify about
 * multibuttonentry content changes
 *
 * @param data passed to callback
 * @param obj object that raised event
 * @param event_info additional event info (item is passed here)
 */
static void
eail_multibuttonentry_item_handle_removed_event(void *data,
                                                Evas_Object *obj,
                                                void *event_info)
{
   Elm_Object_Item *item = (Elm_Object_Item *) event_info;
   AtkObject *atk_item = NULL, *atk_parent = NULL;

   atk_parent = ATK_OBJECT(data);
   if (!atk_parent) return;

   atk_item = eail_factory_get_item_atk_obj
                                         (item, ATK_ROLE_LABEL, atk_parent);

   if (!atk_item) return;

   eail_emit_children_changed_obj(FALSE, atk_parent, atk_item);
   atk_object_notify_state_change(atk_item, ATK_STATE_DEFUNCT, TRUE);

   DBG("Unregistering item from cache...");
   eail_factory_unregister_item_from_cache(item);
}

/**
 * @brief handler for event which is raised when entry cusror position is being changed
 *
 * @param data passed to callback
 * @param obj object that raised event
 * @param event_info additional event info
 */
void
_eail_multibuttonentry_handle_cursor_changed_event(void *data,
                                 Evas_Object *obj,
                                 void *event_info)
{
   g_signal_emit_by_name (ATK_OBJECT(data), "text_caret_moved",
                                 elm_entry_cursor_pos_get(obj));
}

/**
 * @brief EailMultibuttonentry type initializer
 * @param obj AtkObject instance
 * @param data initialization data
 */
void
eail_multibuttonentry_initialize(AtkObject *obj, gpointer data)
{
   Evas_Object *nested_widget = NULL;
   ATK_OBJECT_CLASS(eail_multibuttonentry_parent_class)->initialize(obj, data);
   obj->role = ATK_ROLE_ENTRY;

   eail_multibuttonentry_actions_init(EAIL_ACTION_WIDGET(obj));

   nested_widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!nested_widget)
     {
        ERR("No evas object inside EailWidget was found");
        return;
     }

   evas_object_smart_callback_add(nested_widget, "item,added",
                          eail_multibuttonentry_item_handle_added_event, obj);
   evas_object_smart_callback_add(nested_widget, "item,deleted",
                          eail_multibuttonentry_item_handle_removed_event, obj);
   evas_object_smart_callback_add(elm_multibuttonentry_entry_get(nested_widget),
                          "cursor,changed",
                          _eail_multibuttonentry_handle_cursor_changed_event, obj);
}

/**
 * @brief EailMultibuttonentry instance initializer
 *
 * @param multibuttonentry EailMultibuttonentry instance
 */
static void
eail_multibuttonentry_init(EailMultibuttonentry *multibuttonentry)
{
   multibuttonentry->selection_start = 0;
   multibuttonentry->selection_end = 0;
}

/**
 * @brief Gets obj's state set
 *
 * The caller must unreference it when it is no longer needed.
 *
 * @param obj AtkObject instance
 * @return AtkStateSet containing object's state set
 */
static AtkStateSet *
eail_multibuttonentry_ref_state_set(AtkObject *obj)
{
   AtkStateSet *state_set;
   Evas_Object *widget;

   g_return_val_if_fail(EAIL_MULTIBUTTONENTRY(obj), NULL);

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   state_set = ATK_OBJECT_CLASS(eail_multibuttonentry_parent_class)->ref_state_set(obj);

   if (!widget) return state_set;

   if (elm_multibuttonentry_expanded_get(widget))
     {
        atk_state_set_add_state(state_set, ATK_STATE_EXPANDED);

        if (elm_multibuttonentry_editable_get(widget))
            atk_state_set_add_state(state_set, ATK_STATE_EDITABLE);
     }
   else
     {
        atk_state_set_add_state(state_set, ATK_STATE_DEFAULT);
     }

   return state_set;
}

/**
 * @brief Gets the list of multibuttonentry's items
 *
 * @param multibuttonentry EailMultibuttonentry instance
 * @return Eina_List containing multibuttonentry's items
 * */
static const Eina_List *
eail_multibuttonentry_get_items(EailMultibuttonentry *multibuttonentry)
{
   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(multibuttonentry));
   if (!widget) return NULL;

   return elm_multibuttonentry_items_get(widget);
}

/**
 * @brief Gets the number of accessible children of the accessible
 *
 * @param obj AtkObject instance
 * @return integer representing the number of accessible children of the accessible
 */
static gint
eail_multibuttonentry_get_n_children(AtkObject *obj)
{
   gint n_items;
   const Eina_List *items;

   items = eail_multibuttonentry_get_items(EAIL_MULTIBUTTONENTRY(obj));
   n_items = eina_list_count(items);

   return n_items;
}

/**
 * @brief Gets a reference to the specified child of obj
 *
 * The caller must unreference it when it is no longer needed.
 *
 * @param obj AtkObject instance
 * @param i child index
 * @return AtkObject containing reference to the specified child of obj
 */
static AtkObject *
eail_multibuttonentry_ref_child(AtkObject *obj, gint i)
{
   const Eina_List *items;
   AtkObject *child = NULL;

   items = eail_multibuttonentry_get_items(EAIL_MULTIBUTTONENTRY(obj));
   if (eina_list_count(items) > i)
     {
        child = eail_factory_get_item_atk_obj
            (eina_list_nth(items, i), ATK_ROLE_LABEL, obj);

        g_object_ref(child);
     }
   else
     ERR("Tried to ref child with index %d out of bounds!", i);

   return child;
}

/**
 * @brief EailMultibuttonentry class initializer
 *
 * @param klass EailMultibuttonentryClass instance
 */
static void
eail_multibuttonentry_class_init(EailMultibuttonentryClass *klass)
{
   AtkObjectClass *atk_class = ATK_OBJECT_CLASS(klass);

   atk_class->initialize = eail_multibuttonentry_initialize;
   atk_class->get_n_children = eail_multibuttonentry_get_n_children;
   atk_class->ref_child = eail_multibuttonentry_ref_child;
   atk_class->ref_state_set = eail_multibuttonentry_ref_state_set;
}
