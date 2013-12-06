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
 * @file eail_fileselector_entry.c
 * @brief EailFileselectorEntry implementation
 */

#include <Elementary.h>

#include "eail_fileselector_entry.h"
#include "eail_factory.h"
#include "eail_utils.h"
#include "eail_priv.h"

static void atk_text_interface_init(AtkTextIface *iface);
static void eail_fileselector_entry_actions_init(EailActionWidget *widget);

#define ICON_CONTENT_NAME "button icon" /**< @brief content name for icon */
#define EAIL_FSE_ACTION_CLICK "click"/**< @brief 'click' action name */
#define EAIL_FSE_ACTION_LONGPRESS "longpress" /**< @brief 'longpress' action name */
#define EAIL_FSE_ACTION_PRESS "press" /**< @brief 'press' action name */
#define EAIL_FSE_ACTION_RELEASE "release" /**< @brief 'release' action name */

/**
 * @brief Definition of EailFileselectorEntry as GObject
 *
 * EailFileselectorEntry is extended EAIL_TYPE_ACTION_WIDGET with ATK_TYPE_TEXT
 * interface implemented.
 */
G_DEFINE_TYPE_WITH_CODE(EailFileselectorEntry,
                        eail_fileselector_entry,
                        EAIL_TYPE_ACTION_WIDGET,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_TEXT,
                                              atk_text_interface_init));

/**
 * @brief Initializer for AtkObject
 *
 * @param obj AtkObject instance
 * @param data initialization data
 */
static void
eail_fileselector_entry_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_fileselector_entry_parent_class) ->initialize(obj, data);
   obj->role = ATK_ROLE_GROUPING;

   EailActionWidget *action_widget = EAIL_ACTION_WIDGET(obj);

   /* Initializing possible widget actions table*/
   eail_fileselector_entry_actions_init(action_widget);
}

/**
 * @brief Initializer for EailEntry GObject implementation
 *
 * @param fileselector_entry EailFileselectorEntry instance
 */
static void
eail_fileselector_entry_init(EailFileselectorEntry *fileselector_entry)
{
   fileselector_entry->selection_start = 0;
   fileselector_entry->selection_end = 0;
}

/**
 * @brief Gets the accessible name of FileselectorEntry
 *
 * Name is the label-text on the button.
 *
 * Implementation of get_name from AtkObject interface.
 *
 * @param obj AtkObject instance
 *
 * @returns character string representing the accessible name of
 * the accessible
 */
static const gchar *
eail_fileselector_entry_get_name(AtkObject *obj)
{
   Evas_Object *fse_widget = NULL;
   const gchar *atk_name = NULL;

   /* returning name from ATK default implementation if available */
   atk_name = ATK_OBJECT_CLASS(eail_fileselector_entry_parent_class)->get_name(obj);
   if (atk_name)
     return atk_name;

   fse_widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!fse_widget) return NULL;

   return elm_object_part_text_get(fse_widget, "default");
}

/**
 * @brief Gets nested button from FileSelectorEntry
 *
 * @param obj AtkObject instance
 * @returns Evas_Object representing the file selector button
 * or NULL if one was not found
 */
static Evas_Object *
_eail_fileselector_entry_get_nested_button(AtkObject *obj)
{
   Evas_Object *button_widget = NULL, *fse_widget = NULL;

   fse_widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!fse_widget) return NULL;

   button_widget = elm_object_part_content_get(fse_widget, "elm.swallow.button");
   if (!button_widget)
     DBG("Button widget NOT found!");

   return button_widget;
}

/**
 * @brief Helper function for returning list of nested widgets in fileselector entry
 *
 * @param obj AtkObject instance
 *
 * @return Eina_list representing the list of Evas_Object* objects with nested widgets
 */
static Eina_List *
_eail_fileselector_entry_get_items(AtkObject *obj)
{
   Eina_List *items = NULL;
   Evas_Object *icon_widget = NULL, *fse_widget = NULL;

   fse_widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!fse_widget) return NULL;

   icon_widget = elm_object_part_content_get(fse_widget, ICON_CONTENT_NAME);
   if (!icon_widget) return NULL;

   items = eina_list_append(items, icon_widget);

   return items;
}

/**
 * @brief Gets the number of accessible children of the accessible
 *
 * Implementation of get_n_children from AtkObject interface.
 *
 * @param obj AtkObject instance
 *
 * @returns integer representing the number of accessible children of
 * the accessible
 */
static gint
eail_fileselector_entry_get_n_children(AtkObject *obj)
{
   gint n_items;
   Eina_List *items;

   items = _eail_fileselector_entry_get_items(obj);
   n_items = eina_list_count(items);

   eina_list_free(items);

   return n_items;
}

/**
 * @brief Gets a reference to the specified accessible child of the object
 *
 * The accessible children are 0-based so the first accessible child
 * is at index 0, the second at index 1 and so on.
 *
 * Implementation of get_n_children from AtkObject interface.
 *
 * @param obj AtkObject instance
 * @param i index of the child
 *
 * @returns AtkObject representing the specified accessible child of the
 * accessible
 */
static AtkObject *
eail_fileselector_entry_ref_child(AtkObject *obj, gint i)
{
   Eina_List *items = NULL;
   AtkObject *child = NULL;

   items = _eail_fileselector_entry_get_items(obj);
   if (eina_list_count(items) > i)
     {
        child = eail_factory_get_accessible(eina_list_nth(items, i));
        if (child)  g_object_ref(child);
     }

   eina_list_free(items);

   return child;
}

/**
 * @brief Destructor of FileselectorEntry class
 *
 * @param object GObject instance to be finalized
 */
static void
eail_fileselector_entry_finalize(GObject *object)
{
   G_OBJECT_CLASS(eail_fileselector_entry_parent_class)->finalize(object);
}

/**
 * @brief Initializer for EailFileselectorEntry GObject class
 *
 * Defines callbacks for base AtkObject.
 *
 * @param klass EailFileselectorEntryClass instance
 */
static void
eail_fileselector_entry_class_init(EailFileselectorEntryClass *klass)
{
   AtkObjectClass *atk_class = ATK_OBJECT_CLASS(klass);
   GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
   atk_class->initialize = eail_fileselector_entry_initialize;
   atk_class->get_name = eail_fileselector_entry_get_name;
   atk_class->get_n_children = eail_fileselector_entry_get_n_children;
   atk_class->ref_child = eail_fileselector_entry_ref_child;
   gobject_class->finalize = eail_fileselector_entry_finalize;
}

/*
 * Implementation of the *AtkText* interface
 */

/**
 * @brief Gets text content from item
 *
 * Implementation of get_text from AtkTextIface.
 *
 * Use g_free() to free the returned string.
 *
 * @param text AtkText instance
 * @param start_offset start position
 * @param end_offset end position, or -1 for the end of the string
 *
 * @returns newly allocated string containing the text from start_offset
 * up to, but not including end_offset
 */
static gchar*
eail_fileselector_entry_get_text(AtkText  *text,
                                 gint      start_offset,
                                 gint      end_offset)
{
   gchar *ret_str = NULL;
   Evas_Object *fse_widget = NULL;

   fse_widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!fse_widget) return NULL;

   ret_str = eail_get_substring
                (elm_fileselector_entry_path_get(fse_widget),
                 start_offset, end_offset);

   return ret_str;
}

/**
 * @brief Gets the character from popup at the given offset
 *
 * Implementation of get_character_at_offset from AtkTextIface.
 *
 * @param text AtkText instance
 * @param offset character offset
 *
 * @returns char representing the character at offset
 */
static gunichar
eail_fileselector_entry_get_character_at_offset(AtkText *text,
                                                gint     offset)
{
   gunichar character = '\0';
   gchar *entry_str = NULL;

   entry_str = eail_fileselector_entry_get_text(text, 0, -1);
   if (entry_str)
      {
         character = g_utf8_get_char
                           (g_utf8_offset_to_pointer(entry_str, offset));
         g_free(entry_str);
      }

   return character;
}

/**
 * @brief Gets the number of characters from text content in item
 *
 * Implementation of get_character_count from AtkTextIface.
 *
 * @param text AtkText instance
 *
 * @returns integer representing the number of characters in text nontent
 */
static gint
eail_fileselector_entry_get_character_count(AtkText *text)
{
   gint count = 0;
   gchar *entry_str = NULL;
   entry_str = eail_fileselector_entry_get_text(text, 0, -1);

    if (entry_str)
      {
         count = g_utf8_strlen(entry_str, -1);
         g_free(entry_str);
      }

    return count;
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
eail_fileselector_entry_get_text_after_offset(AtkText *text,
                                              gint offset,
                                              AtkTextBoundary boundary_type,
                                              gint *start_offset,
                                              gint *end_offset)
{
   Evas_Object *widget;
   Evas_Object *textblock;
   Evas_Object *entry = NULL;
   Evas_Object *fileselector_entry_edje_layer = NULL;

   widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget) return NULL;

   fileselector_entry_edje_layer = elm_layout_edje_get(widget);
   if (!fileselector_entry_edje_layer) return NULL;

   entry = edje_object_part_swallow_get(fileselector_entry_edje_layer,
                                        "elm.swallow.entry");
   if (!entry) return NULL;

   textblock = elm_entry_textblock_get(entry);
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
eail_fileselector_entry_get_text_at_offset(AtkText *text,
                                           gint offset,
                                           AtkTextBoundary boundary_type,
                                           gint *start_offset,
                                           gint *end_offset)
{
   Evas_Object *widget;
   Evas_Object *textblock;
   Evas_Object *entry = NULL;
   Evas_Object *fileselector_entry_edje_layer = NULL;

   widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget) return NULL;

   fileselector_entry_edje_layer = elm_layout_edje_get(widget);
   if (!fileselector_entry_edje_layer) return NULL;

   entry = edje_object_part_swallow_get(fileselector_entry_edje_layer,
                                        "elm.swallow.entry");
   if (!entry) return NULL;

   textblock = elm_entry_textblock_get(entry);
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
eail_fileselector_entry_get_text_before_offset(AtkText *text,
                                               gint offset,
                                               AtkTextBoundary boundary_type,
                                               gint *start_offset,
                                               gint *end_offset)
{
   Evas_Object *widget;
   Evas_Object *textblock;
   Evas_Object *entry = NULL;
   Evas_Object *fileselector_entry_edje_layer = NULL;

   widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget) return NULL;

   fileselector_entry_edje_layer = elm_layout_edje_get(widget);
   if (!fileselector_entry_edje_layer) return NULL;

   entry = edje_object_part_swallow_get(fileselector_entry_edje_layer,
                                        "elm.swallow.entry");
   if (!entry) return NULL;

   textblock = elm_entry_textblock_get(entry);
   if (!textblock) return NULL;

   return eail_get_text_before(textblock, offset, boundary_type, start_offset,
                               end_offset);
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
eail_fileselector_entry_get_character_extents(AtkText *text,
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
   Evas_Object *entry = NULL;
   Evas_Object *fileselector_entry_edje_layer = NULL;

   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget) return;

   fileselector_entry_edje_layer = elm_layout_edje_get(widget);
   if (!fileselector_entry_edje_layer) return;

   entry = edje_object_part_swallow_get(fileselector_entry_edje_layer,
                                        "elm.swallow.entry");
   if (!entry) return;

   textblock = elm_entry_textblock_get(entry);
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

/*
 * @brief Adds a selection bounded by the specified offsets
 *
 * @param text AtkText instance
 * @param start_offset start position of the selection
 * @param end_offset offset of the first character after selection
 * @returns TRUE on success, FALSE otherwise
 */
static gboolean
eail_fileselector_entry_add_selection(AtkText *text,
                                      gint start_offset,
                                      gint end_offset)
{
   Evas_Object *widget;
   Evas_Object *entry;
   Evas_Object *fileselector_entry_edje_layer = NULL;

   widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget)
     return FALSE;

   fileselector_entry_edje_layer = elm_layout_edje_get(widget);
   if (!fileselector_entry_edje_layer)
     return FALSE;

   entry = edje_object_part_swallow_get(fileselector_entry_edje_layer,
                                        "elm.swallow.entry");
   if (!entry)
     return FALSE;

   elm_entry_cursor_pos_set(entry, start_offset);
   elm_entry_cursor_selection_begin(entry);
   elm_entry_cursor_pos_set(entry, end_offset);
   elm_entry_cursor_selection_end(entry);

   EAIL_FILESELECTOR_ENTRY(text)->selection_start = start_offset;
   EAIL_FILESELECTOR_ENTRY(text)->selection_end = end_offset;

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
eail_fileselector_entry_remove_selection(AtkText *text,
                                         gint selection_num)
{
   Evas_Object *widget;
   Evas_Object *entry;
   Evas_Object *fileselector_entry_edje_layer = NULL;

   widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget)
     return FALSE;

   fileselector_entry_edje_layer = elm_layout_edje_get(widget);
   if (!fileselector_entry_edje_layer)
     return FALSE;

   entry = edje_object_part_swallow_get(fileselector_entry_edje_layer,
                                        "elm.swallow.entry");
   if (!entry)
     return FALSE;

   if (selection_num != 0 || !elm_entry_selection_get(entry))
     return FALSE;

   elm_entry_select_none(entry);
   EAIL_FILESELECTOR_ENTRY(text)->selection_start = 0;
   EAIL_FILESELECTOR_ENTRY(text)->selection_end = 0;

   return TRUE;
}

/**
 * @brief Sets text selection for fileselector_entry
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
eail_fileselector_entry_set_selection(AtkText *text,
                                      gint     selection_num,
                                      gint     start_pos,
                                      gint     end_pos)
{
   if (0 != selection_num)
     return FALSE;

   return eail_fileselector_entry_add_selection(text, start_pos, end_pos);
}

/**
 * @brief Gets text selection from fileselector_entry
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
eail_fileselector_entry_get_selection(AtkText *text,
                                      gint     selection_num,
                                      gint    *start_offset,
                                      gint    *end_offset)
{
   const char* selection;
   Evas_Object *widget;
   Evas_Object *entry;
   Evas_Object *fileselector_entry_edje_layer = NULL;

   if (0 != selection_num)
     return g_strdup("");

   widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget)
     return g_strdup("");

   fileselector_entry_edje_layer = elm_layout_edje_get(widget);
   if (!fileselector_entry_edje_layer)
     return g_strdup("");

   entry = edje_object_part_swallow_get(fileselector_entry_edje_layer,
                                        "elm.swallow.entry");
   if (!widget)
     return g_strdup("");

   selection = elm_entry_selection_get(entry);
   if (selection)
   {
     *start_offset = EAIL_FILESELECTOR_ENTRY(text)->selection_start;
     *end_offset = EAIL_FILESELECTOR_ENTRY(text)->selection_end;
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
eail_fileselector_entry_get_n_selections(AtkText *text)
{
   Evas_Object *widget;
   Evas_Object *entry;
   Evas_Object *fileselector_entry_edje_layer = NULL;

   widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget)
     return 0;

   fileselector_entry_edje_layer = elm_layout_edje_get(widget);
   if (!fileselector_entry_edje_layer)
     return 0;

   entry = edje_object_part_swallow_get(fileselector_entry_edje_layer,
                                        "elm.swallow.entry");
   if (!entry)
     return 0;

   if (elm_entry_selection_get(entry))
     return 1;

   return 0;
}

/**
 * @brief Gets the caret offset
 *
 * @param text AtkText instance
 * @return integer representing the caret offset
 */
static gint
eail_fileselector_entry_get_caret_offset(AtkText *text)
{
   Evas_Object *widget;
   Evas_Object *entry = NULL;
   Evas_Object *fileselector_entry_edje_layer = NULL;

   widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget) return 0;

   fileselector_entry_edje_layer = elm_layout_edje_get(widget);
   if (!fileselector_entry_edje_layer) return 0;

   entry = edje_object_part_swallow_get(fileselector_entry_edje_layer,
                                        "elm.swallow.entry");
   if (!entry) return 0;

   return elm_entry_cursor_pos_get(entry);
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
eail_fileselector_entry_set_caret_offset (AtkText *text,
                                          gint offset)
{
   Evas_Object *widget;
   Evas_Object *entry = NULL;
   Evas_Object *fileselector_entry_edje_layer = NULL;

   widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget) return FALSE;

   fileselector_entry_edje_layer = elm_layout_edje_get(widget);
   if (!fileselector_entry_edje_layer) return FALSE;

   entry = edje_object_part_swallow_get(fileselector_entry_edje_layer,
                                        "elm.swallow.entry");
   if (!entry) return FALSE;

   elm_entry_cursor_pos_set(entry, offset);

   return TRUE;
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
eail_fileselector_entry_get_default_attributes(AtkText *text)
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
eail_fileselector_entry_get_run_attributes(AtkText *text,
                                           gint offset,
                                           gint *start_offset,
                                           gint *end_offset)
{
   AtkAttributeSet *at_set = NULL;
   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(text));
   Evas_Object *entry;
   Evas_Object *fileselector_entry_edje_layer = NULL;

   if (!widget || offset >= eail_fileselector_entry_get_character_count(text))
     {
        *start_offset = -1;
        *end_offset = -1;

        return NULL;
     }

   fileselector_entry_edje_layer = elm_layout_edje_get(widget);
   if (!fileselector_entry_edje_layer)
     return NULL;

   entry = edje_object_part_swallow_get(fileselector_entry_edje_layer,
                                        "elm.swallow.entry");
   if (!entry)
     return NULL;

   *start_offset = 0;
   *end_offset = eail_fileselector_entry_get_character_count(text);

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
 * @brief Initializer for AtkTextIface interface
 *
 * @param iface AtkTextIface instance
 */
static void
atk_text_interface_init(AtkTextIface *iface)
{
    iface->get_text = eail_fileselector_entry_get_text;
    iface->get_character_at_offset = eail_fileselector_entry_get_character_at_offset;
    iface->get_character_count = eail_fileselector_entry_get_character_count;
    iface->get_text_after_offset = eail_fileselector_entry_get_text_after_offset;
    iface->get_text_at_offset = eail_fileselector_entry_get_text_at_offset;
    iface->get_text_before_offset = eail_fileselector_entry_get_text_before_offset;
    iface->get_character_extents = eail_fileselector_entry_get_character_extents;
    iface->get_selection = eail_fileselector_entry_get_selection;
    iface->set_selection = eail_fileselector_entry_set_selection;
    iface->get_n_selections = eail_fileselector_entry_get_n_selections;
    iface->add_selection = eail_fileselector_entry_add_selection;
    iface->remove_selection = eail_fileselector_entry_remove_selection;
    iface->get_caret_offset = eail_fileselector_entry_get_caret_offset;
    iface->set_caret_offset = eail_fileselector_entry_set_caret_offset;
    iface->get_run_attributes = eail_fileselector_entry_get_run_attributes;
    iface->get_default_attributes = eail_fileselector_entry_get_default_attributes;
}

/*
 * Implementation of the Actions
 */

/**
 * @brief Calls callback with given name
 *
 * @param action AtkAction instance
 * @param action_name action name to be called
 *
 * @returns TRUE on success, FALSE otherwise
 */
static gboolean
eail_fileselector_entry_do_action(AtkAction *action,
                                  const char *action_name)
{
   Evas_Object *button = NULL;

   button = _eail_fileselector_entry_get_nested_button(ATK_OBJECT(action));
   if (!button) return FALSE;

   if (action_name == NULL) return FALSE;
   /*emit signal*/
   evas_object_smart_callback_call(button, action_name, NULL);
   return TRUE;
}

/**
 * @brief Handle for 'click' action
 *
 * @param action AtkAction instance
 * @param data additional action data (not used here)
 *
 * @return TRUE if action was triggered successfully, FALSE otherwise
 */
static gboolean
eail_action_click(AtkAction *action, void *data)
{
   return eail_fileselector_entry_do_action(action, "clicked");
}

/**
 * @brief Calls action with given name for actions based on x-y coordinates
 *
 * @param action AtkAction instance
 * @param name name string of the action to be called
 *
 * @returns TRUE on success, FALSE otherwise
 */
static gboolean
_eail_action_handle_coords_action(AtkAction *action, gchar *name)
{
   Evas_Object *button = NULL;
   int x, y;

   if (name == NULL) return FALSE;

   button = _eail_fileselector_entry_get_nested_button(ATK_OBJECT(action));
   if (!button) return FALSE;

   /* getting coordinates of center of the widget to make sure, that
    * click will be performed on active widget area */
   eail_get_coords_widget_center(button, &x, &y);

   if (0 == g_strcmp0(name, EAIL_FSE_ACTION_PRESS))
        {
          DBG("Calling 'press' on button");
          eail_mouse_press_on_coords(button, x, y);
        }
   else if (0 == g_strcmp0(name, EAIL_FSE_ACTION_RELEASE))
      {
         DBG("Calling 'release' on button");
         eail_mouse_release_on_coords(button, x, y);
      }
   else
      {
         DBG("No action by given name %s", name);
         return FALSE;
      }

   return TRUE;
}
/**
 * @brief Handle for 'longpress' action
 *
 * @param action AtkAction instance
 * @param data additional action data (not used here)
 *
 * @return TRUE if action was triggered successfully, FALSE otherwise
 */
static gboolean
eail_action_longpress(AtkAction *action, void *data)
{
   return eail_fileselector_entry_do_action(action, "longpressed");
}

/**
 * @brief Handle for 'press' action
 *
 * @param action AtkAction instance
 * @param data additional action data (not used here)
 *
 * @return TRUE if action was triggered successfully, FALSE otherwise
 */
static gboolean
eail_action_press(AtkAction *action, void *data)
{
   /* this is being done to action press/release raise properly click event.
    * Without that for standard behavior of fileselector entry, nothing will
    * happen (that why not launching event but emulating mouse press)*/
   return _eail_action_handle_coords_action(action, EAIL_FSE_ACTION_PRESS);
}

/**
 * @brief Handle for 'release' action
 *
 * @param action AtkAction instance
 * @param data additional action data (not used here)
 *
 * @return TRUE if action was triggered successfully, FALSE otherwise
 */
static gboolean
eail_action_release(AtkAction *action, void *data)
{
   /* this is being done to action press/release raise properly click event.
    * Without that for standard behavior of fileselector entry, nothing will
    * happen (that why not launching event but emulating mouse release)*/
   return _eail_action_handle_coords_action(action, EAIL_FSE_ACTION_RELEASE);
}

/**
 * @brief Adds fileselector actions to actions table
 *
 * @param action_widget widget that implements EailActionWidget interface
 */
static void
eail_fileselector_entry_actions_init(EailActionWidget *action_widget)
{
   eail_action_widget_action_append(action_widget,
                                    EAIL_FSE_ACTION_CLICK, NULL,
                                    eail_action_click);
   eail_action_widget_action_append(action_widget,
                                    EAIL_FSE_ACTION_LONGPRESS, NULL,
                                    eail_action_longpress);
   eail_action_widget_action_append(action_widget,
                                    EAIL_FSE_ACTION_PRESS, NULL,
                                    eail_action_press);
   eail_action_widget_action_append(action_widget,
                                    EAIL_FSE_ACTION_RELEASE, NULL,
                                    eail_action_release);
}
