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
 * @brief Implementation of entry widget
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
 * ATK_TYPE_EDITABLE_TEXT interfaces implemented
 */
G_DEFINE_TYPE_WITH_CODE(EailEntry,
                        eail_entry,
                        EAIL_TYPE_WIDGET,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_TEXT,
                                              atk_text_interface_init)
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_EDITABLE_TEXT,
                                              atk_editable_text_interface_init));

/**
 * @brief handler for event which is raised when entry content is being changed
 *
 * @param data passed to callback
 * @param obj object that raised event
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
 * @brief Initializer for AtkObject
 *
 * @param obj an AtkObject
 * @param data Initialization data
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
                                  _eail_entry_handle_changed_event, obj);
   evas_object_smart_callback_add(nested_widget, "press",
                                  _eail_entry_handle_pressed_event, obj);
}

/**
 * @brief Class destructor
 *
 * @param obj object instance
 */
static void
eail_entry_finalize(GObject *obj)
{
}

/**
 * @brief Implementation of AtkObject->ref_state_set callback
 *
 * ATK doc says:\n
 * Gets a reference to the state set of the accessible; the caller must
 * unreference it when it is no longer needed.
 *
 * @param obj an AtkObject
 *
 * @returns a reference to an AtkStateSet which is the state set of the
 * accessible.
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
 * @param entry an EailEntry
 */
static void
eail_entry_init(EailEntry *entry)
{
   entry->selection_start = 0;
   entry->selection_end = 0;
}

/**
 * @brief Initializer for EailPopup GObject class (defines callbacks for
 * base AtkObject)
 *
 * @param klass an EailEntryClass
 */
static void
eail_entry_class_init(EailEntryClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
   GObjectClass *gobject_class = G_OBJECT_CLASS(klass);

   class->initialize = eail_entry_initialize;
   class->ref_state_set = eail_entry_ref_state_set;
   gobject_class->finalize = eail_entry_finalize;
}

/**
 * @brief Gets caret offset
 *
 * @param text an AtkText
 * @return caret offset
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
 * @brief Sets offset for caret
 *
 * Implementation of AtkTextIface->set_caret_offset callback<br>
 * ATK doc says:
 * Sets the caret (cursor) position to the specified offset.
 *
 * @param text an AtkText
 * @param offset starting position
 *
 * @returns TRUE if success, FALSE otherwise.
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
 * @param text an AtkText
 * @returns number of selected text regions
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
 * Implementation of AtkTextIface->get_selection callback
 *
 * @param text an AtkText
 * @param selection_num The selection number. The selected regions are assigned
 * numbers that correspond to how far the region is from the start of the text.
 * The selected region closest to the beginning of the text region is assigned
 * the number 0, etc. Note that adding, moving or deleting a selected region can
 *  change the numbering.
 * @param start_offset passes back the start position of the selected region
 * @param end_offset passes back the end position of the selected region
 *
 * @returns a newly allocated string containing the selected text. Use g_free()
 * to free the returned string.
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
 * @param text an AtkText
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
 * selection_number equals 0.
 *
 * @param text an AtkText
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
 * Implementation of AtkTextIface->set_selection callback
 *
 * @param text an AtkText
 * @param selection_num The selection number. The selected regions are assigned
 * numbers that correspond to how far the region is from the start of the text.
 * The selected region closest to the beginning of the text region is assigned
 * the number 0, etc. Note that adding, moving or deleting a selected region can
 *  change the numbering.
 * @param start_pos start position of the selected region
 * @param end_pos end position of the selected region
 *
 * @returns TRUE if success, FALSE otherwise
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
 * Use g_free() to free the returned string
 *
 * @param text an AtkText
 * @param start_offset start position
 * @param end_offset end position, -1 for the end of the string
 * @return string containing text from start_offset up to, but not including
 * end_offset
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
 * @brief Returns the position that is count characters from the
 * given offset.
 *
 * count may be positive or negative.
 *
 * @param textblock an evas textblock
 * @param offset a character offset
 * @param count the number of characters to move from offset
 * @returns a new position
 */
static gint
_eail_move_chars(const Evas_Object *textblock,
                 gint offset,
                 gint count)
{
   Evas_Textblock_Cursor *cur;
   gint dir;
   gboolean res;

   dir = count > 0 ? 1 : -1;
   cur = evas_object_textblock_cursor_new(textblock);
   evas_textblock_cursor_pos_set(cur, offset);

   while(count)
     {
        res = dir > 0 ? evas_textblock_cursor_char_next(cur) :
            evas_textblock_cursor_char_prev(cur);
        if (!res) break;

        count -= dir;
     }

   offset = evas_textblock_cursor_pos_get(cur);
   evas_textblock_cursor_free(cur);

   return offset;
}

/**
 * @brief Gets utf8 character at offset
 *
 * @param textblock an evas textblock
 * @param offset a character offset
 * @returns a utf8 character
 */
static gunichar
_eail_get_unichar_at_offset(const Evas_Object *textblock,
                            int offset)
{
   Evas_Textblock_Cursor *cur;
   gchar *s;
   gunichar c;

   cur = evas_object_textblock_cursor_new(textblock);
   evas_textblock_cursor_pos_set(cur, offset);
   s = evas_textblock_cursor_content_get(cur);
   c = g_utf8_get_char(s);

   evas_textblock_cursor_free(cur);
   g_free(s);

   return c;
}

/**
 * @brief Checks whether character at offset in textblock is a word start
 *
 * @param textblock an evas textblock
 * @param offset a character offset
 * @returns TRUE on success, FALSE otherwise
 */
static gboolean
_eail_is_word_start(const Evas_Object *textblock,
                    gint offset)
{
   /* first character in a word */
   gunichar c1, c2;

   c1 = _eail_get_unichar_at_offset(textblock, offset);
   c2 = _eail_get_unichar_at_offset(textblock, offset - 1);
   if (g_unichar_isalnum(c1) && g_unichar_isspace(c2)) return TRUE;

   return FALSE;
}

/**
 * @brief Checks whether character at offset in textblock is a word end
 *
 * @param textblock an evas textblock
 * @param offset a character offset
 * @return TRUE on success, FALSE otherwise
 */
static gboolean
_eail_is_word_end(const Evas_Object *textblock,
                  gint offset)
{
   /* is first non-word char after a word */
   gunichar c1, c2;

   c1 = _eail_get_unichar_at_offset(textblock, offset - 1);
   c2 = _eail_get_unichar_at_offset(textblock, offset);
   if (g_unichar_isalnum(c1) && g_unichar_isspace(c2)) return TRUE;

   return FALSE;
}

/**
 * @brief Check whether character at offset in textblock is inside a word
 *
 * @param textblock an evas textblock
 * @param offset a character offset
 * @returns TRUE on success, FALSE otherwise
 */
static gboolean
_eail_is_inside_word(const Evas_Object *textblock,
                     gint offset)
{
   gunichar c1, c2, c3;

   c1 = _eail_get_unichar_at_offset(textblock, offset - 1);
   c2 = _eail_get_unichar_at_offset(textblock, offset);
   c3 = _eail_get_unichar_at_offset(textblock, offset + 1);
   if (g_unichar_isalnum(c1) && g_unichar_isalnum(c2) && g_unichar_isalnum(c3))
     return TRUE;

   if (g_unichar_isalnum(c1) && g_unichar_isalnum(c2) && g_unichar_isspace(c3))
     return TRUE;

   if (g_unichar_isspace(c1) && g_unichar_isalnum(c2) && g_unichar_isalnum(c3))
     return TRUE;

   if (g_unichar_isspace(c1) && g_unichar_isalnum(c2) && g_unichar_isspace(c3))
     return TRUE;

   return FALSE;
}

/**
 * @brief Gets texblock length
 *
 * @param textblock an evas textblock
 * @returns a textblock length
 */
static gint
_eail_get_len(const Evas_Object *textblock)
{
   Evas_Textblock_Cursor *cur;
   int ctr = 0;

   cur = evas_object_textblock_cursor_new(textblock);
   while (evas_textblock_cursor_char_next(cur))
     ++ctr;

   evas_textblock_cursor_free(cur);

   return ctr;
}

/**
 * @brief Returns the position that is count words from the given offset.
 *
 * count may  be positive or negative. If count is positive, the returned
 * position will be a word end, otherwise it will be a word start.
 *
 * @param textblock an evas textblock
 * @param offset a cahracter offset
 * @param count the number of words to move from offset
 * @returns a new position
 */
static gint
_eail_move_words(const Evas_Object *textblock,
                 gint offset,
                 gint count)
{
   gint len = _eail_get_len(textblock);

   while (count > 0 && offset < len)
     {
        do
          offset++;
        while (offset < len && !_eail_is_word_end(textblock, offset));

        count--;
     }
   while (count < 0 && offset > 0)
     {
        do
          offset--;
        while (offset > 0 && !_eail_is_word_start(textblock, offset));

        count++;
     }

   return offset;
}

/**
 * @brief Gets position of first character in line
 *
 * @param cur an Evas_Textblock_Cursor
 * @returns TRUE on success, FALSE otherwise
 */
static gint
_eail_get_line_start(Evas_Textblock_Cursor *cur)
{
   gint start = 0;
   gint pos;

   pos = evas_textblock_cursor_pos_get(cur);
   evas_textblock_cursor_line_char_first(cur);
   start = evas_textblock_cursor_pos_get(cur);
   evas_textblock_cursor_pos_set(cur, pos);

   return start;
}

/**
 * @brief Gets position of last character in line
 *
 * @param cur an Evas_Textblock_Cursor
 * @returns TRUE on success, FALSE otherwise
 */
static gint
_eail_get_line_end(Evas_Textblock_Cursor *cur)
{
   gint end = 0;
   gint pos;

   pos = evas_textblock_cursor_pos_get(cur);
   evas_textblock_cursor_line_char_last(cur);
   end = evas_textblock_cursor_pos_get(cur);
   evas_textblock_cursor_pos_set(cur, pos);

   return end;
}

/**
 * @brief Moves cursor to the beginning of the next line
 *
 * @param cur an Evas_Textblock_Cursor
 * @returns TRUE on success, FALSE otherwise
 */
static gboolean
_eail_iter_next_line(Evas_Textblock_Cursor *cur)
{
   evas_textblock_cursor_line_char_last(cur);

   return evas_textblock_cursor_char_next(cur);
}

/**
 * @brief Gets length of the line shown by cursor cur
 *
 * @param cur an Evas_Textblock_Cursor
 * @return line length
 */
static gint
_eail_get_line_length(Evas_Textblock_Cursor *cur)
{
   gint start;
   gint end;

   start = _eail_get_line_start(cur);
   end = _eail_get_line_end(cur);

   return end - start + 1;
}

/**
 * @brief Gets before at offset
 *
 * @param entry an Evas_Object
 * @param boundary_type an AtkTextBoundary
 * @param offset position
 * @param [out] start_offset the start offset of the returned string
 * @param [out] end_offset the offset of the first character after the
 * returned substring
 */
static void
_eail_get_line_before(Evas_Object     *entry,
                      AtkTextBoundary  boundary_type,
                      gint             offset,
                      gint            *start_offset,
                      gint            *end_offset)
{
   Evas_Object *textblock;
   Evas_Textblock_Cursor *cur, *prev_cur = NULL, *prev_prev_cur = NULL;
   gint index, start_index, end_index;
   const gchar *text;
   gboolean found = FALSE;

   textblock = elm_entry_textblock_get(entry);
   cur = evas_object_textblock_cursor_new(textblock);

   text = evas_textblock_text_markup_to_utf8(
       textblock, evas_object_textblock_text_markup_get(textblock));
   index = g_utf8_offset_to_pointer (text, offset) - text;
   do
     {
        start_index = _eail_get_line_start(cur);
        end_index = start_index + _eail_get_line_length(cur);

        if (index >= start_index && index <= end_index)
          {
             /* Found line for offset */
             if (prev_cur)
               {
                  switch (boundary_type)
                    {
                      case ATK_TEXT_BOUNDARY_LINE_START:
                          end_index = start_index;
                          start_index = _eail_get_line_start(prev_cur);
                          break;
                      case ATK_TEXT_BOUNDARY_LINE_END:
                          if (prev_prev_cur)
                            start_index = _eail_get_line_start(prev_prev_cur) +
                                _eail_get_line_length(prev_prev_cur);
                          else
                            start_index = 0;
                          end_index = _eail_get_line_start(prev_cur) +
                              _eail_get_line_length(prev_cur);
                          break;
                      default:
                          g_assert_not_reached();
                    }
               }
             else
               start_index = end_index = 0;

             found = TRUE;
             break;
          }

        prev_prev_cur = prev_cur;
        prev_cur = cur;
     }
   while (_eail_iter_next_line(cur));

   if (!found)
     {
        start_index = _eail_get_line_start(prev_cur) +
            _eail_get_line_length(prev_cur);
        end_index = start_index;
     }
   evas_textblock_cursor_free(cur);

   *start_offset = g_utf8_pointer_to_offset(text, text + start_index);
   *end_offset = g_utf8_pointer_to_offset(text, text + end_index);
}

/**
 * @brief Gets line after offset
 *
 * @param entry an Evas_Object
 * @param boundary_type an AtkTextBoundary
 * @param offset position
 * @param [out] start_offset the start offset of the returned string
 * @param [out] end_offset the offset of the first character after the
 * returned substring
 */
static void
_eail_get_line_after (Evas_Object     *entry,
                      AtkTextBoundary  boundary_type,
                      gint             offset,
                      gint            *start_offset,
                      gint            *end_offset)
{
   Evas_Object *textblock;
   Evas_Textblock_Cursor *cur, *prev_cur = NULL;
   gint index, start_index, end_index;
   const gchar *text;
   gboolean found = FALSE;

   textblock = elm_entry_textblock_get(entry);
   cur = evas_object_textblock_cursor_new(textblock);
   text = evas_textblock_text_markup_to_utf8(
       textblock, evas_object_textblock_text_markup_get(textblock));

   index = g_utf8_offset_to_pointer (text, offset) - text;
   do
     {
        start_index = _eail_get_line_start(cur);
        end_index = start_index + _eail_get_line_length(cur);

        if (index >= start_index && index <= end_index)
          {
             /* Found line for offset */
             if (_eail_iter_next_line (cur))
               {
                  switch (boundary_type)
                    {
                      case ATK_TEXT_BOUNDARY_LINE_START:
                          start_index = _eail_get_line_start(cur);
                          if (_eail_iter_next_line (cur))
                            end_index = _eail_get_line_start(cur);
                          else
                            end_index = start_index + _eail_get_line_length(cur);
                          break;
                      case ATK_TEXT_BOUNDARY_LINE_END:
                          start_index = end_index;
                          end_index = _eail_get_line_start(cur) +
                              _eail_get_line_length(cur);
                          break;
                      default:
                          g_assert_not_reached();
                    }
               }
             else
               start_index = end_index;

             found = TRUE;
             break;
          }

        prev_cur = cur;
     }
   while (_eail_iter_next_line (cur));

   if (!found)
     {
        start_index = _eail_get_line_start(prev_cur) +
            _eail_get_line_length(prev_cur);
        end_index = start_index;
     }
   evas_textblock_cursor_free(cur);

   *start_offset = g_utf8_pointer_to_offset (text, text + start_index);
   *end_offset = g_utf8_pointer_to_offset (text, text + end_index);
}

/**
 * @brief Gets line at offset
 *
 * @param entry an Evas_Object
 * @param boundary_type an AtkTextBoundary
 * @param offset position
 * @param [out] start_offset the start offset of the returned string
 * @param [out] end_offset the offset of the first character after the
 * returned substring
 */
static void
_eail_get_line_at (Evas_Object *entry,
                   AtkTextBoundary  boundary_type,
                   gint             offset,
                   gint            *start_offset,
                   gint            *end_offset)
{
   Evas_Object *textblock;
   Evas_Textblock_Cursor *cur, *prev_cur = NULL;
   gint index, start_index, end_index;
   const gchar *text;
   gboolean found = FALSE;

   textblock = elm_entry_textblock_get(entry);
   cur = evas_object_textblock_cursor_new(textblock);

   text = evas_textblock_text_markup_to_utf8(
       textblock, evas_object_textblock_text_markup_get(textblock));
   index = g_utf8_offset_to_pointer (text, offset) - text;
   //iter = pango_layout_get_iter (layout);
   do
     {
        //line = pango_layout_iter_get_line (iter);
        start_index = _eail_get_line_start(cur);
        end_index = start_index + _eail_get_line_end(cur);

        if (index >= start_index && index <= end_index)
          {
             /* Found line for offset */
             switch (boundary_type)
               {
                 case ATK_TEXT_BOUNDARY_LINE_START:
                     if (_eail_iter_next_line (cur))
                       end_index = _eail_get_line_start(cur);
                     break;
                 case ATK_TEXT_BOUNDARY_LINE_END:
                     if (prev_cur)
                       start_index = _eail_get_line_start(prev_cur) +
                           _eail_get_line_length(prev_cur);
                     break;
                 default:
                     g_assert_not_reached();
               }

             found = TRUE;
             break;
          }

        prev_cur = cur;
     }
   while (_eail_iter_next_line (cur));

   if (!found)
     {
        start_index = _eail_get_line_start(prev_cur) +
            _eail_get_line_length(prev_cur);
        end_index = start_index;
     }
   evas_textblock_cursor_free(cur);

   *start_offset = g_utf8_pointer_to_offset (text, text + start_index);
   *end_offset = g_utf8_pointer_to_offset (text, text + end_index);
}

/**
 * @brief Gets a slice of the text from entry after offset
 *
 * @param entry an entry widget
 * @param offset a character offset
 * @param boundary_type an AtkTextBoundary
 * @param [out] start_offset return location for the start of the returned text
 * @param [out] end_offset return location for the end of the returned text
 * @returns a newly allocated string containg a slice of text from textblock.
 * Free with g_free()
 */
static gchar *
_eail_get_text_after(Evas_Object *entry,
                     gint offset,
                     AtkTextBoundary boundary_type,
                     gint *start_offset,
                     gint *end_offset)
{
   const gchar *text;
   int len;
   gint start, end;
   Evas_Object *textblock;

   if (!entry) return g_strdup("");

   textblock = elm_entry_textblock_get(entry);

   text = evas_textblock_text_markup_to_utf8(
       textblock, evas_object_textblock_text_markup_get(textblock));
   if (!text)
     {
        *start_offset = 0;
        *end_offset = 0;
        return g_strdup("");
     }

   start = offset;
   end = offset;
   len = _eail_get_len(textblock);

   switch (boundary_type)
     {
       case ATK_TEXT_BOUNDARY_CHAR:
           start = _eail_move_chars(textblock, start, 1);
           end = start;
           end = _eail_move_chars(textblock, end, 1);
           break;

       case ATK_TEXT_BOUNDARY_WORD_START:
           if (_eail_is_inside_word(textblock, end))
             end = _eail_move_words(textblock, end, 1);
           while (!_eail_is_word_start(textblock, end) && end < len)
             end = _eail_move_chars(textblock, end, 1);
           start = end;
           if (end < len)
             {
                end = _eail_move_words(textblock, end, 1);
                while (!_eail_is_word_start(textblock, end) && end < len)
                  end = _eail_move_chars(textblock, end, 1);
             }
           break;

       case ATK_TEXT_BOUNDARY_WORD_END:
           end = _eail_move_words(textblock, end, 1);
           start = end;
           if (end < len)
             end = _eail_move_words(textblock, end, 1);
           break;

       case ATK_TEXT_BOUNDARY_SENTENCE_START:
       case ATK_TEXT_BOUNDARY_SENTENCE_END:
           break;

       case ATK_TEXT_BOUNDARY_LINE_START:
       case ATK_TEXT_BOUNDARY_LINE_END:
           _eail_get_line_after (entry, boundary_type, offset, &start, &end);
           break;
     }

   *start_offset = start;
   *end_offset = end;
   g_assert(start <= end);

   return g_utf8_substring(text, start, end);
}

/**
 * @brief Gets a slice of the text from entry at offset
 *
 * @param entry an entry widget
 * @param offset a character offset in entry
 * @param boundary_type an AtkTextBoundary
 * @param [out] start_offset return location for the start of the returned text
 * @param [out] end_offset return location for the end of the return text
 * @returns a newly allocated string containing a slice of text from entry.
 * Free with g_free().
 */
static gchar *
_eail_get_text_at(Evas_Object *entry,
                  gint offset,
                  AtkTextBoundary boundary_type,
                  gint *start_offset,
                  gint *end_offset)
{
   const gchar *text;
   int len;
   gint start, end;
   Evas_Object *textblock;

   if (!entry) return g_strdup("");

   textblock = elm_entry_textblock_get(entry);

   text = evas_textblock_text_markup_to_utf8(
       textblock, evas_object_textblock_text_markup_get(textblock));
   if (!text)
     {
        *start_offset = 0;
        *end_offset = 0;
        return g_strdup("");
     }

   start = offset;
   end = offset;
   len = _eail_get_len(textblock);

   switch (boundary_type)
     {
       case ATK_TEXT_BOUNDARY_CHAR:
           end = _eail_move_chars(textblock, end, 1);
           break;

       case ATK_TEXT_BOUNDARY_WORD_START:
           if (!_eail_is_word_start(textblock, start))
             start = _eail_move_words(textblock, start, -1);
           if (_eail_is_inside_word(textblock, end))
             end = _eail_move_words(textblock, end, 1);
           while (!_eail_is_word_start(textblock, end) && end < len)
             end = _eail_move_chars(textblock, end, 1);
           break;

       case ATK_TEXT_BOUNDARY_WORD_END:
           if (_eail_is_inside_word(textblock, start) &&
               !_eail_is_word_start(textblock, start))
             start = _eail_move_words(textblock, start, -1);
           while (!_eail_is_word_end(textblock, start) && start > 0)
             start = _eail_move_chars(textblock, start, -1);
           end = _eail_move_words(textblock, end, 1);
           break;

       case ATK_TEXT_BOUNDARY_SENTENCE_START:
       case ATK_TEXT_BOUNDARY_SENTENCE_END:
           break;

       case ATK_TEXT_BOUNDARY_LINE_START:
       case ATK_TEXT_BOUNDARY_LINE_END:
           _eail_get_line_at (entry, boundary_type, offset, &start, &end);
           break;
     }

   *start_offset = start;
   *end_offset = end;
   g_assert(start <= end);

   return g_utf8_substring(text, start, end);
}

/**
 * @brief Gets a slice of the text from entry before offset
 *
 * @param entry an entry widget
 * @param offset a character offset
 * @param boundary_type an AtkTextBoundary
 * @param start_offset return location for the start of the returned text
 * @param end_offset return location for the end of the returned text
 * @returns a newly allocated string containing a slice of text from entry.
 * Free with g_free().
 */
static gchar *
_eail_get_text_before(Evas_Object *entry,
                      gint offset,
                      AtkTextBoundary boundary_type,
                      gint *start_offset,
                      gint *end_offset)
{
   const gchar *text;
   gint start, end;
   Evas_Object *textblock;

   if (!entry) return g_strdup("");

   textblock = elm_entry_textblock_get(entry);

   text = evas_textblock_text_markup_to_utf8(
       textblock, evas_object_textblock_text_markup_get(textblock));
   if (!text)
     {
        *start_offset = 0;
        *end_offset = 0;
        return g_strdup("");
     }

   start = offset;
   end = offset;

   switch (boundary_type)
     {
       case ATK_TEXT_BOUNDARY_CHAR:
           start = _eail_move_chars(textblock, start, -1);
           break;

       case ATK_TEXT_BOUNDARY_WORD_START:
           if (!_eail_is_word_start(textblock, start))
             start = _eail_move_words(textblock, start, -1);
           end = start;
           start = _eail_move_words(textblock, start, -1);
           break;

       case ATK_TEXT_BOUNDARY_WORD_END:
           if (_eail_is_inside_word(textblock, start) &&
               !_eail_is_word_start(textblock, start))
             start = _eail_move_words(textblock, start, -1);
           while (!_eail_is_word_end(textblock, start) && start > 0)
             start = _eail_move_chars(textblock, start, -1);
           end = start;
           start = _eail_move_words(textblock, start, -1);
           while (!_eail_is_word_end(textblock, start) && start > 0)
             start = _eail_move_chars(textblock, start, -1);
           break;

       case ATK_TEXT_BOUNDARY_SENTENCE_START:
       case ATK_TEXT_BOUNDARY_SENTENCE_END:
           break;

       case ATK_TEXT_BOUNDARY_LINE_START:
       case ATK_TEXT_BOUNDARY_LINE_END:
           _eail_get_line_before (entry, boundary_type, offset, &start, &end);
           break;
     }

   *start_offset = start;
   *end_offset = end;
   g_assert(start <= end);

   return g_utf8_substring(text, start, end);
}

/**
 * @brief Gets the specified text
 *
 * @param text an AtkText
 * @param offset position
 * @param boundary_type an AtkTextBoundary
 * @param [out] start_offset the start offset of the returned string
 * @param [out] end_offset the offset of the first character after the returned
 * substring
 * @returns a newly allocated string containing the text after offset bounded
 * by the specified boundary_type. Use g_free() to free the returned string.
 */
static gchar *
eail_entry_get_text_after_offset(AtkText *text,
                                 gint offset,
                                 AtkTextBoundary boundary_type,
                                 gint *start_offset,
                                 gint *end_offset)
{
   Evas_Object *widget;

   widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget) return NULL;

   return _eail_get_text_after(widget, offset, boundary_type, start_offset,
                               end_offset);

}

/**
 * @brief Gets the specified text
 *
 * @param text an AtkText
 * @param offset position
 * @param boundary_type an AtkTextBoundary
 * @param [out] start_offset the start offset of the returned string
 * @param [out] end_offset the offset of the first character after the returned
 * substring
 * @returns a newly allocated string containing the text after offset bounded
 * by the specified boundary_type. Use g_free() to free the returned string.
 */
static gchar *
eail_entry_get_text_at_offset(AtkText *text,
                              gint offset,
                              AtkTextBoundary boundary_type,
                              gint *start_offset,
                              gint *end_offset)
{
   Evas_Object *widget;

   widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget) return NULL;

   return _eail_get_text_at(widget, offset, boundary_type, start_offset,
                            end_offset);
}

/**
 * @brief Gets the specified text
 *
 * @param text an AtkText
 * @param offset position
 * @param boundary_type an AtkTextBoundary
 * @param [out] start_offset the start offset of the returned string
 * @param [out] end_offset the offset of the first character after the returned
 * substring
 * @returns a newly allocated string containing the text after offset bounded
 * by the specified boundary_type. Use g_free() to free the returned string.
 */
static gchar *
eail_entry_get_text_before_offset(AtkText *text,
                                  gint offset,
                                  AtkTextBoundary boundary_type,
                                  gint *start_offset,
                                  gint *end_offset)
{
   Evas_Object *widget;

   widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget) return NULL;

   return _eail_get_text_before(widget, offset, boundary_type, start_offset,
                                end_offset);
}

/**
 * @brief Gets character at offset
 *
 * @param text an AtkText
 * @param offset character offset
 * @return character at offset
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
 * @brief Gets text length
 *
 * @param text an AtkText
 * @return text length
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
 * @brief Adds attribute to attribute set
 *
 * @param attrib_set The AtkAttributeSet to add the attribute to
 * @param attr The AtkTextAttrribute which identifies the attribute to be added
 * @param value The attribute value
 *
 * Creates an AtkAttribute from attr and value, and adds it
 * to attrib_set.
 *
 * @returns: A pointer to the new AtkAttributeSet.
 **/
AtkAttributeSet*
eail_entry_add_attribute(AtkAttributeSet *attrib_set,
                         AtkTextAttribute attr,
                         gchar           *value)
{
   AtkAttributeSet *return_set;
   AtkAttribute *at = g_malloc (sizeof (AtkAttribute));
   at->name = g_strdup (atk_text_attribute_get_name (attr));
   at->value = value;
   return_set = g_slist_prepend(attrib_set, at);
   return return_set;
}

/**
 * @brief Creates an AtkAttributeSet which consists of the default values of
 * attributes for the text.
 *
 * @param text an AtkText
 *
 * @returns an AtkAttributeSet which contains the default values of attributes
 * at offset. this atkattributeset should be freed by a call to
 * atk_attribute_set_free()
 */
AtkAttributeSet *
eail_entry_get_default_attributes(AtkText *text)
{
   AtkAttributeSet *at_set = NULL;

   at_set = eail_entry_add_attribute
       (at_set, ATK_TEXT_ATTR_WRAP_MODE,
        g_strdup
        (atk_text_attribute_get_value(ATK_TEXT_ATTR_WRAP_MODE, 0)));

   at_set = eail_entry_add_attribute
       (at_set, ATK_TEXT_ATTR_EDITABLE,
        g_strdup
        (atk_text_attribute_get_value
         (ATK_TEXT_ATTR_EDITABLE, TRUE)));

   return at_set;
}

/**
 * @brief Creates an AtkAttributeSet which consists of the attributes
 * explicitly set at the position offset in the text. start_offset and
 * end_offset are set to the start and end of the range around offset
 * where the attributes are invariant. Note that end_offset is the offset
 * of the first character after the range.
 *
 * @param text an AtkText
 * @param offset the offset at which to get the attributes
 * @param start_offset the address to put the start offset of the range (used to
 * store output value)
 * @param end_offset the address to put the end offset of the range (used to
 * store output value)
 *
 * @returns an AtkAttributeSet which contains the attributes explicitly set at
 * offset. This AtkAttributeSet should be freed by a call to
 * atk_attribute_set_free()
 */
AtkAttributeSet *
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
   at_set = eail_entry_add_attribute
       (at_set, ATK_TEXT_ATTR_WRAP_MODE,
        g_strdup
        (atk_text_attribute_get_value
         (ATK_TEXT_ATTR_WRAP_MODE, wrap_type)));

   Eina_Bool editable = elm_entry_editable_get(widget);
   at_set = eail_entry_add_attribute
       (at_set, ATK_TEXT_ATTR_EDITABLE,
        g_strdup
        (atk_text_attribute_get_value
         (ATK_TEXT_ATTR_EDITABLE, editable)));

   return at_set;
}

/**
 * @brief Initializes AtkTextIface interface
 *
 * @param iface an AtkTextIface
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
}

/*
 * Implementation of the *AtkEditableText* interface
 */

/**
 * @brief Set text content for entry
 * @param text an AtkEditableText
 * @param string string to be set
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
 * @param text an AtkEditableText
 * @param start_pos start position of cursor
 * @param end_pos end position of cursor
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
 * @param text an AtkEditableText
 * @param start_pos start position of cursor
 * @param end_pos end position of cursor
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
 * @param text an AtkEditableText
 * @param position start position of cursor
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
 * @param text an AtkEditableText
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
 * @brief Inserts text at given position
 *
 * @param text an AtkEditableText
 * @param string string to insert
 * @param length string length
 * @param [out] position at witch text is inserted.
 * After the call it points at the position after the newly inserted text.
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
 * @param iface an AtkEditableTextIface
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
