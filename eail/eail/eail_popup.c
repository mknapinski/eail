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
 * @brief Implementation of popup widget
 */

#include <atk/atk.h>
#include <Elementary.h>

#include "eail_popup.h"
#include "eail_notify.h"
#include "eail_factory.h"
#include "eail_utils.h"
#include "eail_priv.h"

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
 * @brief number of popup buttons
 */
#define EAIL_POPUP_NUM_BUTTONS 3
/**
 * @brief Part name buffer size
 */
#define EAIL_POPUP_CHAR_BUF_SIZE 30

/**
 * @brief Initializer for AtkObject
 * @param obj an AtkObject
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
 * @param button an EailPopup
 */
static void
eail_popup_init(EailPopup *button)
{
}

/**
 * @brief Helper function to getting "elm_popup" Evas_Object* from Atk EailPopup
 *
 * @param obj an AtkObject
 *
 * @returns nested "elm_popup" widget
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
 * @brief Helper function to getting nested content in "elm_popup" widget
 *
 * @param obj an AtkObject
 *
 * @returns nested widget content from "elm_popup" widget
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
 * @brief Helper function to getting nested button content in "elm_popup" widget
 *
 * @param obj an AtkObject
 * @param index index of button in popup widget
 *
 * @returns nested button widget from "elm_popup" widget or NULL if failed
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

   snprintf(buf, sizeof(buf), EAIL_POPUP_BUTTON_FORMAT, (index + 1));
   ret_button = elm_object_part_content_get(popup_widget, buf);

   return ret_button;
}

/**
 * @brief Prepares Eina_List filled with Evas_Object * objects
 * representing nested content in elm_popup widget
 *
 * @param obj an AtkObject
 *
 * @return filled list with Evas_Object* objects. Call eina_list_free on that
 * list when results processing has been finished
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
 * @brief Implementation AtkObject->get_n_children callback
 *
 * ATK doc says:\n
 * Gets the number of accessible children of the accessible.
 * @param obj an AtkObject
 *
 * @returns an integer representing the number of accessible children of
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
 * @brief Implementation AtkObject->ref_child callback
 *
 * ATK doc says:\n
 * Gets a reference to the specified accessible child of the object. The
 * accessible children are 0-based so the first accessible child is at index 0,
 * the second at index 1 and so on.
 *
 * @param obj an AtkObject
 * @param i index of a child
 *
 * @returns an AtkObject representing the specified accessible child of the
 * accessible.
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
 * @brief Gets name of EailItem
 *
 * Implementation of AtkObject->get_name callback
 *
 * ATK doc says:\n
 * Gets the accessible name of the accessible.
 *
 * @param obj an AtkObject
 *
 * @returns a character string representing the accessible description of
 * the accessible.
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
 */
static void
eail_popup_finalize(GObject *object)
{
   G_OBJECT_CLASS(eail_popup_parent_class)->finalize(object);
}

/**
 * @brief Initializer for EailPopup GObject class (defines callbacks for
 * base AtkObject)
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
 * @param popup an EailPopup instance
 *
 * @returns Eina_Strbuf* object with filled text content of a popup
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
 * @brief Gets text content from popup
 *
 * Implementation of AtkTextIface->get_text callback\n
 *
 * @param popup an AtkText
 * @param start_offset start position
 * @param end_offset end position, or -1 for the end of the string.
 *
 * @returns a newly allocated string containing the text from start_offset
 * up to, but not including end_offset. Use g_free() to free the returned
 * string.
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
 * @brief Gets character from popup at given offset
 *
 * Implementation of AtkTextIface->get_character_at_offset callback
 *
 * @param popup an AtkText
 * @param offset an offset
 *
 * @returns the character at offset.
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
 * @brief Gets character count from text content in popup<br>
 * Implementation of AtkTextIface->get_character_count callback
 *
 * @param popup an AtkText
 *
 * @returns the character count
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
 * @brief Initializer for AtkTextIface interface class (defines callbacks for
 * AtkTextIface)
 *
 * @param iface an AtkText interface
 */
static void
atk_text_interface_init(AtkTextIface *iface)
{
    iface->get_text = eail_popup_get_text;
    iface->get_character_at_offset = eail_popup_get_character_at_offset;
    iface->get_character_count = eail_popup_get_character_count;
}
