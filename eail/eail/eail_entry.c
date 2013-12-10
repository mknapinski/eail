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
 * @file eail_entry.c
 * @brief EailEntry implementation
 */

#include <Elementary.h>

#include "eail_entry.h"
#include "eail_utils.h"
#include "eail_priv.h"
#include "eail_clipboard.h"

static void atk_text_interface_init(AtkTextIface *iface);
static void atk_editable_text_interface_init(AtkEditableTextIface *iface);

/**
 * @brief Definition of EailEntry as GObject
 *
 * EailEntry is extended EAIL_TYPE_TEXT with ATK_TYPE_TEXT and
 * ATK_TYPE_EDITABLE_TEXT interfaces implemented.
 *
 */
G_DEFINE_TYPE_WITH_CODE(EailEntry,
                        eail_entry,
                        EAIL_TYPE_WIDGET,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_TEXT,
                                              atk_text_interface_init)
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_EDITABLE_TEXT,
                                              atk_editable_text_interface_init));

/**
 * @brief Handler for event which is raised when entry content has changed
 *
 * @param data data passed to callback
 * @param obj Evas_Object that raised event
 * @param event_info additional event info
 */
void
_eail_entry_handle_changed_event(void *data,
                                 Evas_Object *obj,
                                 void *event_info)
{
   eail_emit_atk_signal
                  (ATK_OBJECT(data), "visible-data-changed", ATK_TYPE_OBJECT);
}

/**
 * @brief handler for event which is raised when entry content is being pressed
 *
 * @param data passed to callback
 * @param obj object that raised event
 * @param event_info additional event info
 */
void
_eail_entry_handle_pressed_event(void *data,
                                 Evas_Object *obj,
                                 void *event_info)
{
   atk_object_notify_state_change(ATK_OBJECT(data), ATK_STATE_PRESSED, TRUE);
}

/**
 * @brief handler for event which is raised when entry cusror position is being changed
 *
 * @param data passed to callback
 * @param obj object that raised event
 * @param event_info additional event info
 */
void
_eail_entry_handle_cursor_changed_event(void *data,
                                 Evas_Object *obj,
                                 void *event_info)
{
   g_signal_emit_by_name (ATK_OBJECT(data), "text_caret_moved",
                                 elm_entry_cursor_pos_get(obj));
}

/**
 * @brief Initializer for AtkObject
 *
 * @param obj AtkObject instance
 * @param data initialization data
 */
static void
eail_entry_initialize(AtkObject *obj, gpointer data)
{
   Evas_Object *nested_widget = NULL;
   ATK_OBJECT_CLASS(eail_entry_parent_class)->initialize(obj, data);

   obj->role = ATK_ROLE_ENTRY;

   g_return_if_fail(EAIL_IS_WIDGET(obj));
   nested_widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!nested_widget)
     {
        ERR("No evas object inside EailWidget was found");
        return;
     }

   evas_object_smart_callback_add(nested_widget, "changed",
                                  _eail_entry_handle_changed_event, obj);
   evas_object_smart_callback_add(nested_widget, "cursor,changed",
                                  _eail_entry_handle_cursor_changed_event, obj);
   evas_object_smart_callback_add(nested_widget, "press",
                                  _eail_entry_handle_pressed_event, obj);
}

/**
 * @brief Class destructor
 *
 * @param obj GObject instance
 */
static void
eail_entry_finalize(GObject *obj)
{
}

/**
 * @brief Gets a reference to the state set of the accessible
 *
 * The caller must unreference it when it is no longer needed.
 *
 * Implementation of AtkObject->ref_state_set callback.
 *
 * @param obj AtkObject instance
 *
 * @returns AtkStateSet representing the state set of the
 * accessible
 */
static AtkStateSet *
eail_entry_ref_state_set(AtkObject *obj)
{
   AtkStateSet *state_set;
   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(obj));

   if (!widget)
     return NULL;

   state_set = ATK_OBJECT_CLASS(eail_entry_parent_class)->ref_state_set(obj);

   if (elm_entry_editable_get(widget))
     atk_state_set_add_state(state_set, ATK_STATE_EDITABLE);

   if (elm_entry_single_line_get(widget))
     atk_state_set_add_state(state_set, ATK_STATE_SINGLE_LINE);
   else
     atk_state_set_add_state(state_set, ATK_STATE_MULTI_LINE);

   return state_set;
}

/**
 * @brief Initializer for EailEntry GObject implementation
 *
 * @param entry EailEntry instance
 */
static void
eail_entry_init(EailEntry *entry)
{
   entry->selection_start = 0;
   entry->selection_end = 0;
}

/**
 * @brief Initializer for EailPopup GObject class
 *
 * Defines callbacks for base AtkObject.
 *
 * @param klass EailEntryClass instance
 */
static void
eail_entry_class_init(EailEntryClass *klass)
{
   AtkObjectClass *atk_class = ATK_OBJECT_CLASS(klass);
   GObjectClass *gobject_class = G_OBJECT_CLASS(klass);

   atk_class->initialize = eail_entry_initialize;
   atk_class->ref_state_set = eail_entry_ref_state_set;
   gobject_class->finalize = eail_entry_finalize;
}

/**
 * @brief Gets the caret offset
 *
 * @param text AtkText instance
 * @return integer representing the caret offset
 */
static gint
eail_entry_get_caret_offset(AtkText *text)
{
   Evas_Object *widget;

   widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget)
     return 0;

   return elm_entry_cursor_pos_get(widget);
}

/**
 * @brief Sets the caret (cursor) position to the specified offset.
 *
 * Implementation of AtkTextIface->set_caret_offset callback.
 *
 * @param text AtkText instance
 * @param offset starting position
 *
 * @returns TRUE on success, FALSE otherwise
 */
static gboolean
eail_entry_set_caret_offset (AtkText *text,
                             gint     offset)
{
   Evas_Object *widget;

   widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget)
     return FALSE;

   elm_entry_cursor_pos_set(widget, offset);

   return TRUE;
}

/**
 * @brief Gets the number of selected text regions
 *
 * @param text AtkText instance
 * @returns integer representing the number of
 * selected text regions
 */
static gint
eail_entry_get_n_selections(AtkText *text)
{
   Evas_Object *widget;

   widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget) return 0;

   if (elm_entry_selection_get(widget)) return 1;

   return 0;
}

/**
 * @brief Gets text selection from entry
 *
 * Use g_free() to free the returned string.
 *
 * The selected regions are assigned
 * numbers that correspond to how far the region is from the start of the text.
 * The selected region closest to the beginning of the text region is assigned
 * the number 0, etc. Note that adding, moving or deleting a selected region can
 * change the numbering.
 *
 * Implementation of AtkTextIface->get_selection callback.
 *
 * @param text AtkText instance
 * @param selection_num selection number
 * @param start_offset start position of the selected region
 * @param end_offset end position of the selected region
 *
 * @returns newly allocated string containing the selected text
 */
static gchar *
eail_entry_get_selection(AtkText *text,
                         gint     selection_num,
                         gint    *start_offset,
                         gint    *end_offset)
{
   const char* selection;
   Evas_Object *widget;

   widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget)
     return g_strdup("");

   if (0 != selection_num)
     return g_strdup("");

   selection = elm_entry_selection_get(widget);
   if (selection)
     {
        *start_offset = EAIL_ENTRY(text)->selection_start;
        *end_offset = EAIL_ENTRY(text)->selection_end;
        return g_strdup(selection);
     }

   return g_strdup("");
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
eail_entry_add_selection(AtkText *text,
                         gint start_offset,
                         gint end_offset)
{
   Evas_Object *widget;

   widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget) return FALSE;

   elm_entry_cursor_pos_set(widget, start_offset);
   elm_entry_cursor_selection_begin(widget);
   elm_entry_cursor_pos_set(widget, end_offset);
   elm_entry_cursor_selection_end(widget);

   EAIL_ENTRY(text)->selection_start = start_offset;
   EAIL_ENTRY(text)->selection_end = end_offset;

   return TRUE;
}

/**
 * @brief Removes text selection
 *
 * Only one selection is supported, so
 * selection number equals 0.
 *
 * @param text AtkText instance
 * @param selection_num selection number
 * @return TRUE on success, FALSE otherwise
 */
static gboolean
eail_entry_remove_selection(AtkText *text,
                            gint selection_num)
{
   Evas_Object *widget;

   widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget) return FALSE;

   if (selection_num != 0 || !elm_entry_selection_get(widget)) return FALSE;

   elm_entry_select_none(widget);
   EAIL_ENTRY(text)->selection_start = 0;
   EAIL_ENTRY(text)->selection_end = 0;

   return TRUE;
}

/**
 * @brief Sets text selection for entry
 *
 * The selected regions are assigned
 * numbers that correspond to how far the region is from the start of the text.
 * The selected region closest to the beginning of the text region is assigned
 * the number 0, etc. Note that adding, moving or deleting a selected region can
 * change the numbering.
 *
 * Implementation of AtkTextIface->set_selection callback.
 *
 * @param text AtkText instance
 * @param selection_num selection number
 * @param start_pos start position of the selected region
 * @param end_pos end position of the selected region
 *
 * @returns TRUE on success, FALSE otherwise
 */
static gboolean
eail_entry_set_selection(AtkText *text,
                         gint     selection_num,
                         gint     start_pos,
                         gint     end_pos)
{
   if (0 != selection_num)
     return FALSE;

   return eail_entry_add_selection(text, start_pos, end_pos);
}

/**
 * @brief Gets text bounded by start_offset and end_offset
 *
 * Use g_free() to free the returned string.
 *
 * @param text AtkText instance
 * @param start_offset start position
 * @param end_offset end position, -1 for the end of the string
 * @return string containing text from start_offset up to,
 * but not including end_offset
 */
static gchar*
eail_entry_get_text(AtkText   *text,
                    gint       start_offset,
                    gint       end_offset)
{
   const gchar *string = NULL;
   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(text));

   if (widget)
     string = elm_entry_entry_get(widget);

   return eail_get_substring(string, start_offset, end_offset);
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
eail_entry_get_text_after_offset(AtkText *text,
                                 gint offset,
                                 AtkTextBoundary boundary_type,
                                 gint *start_offset,
                                 gint *end_offset)
{
   Evas_Object *widget;
   Evas_Object *textblock;

   widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget) return NULL;

   textblock = elm_entry_textblock_get(widget);

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
eail_entry_get_text_at_offset(AtkText *text,
                              gint offset,
                              AtkTextBoundary boundary_type,
                              gint *start_offset,
                              gint *end_offset)
{
   Evas_Object *widget;
   Evas_Object *textblock;

   widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget) return NULL;

   textblock = elm_entry_textblock_get(widget);

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
eail_entry_get_text_before_offset(AtkText *text,
                                  gint offset,
                                  AtkTextBoundary boundary_type,
                                  gint *start_offset,
                                  gint *end_offset)
{
   Evas_Object *widget;
   Evas_Object *textblock;

   widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget) return NULL;

   textblock = elm_entry_textblock_get(widget);

   return eail_get_text_before(textblock, offset, boundary_type, start_offset,
                               end_offset);
}

/**
 * @brief Gets the character at offset
 *
 * @param text AtkText instance
 * @param offset character offset
 * @return char representing the character at offset
 */
static gunichar
eail_entry_get_character_at_offset(AtkText    *text,
                                   gint        offset)
{
   gunichar character = '\0';
   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(text));

   if (widget)
     character = g_utf8_get_char(
         g_utf8_offset_to_pointer(elm_entry_entry_get(widget), offset));

   return character;
}

/**
 * @brief Gets the text's length
 *
 * @param text AtkText instance
 * @return integer representing the text length
 */
static gint
eail_entry_get_character_count(AtkText *text)
{
   gint count = 0;
   const gchar *string_text = NULL;

   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(text));

   if (!widget) return count;

   string_text = elm_object_text_get(widget);
   if (!string_text) return count;

   count = g_utf8_strlen(string_text, -1);

   return count;
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
eail_entry_get_default_attributes(AtkText *text)
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
eail_entry_get_run_attributes(AtkText *text,
                              gint offset,
                              gint *start_offset,
                              gint *end_offset)
{
   AtkAttributeSet *at_set = NULL;
   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(text));

   if (!widget || offset >= eail_entry_get_character_count(text))
     {
        *start_offset = -1;
        *end_offset = -1;

        return NULL;
     }

   *start_offset = 0;
   *end_offset = eail_entry_get_character_count(text);

   /* NOTE: Elm_Wrap_Type value is in 100% compatible with ATK wrap modes, so
    * no additional conversion is needed*/
   Elm_Wrap_Type wrap_type = elm_entry_line_wrap_get(widget);
   at_set = eail_utils_text_add_attribute
       (at_set, ATK_TEXT_ATTR_WRAP_MODE,
        atk_text_attribute_get_value
         (ATK_TEXT_ATTR_WRAP_MODE, wrap_type));

   Eina_Bool editable = elm_entry_editable_get(widget);
   at_set = eail_utils_text_add_attribute
       (at_set, ATK_TEXT_ATTR_EDITABLE,
        atk_text_attribute_get_value
         (ATK_TEXT_ATTR_EDITABLE, editable));

   return at_set;
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
eail_entry_get_character_extents(AtkText *text,
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

   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(text));

   if (!widget)
   {
      return;
   }

   textblock = elm_entry_textblock_get(widget);
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

/*
 * @brief Gets the offset of the character located at coordinates
 * x and y. x and y are interpreted as being relative to the
 *  screen or this widget's window depending on coords.
 *
 * @param text AtkText instance
 * @param x screen x-position of character
 * @param y screen y-position of character
 * @param coords specify whether coordinates are relative to the
 * screen or widget window
 *
 * @returns the offset to the character which is located at the
 * specified x and y coordinates.
 */
static gint
eail_entry_get_offset_at_point(AtkText *text,
                               gint x,
                               gint y,
                               AtkCoordType coords)
{
   Eina_Bool result = EINA_FALSE;
   Evas_Object *textblock = NULL;
   Evas_Textblock_Cursor *cur = NULL;
   gint offset = -1;

   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(text));

   if (!widget) return offset;

   if (coords == ATK_XY_SCREEN)
   {
      int ee_x, ee_y;
      Ecore_Evas *ee= ecore_evas_ecore_evas_get(evas_object_evas_get(widget));

      ecore_evas_geometry_get(ee, &ee_x, &ee_y, NULL, NULL);
      x -= ee_x;
      y -= ee_y;
    }

   textblock = elm_entry_textblock_get(widget);
   if (!textblock) return offset;

   cur = evas_object_textblock_cursor_new(textblock);
   if (!cur) return offset;

   result = evas_textblock_cursor_char_coord_set(cur, x, y);

   if (result == EINA_FALSE)
   {
      evas_textblock_cursor_free(cur);
      return offset;
   }

   offset = evas_textblock_cursor_pos_get(cur);
   evas_textblock_cursor_free(cur);

   return offset;
}

/**
 * @brief Initializes AtkTextIface interface
 *
 * @param iface AtkTextIface instance
 */
static void
atk_text_interface_init(AtkTextIface *iface)
{
   iface->get_caret_offset = eail_entry_get_caret_offset;
   iface->set_caret_offset = eail_entry_set_caret_offset;
   iface->get_selection    = eail_entry_get_selection;
   iface->set_selection    = eail_entry_set_selection;
   iface->get_text         = eail_entry_get_text;
   iface->get_character_at_offset = eail_entry_get_character_at_offset;
   iface->get_character_count = eail_entry_get_character_count;
   iface->remove_selection = eail_entry_remove_selection;
   iface->get_n_selections = eail_entry_get_n_selections;
   iface->add_selection = eail_entry_add_selection;
   iface->get_run_attributes = eail_entry_get_run_attributes;
   iface->get_default_attributes = eail_entry_get_default_attributes;
   iface->get_text_after_offset = eail_entry_get_text_after_offset;
   iface->get_text_at_offset = eail_entry_get_text_at_offset;
   iface->get_text_before_offset = eail_entry_get_text_before_offset;
   iface->get_character_extents = eail_entry_get_character_extents;
   iface->get_offset_at_point = eail_entry_get_offset_at_point;
}

/*
 * Implementation of the *AtkEditableText* interface
 */

/**
 * @brief Sets text content of entry
 *
 * @param text AtkEditableText instance
 * @param string string to be set as the content of entry
 */
static void
eail_entry_set_text_contents(AtkEditableText *text,
                             const gchar     *string)
{
   Evas_Object *widget;

   widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (widget && elm_entry_editable_get(widget))
     elm_object_text_set(widget, string);
}

/**
 * @brief Copies text content from entry to clipboard
 *
 * @param text AtkEditableText instance
 * @param start_pos start position of the cursor
 * @param end_pos end position of the cursor
 */
static void
eail_entry_copy_text(AtkEditableText *text,
                     gint             start_pos,
                     gint             end_pos)
{
   Evas_Object *widget;
   const char *entry_text;
   char *tmp;

   widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget) return;

   entry_text = elm_entry_entry_get(widget);
   tmp = eail_get_substring(entry_text, start_pos, end_pos);
   eail_clipboard_set_text(tmp);
   g_free(tmp);
}

/**
 * @brief Cuts text content from entry to clipboard
 *
 * @param text AtkEditableText instance
 * @param start_pos start position of the cursor
 * @param end_pos end position of the cursor
 */
static void
eail_entry_cut_text(AtkEditableText *text,
                    gint             start_pos,
                    gint             end_pos)
{
   Evas_Object *widget;
   const char *entry_text;
   char *tmp;
   GString *s;

   widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget || !elm_entry_editable_get(widget))
     return;

   entry_text = elm_entry_entry_get(widget);
   tmp = eail_get_substring(entry_text, start_pos, end_pos);
   eail_clipboard_set_text(tmp);
   g_free(tmp);

   s = g_string_new(entry_text);
   s = g_string_erase(s, start_pos, end_pos - start_pos);

   elm_entry_entry_set(widget, s->str);
   g_string_free(s, TRUE);
}

/**
 * @brief Pastes text content from clipboard into entry
 *
 * @param text AtkEditableText instance
 * @param position start position of the cursor
 */
static void
eail_entry_paste_text(AtkEditableText *text,
                      gint             position)
{
   Evas_Object *widget;
   GString *s;
   const char *clip;

   widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget || !elm_entry_editable_get(widget))
     return;

   clip = eail_clipboard_get_text();
   if (!clip) return;

   s = g_string_new(elm_entry_entry_get(widget));
   s = g_string_insert(s, position, clip);
   elm_entry_entry_set(widget, s->str);
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
eail_entry_delete_text(AtkEditableText *text,
                       gint start_pos,
                       gint end_pos)
{
   Evas_Object *widget;
   GString *entry_text;

   widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget || !elm_entry_editable_get(widget))
     return;

   entry_text = g_string_new(elm_entry_entry_get(widget));
   entry_text = g_string_erase(entry_text, start_pos, end_pos-start_pos);

   elm_entry_entry_set(widget, entry_text->str);
   g_string_free(entry_text, TRUE);
}

/**
 * @brief Inserts text at the specified position
 *
 * After the call position points to the position after the newly inserted text.
 *
 * @param text AtkEditableText instance
 * @param string string to insert
 * @param length string's length
 * @param [out] position position at which text is inserted
 */
static void
eail_entry_insert_text(AtkEditableText *text,
                       const gchar *string,
                       gint length,
                       gint *position)
{
   Evas_Object *widget;
   GString *entry_text;

   widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget || !elm_entry_editable_get(widget))
     return;

   entry_text = g_string_new(elm_entry_entry_get(widget));
   entry_text = g_string_insert_len(entry_text, *position, string,
                                    length);
   elm_entry_entry_set(widget, entry_text->str);
   *position += length;
   g_string_free(entry_text, TRUE);
}

/**
 * @brief Initialization for AtkEditableTextIface interface
 *
 * Function called upon instance creation. It initializes AtkText interface
 * implementation i.e hooks method pointers in the interface structure
 * to the implementing class's implementation.
 *
 * @param iface AtkEditableTextIface instance
 */
static void
atk_editable_text_interface_init(AtkEditableTextIface *iface)
{
   iface->set_text_contents = eail_entry_set_text_contents;
   iface->copy_text = eail_entry_copy_text;
   iface->cut_text = eail_entry_cut_text;
   iface->paste_text = eail_entry_paste_text;
   iface->delete_text = eail_entry_delete_text;
   iface->insert_text = eail_entry_insert_text;
}
