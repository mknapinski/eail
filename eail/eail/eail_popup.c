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
 * @file eail_popup.c
 * @brief EailPopup implementation
 */

#include <atk/atk.h>
#include <Elementary.h>

#include "eail_popup.h"
#include "eail_notify.h"
#include "eail_factory.h"
#include "eail_utils.h"
#include "eail_priv.h"
#include "eail_text.h"

static void atk_text_interface_init(AtkTextIface *iface);

/**
 * @brief Definition of EailPopup as GObject
 *
 * EailPopup is extended EAIL_TYPE_NOTIFY with ATK_TYPE_TEXT interface
 * implemented
 */
G_DEFINE_TYPE_WITH_CODE(EailPopup, eail_popup, EAIL_TYPE_NOTIFY,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_TEXT,
                                             atk_text_interface_init));

/**
 * @brief Part format
 */
#define EAIL_POPUP_BUTTON_FORMAT "button%d"
/**
 * @brief Number of popup buttons
 */
#define EAIL_POPUP_NUM_BUTTONS 3
/**
 * @brief Part name buffer size
 */
#define EAIL_POPUP_CHAR_BUF_SIZE 30

/**
 * @brief Initializer for AtkObject
 *
 * @param obj AtkObject instance
 * @param data initialization data
 */
static void
eail_popup_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_popup_parent_class) ->initialize(obj, data);
   obj->role = ATK_ROLE_POPUP_MENU;
}

/**
 * @brief EailPopup GObject instance initializer
 *
 * @param button EailPopup instance
 */
static void
eail_popup_init(EailPopup *button)
{
}

/**
 * @brief Helper function for getting elm_popup Evas_Object* from Atk EailPopup
 *
 * @param obj AtkObject instance
 *
 * @returns Evas_Object representing the nested elm_popup widget
 */
static Evas_Object *
_eail_get_popup_widget_from_atkobj(AtkObject *obj)
{
   Evas_Object *popup_widget = NULL;
   popup_widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!popup_widget)
     {
        ERR("No widget found for notification object!");
        return NULL;
     }

   return popup_widget;
}

/**
 * @brief Helper function for getting nested content in elm_popup widget
 *
 * @param obj AtkObject instance
 *
 * @returns Evas_Object representing the nested widget content from elm_popup widget
 */
static Evas_Object *
_eail_get_nested_widget(AtkObject *obj)
{
   Evas_Object *popup_widget = NULL, *nested_widget = NULL;

   /* getting widget of popup class */
   popup_widget = _eail_get_popup_widget_from_atkobj(obj);
   if (!popup_widget) return NULL;

   nested_widget = elm_object_part_content_get(popup_widget, "default");

   return nested_widget;
}

/**
 * @brief Helper function for getting nested button content in elm_popup widget
 *
 * @param obj AtkObject instance
 * @param index index of button in popup widget
 *
 * @returns Evas_Object representing the nested button widget
 * from elm_popup widget or NULL in case of a failure
 */
static Evas_Object *
_eail_get_nested_popup_button(AtkObject *obj, gint index)
{
   gchar buf[EAIL_POPUP_CHAR_BUF_SIZE];
   Evas_Object *ret_button = NULL, *popup_widget = NULL;

   if (index >= EAIL_POPUP_NUM_BUTTONS)
     {
        ERR("Index of popup button cannot be >= %d", EAIL_POPUP_NUM_BUTTONS);
        return NULL;
     }

   popup_widget = _eail_get_popup_widget_from_atkobj(obj);
   if (!popup_widget) return NULL;

   g_snprintf(buf, sizeof(buf), EAIL_POPUP_BUTTON_FORMAT, (index + 1));
   ret_button = elm_object_part_content_get(popup_widget, buf);

   return ret_button;
}

/**
 * @brief Prepares Eina_List filled with Evas_Object * objects
 * representing nested content in elm_popup widget
 *
 * Call eina_list_free on the returned list when results processing has been finished.
 *
 * @param obj AtkObject instance
 *
 * @return Eina_List representing the list of Evas_Object* objects
 */
static Eina_List *
_eail_popup_get_items(AtkObject *obj)
{
   Eina_List *items = NULL;
   Evas_Object *widget = NULL;
   gint i = 0;

   /* nested widget */
   widget = _eail_get_nested_widget(obj);
   if (widget)
     items = eina_list_append(items, widget);

   /* action buttons below popup content */
   for (i = 0; i < EAIL_POPUP_NUM_BUTTONS; ++i)
     {
        widget = _eail_get_nested_popup_button(obj, i);
        if (widget)
          items = eina_list_append(items, widget);
     }

   return items;
}

/**
 * @brief Gets the number of accessible children of the accessible
 *
 * Implementation of AtkObject->get_n_children callback.
 *
 * @param obj AtkObject instance
 *
 * @returns integer representing the number of accessible children of
 * the accessible
 */
static gint
eail_popup_get_n_children(AtkObject *obj)
{
   Eina_List *items = NULL;
   gint num_of_children = 0;

   items = _eail_popup_get_items(obj);
   num_of_children = eina_list_count(items);

   return num_of_children;
}

/**
 * @brief Gets a reference to the specified accessible child of the object.
 *
 * The accessible children are 0-based so the first accessible child is at index 0,
 * the second at index 1 and so on.
 *
 * Implementation of AtkObject->ref_child callback.
 *
 * @param obj AtkObject instance
 * @param i index of a child
 *
 * @returns AtkObject representing the specified accessible child of the
 * accessible
 */
static AtkObject *
eail_popup_ref_child(AtkObject *obj, gint i)
{
   Eina_List *items = NULL;
   AtkObject *atk_obj = NULL;

   items = _eail_popup_get_items(obj);
   if (eina_list_count(items) > i)
     atk_obj = eail_factory_get_accessible(eina_list_nth(items, i));

   if (atk_obj)
     g_object_ref(atk_obj);

   return atk_obj;
}

/**
 * @brief Gets the accessible name of EailItem
 *
 * Implementation of AtkObject->get_name callback.
 *
 * @param obj AtkObject instance
 *
 * @returns character string representing the accessible description of
 * the accessible
 */
static const gchar *
eail_popup_get_name(AtkObject *obj)
{
   Evas_Object *popup_widget = NULL;
   const char *atk_name;

   atk_name = ATK_OBJECT_CLASS(eail_popup_parent_class)->get_name(obj);
   if (atk_name) return atk_name;

   popup_widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!popup_widget)
     {
        ERR("No widget found for notification object!");
        return NULL;
     }

   return elm_object_part_text_get(popup_widget, "title,text");
}

/**
 * @brief Destructor for EailPopup class
 * @param object GObject instance
 */
static void
eail_popup_finalize(GObject *object)
{
   G_OBJECT_CLASS(eail_popup_parent_class)->finalize(object);
}

/**
 * @brief Initializer for EailPopup GObject class
 *
 * Defines callbacks for base AtkObject.
 *
 * @param klass EailPopupClass instance
 */
static void
eail_popup_class_init(EailPopupClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
   GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
   class->initialize = eail_popup_initialize;
   class->get_n_children = eail_popup_get_n_children;
   class->ref_child = eail_popup_ref_child;
   class->get_name = eail_popup_get_name;
   gobject_class->finalize = eail_popup_finalize;
}

/*
 * AtkText interface functions
 */

/**
 * @brief Helper function that gets text from popup as Eina_Strbuf*
 *
 * @param popup AtkText instance
 *
 * @returns Eina_Strbuf representing the text content of the popup
 */
static Eina_Strbuf *
_eail_popup_get_strbuf(AtkText *popup)
{
   Evas_Object *popup_widget = NULL;
   Eina_Strbuf* str_buf = NULL;
   const gchar *text_part = NULL;

   g_return_val_if_fail (ATK_IS_OBJECT(popup), NULL);

   /* getting widget of popup class */
   popup_widget = _eail_get_popup_widget_from_atkobj(ATK_OBJECT(popup));
   if (!popup_widget) return NULL;

   text_part = elm_object_part_text_get(popup_widget, "default");
   if (!text_part) return NULL;

   str_buf = eina_strbuf_new();
   eina_strbuf_append(str_buf, text_part);

   return str_buf;
}

/**
 * @brief Gets the text content from popup
 *
 * Use g_free() to free the returned string.
 *
 * Implementation of AtkTextIface->get_text callback.
 *
 * @param popup AtkText instance
 * @param start_offset start position
 * @param end_offset end position, or -1 for the end of the string.
 *
 * @returns newly allocated string containing the text from start_offset
 * up to, but not including end_offset
 */
static gchar*
eail_popup_get_text(AtkText          *popup,
                    gint              start_offset,
                    gint              end_offset)
{
   Eina_Strbuf* str_buf = NULL;
   gchar *ret_str = NULL;

   g_return_val_if_fail(EAIL_IS_POPUP(popup), NULL);

   str_buf = _eail_popup_get_strbuf(popup);
   if (!str_buf) return NULL;

   ret_str = eail_get_substring
               (eina_strbuf_string_get(str_buf), start_offset, end_offset);

   eina_strbuf_free(str_buf);

   return ret_str;
}

/**
 * @brief Gets the character from popup at given offset
 *
 * Implementation of AtkTextIface->get_character_at_offset callback,
 *
 * @param popup AtkText instance
 * @param offset offset
 *
 * @returns char representing the character at offset
 */
static gunichar
eail_popup_get_character_at_offset(AtkText *popup, gint offset)
{
   gunichar character = '\0';
   gchar* time_str = NULL;

   g_return_val_if_fail(EAIL_IS_POPUP(popup), character);

   time_str = eail_popup_get_text(popup, 0, -1);

   if (time_str)
     {
         character = g_utf8_get_char
                  (g_utf8_offset_to_pointer(time_str, offset));
         g_free(time_str);
     }

   return character;
}

/**
 * @brief Gets the character count from text content in popup
 *
 * Implementation of AtkTextIface->get_character_count callback.
 *
 * @param popup AtkText instance
 *
 * @returns char representing the the character count
 */
static gint
eail_popup_get_character_count(AtkText *popup)
{
   gint count = 0;
   gchar* time_str = NULL;

   g_return_val_if_fail(EAIL_IS_POPUP(popup), 0);

   time_str = eail_popup_get_text(popup, 0, -1);

   if (time_str)
     {
         count = g_utf8_strlen(time_str, -1);
         g_free(time_str);
     }

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
eail_popup_get_character_extents(AtkText *text,
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
   Evas_Object *label = NULL;
   Evas_Object *layout = NULL;
   Evas_Object *layout_edje_layer = NULL;
   Evas_Object *label_edje_layer = NULL;
   Evas_Object *popup_edje_layer = NULL;

   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget) return;

   popup_edje_layer = elm_layout_edje_get(widget);
   if (!popup_edje_layer) return;

   layout = edje_object_part_swallow_get(popup_edje_layer, "elm.swallow.content");
   if (!layout) return;

   layout_edje_layer = elm_layout_edje_get(layout);
   if (!layout_edje_layer) return;

   label = edje_object_part_swallow_get(layout_edje_layer, "elm.swallow.content");
   if (!label) return;

   label_edje_layer = elm_layout_edje_get(label);
   if (!label_edje_layer) return;

   textblock = edje_object_part_object_get(label_edje_layer, "elm.text");
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
eail_popup_get_run_attributes(AtkText *text,
                              gint offset,
                              gint *start_offset,
                              gint *end_offset)
{
   AtkAttributeSet *at_set = NULL;
   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(text));
   gint len = eail_popup_get_character_count(ATK_TEXT(text));

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
   Elm_Wrap_Type wrap_type = elm_popup_content_text_wrap_type_get(widget);
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
eail_popup_get_default_attributes(AtkText *text)
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
 * @brief Initializer for AtkTextIface interface class
 *
 * Defines callbacks for AtkTextIface.
 *
 * @param iface AtkTextIface instance
 */
static void
atk_text_interface_init(AtkTextIface *iface)
{
    iface->get_text = eail_popup_get_text;
    iface->get_character_at_offset = eail_popup_get_character_at_offset;
    iface->get_character_count = eail_popup_get_character_count;
    iface->get_character_extents = eail_popup_get_character_extents;
    iface->get_run_attributes = eail_popup_get_run_attributes;
    iface->get_default_attributes = eail_popup_get_default_attributes;
}
