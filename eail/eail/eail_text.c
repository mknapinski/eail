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
 * @file eail_text.c
 * @brief EailText implementation
 */

#include <Elementary.h>

#include "eail_text.h"
#include "eail_factory.h"
#include "eail_utils.h"

/*
 * Implementation of the *AtkObject* interface
 */

static void atk_text_interface_init(AtkTextIface *iface);

/**
 * @brief EailText type definition
 */
G_DEFINE_TYPE_WITH_CODE(EailText,
                        eail_text,
                        EAIL_TYPE_WIDGET,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_TEXT,
                                              atk_text_interface_init));

/**
 * @brief EailText initializer
 *
 * @param obj AtkObject instance
 * @param data initialization data
 */
static void
eail_text_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_text_parent_class)->initialize(obj, data);

   obj->role = ATK_ROLE_TEXT;
}

/**
 * @brief Gets the accessible name of obj
 *
 * Returns obj accessible name if assigned, obj text content otherwise
 *
 * @param obj AtkObject instance
 * @returns string representing the accessible name of obj if assigned,
 * obj text content otherwise
 */
static const gchar*
eail_text_get_name(AtkObject *obj)
{
   const gchar *name;
   Evas_Object *widget;

   name = ATK_OBJECT_CLASS(eail_text_parent_class)->get_name(obj);
   if (NULL != name)
     return name;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (widget)
     name = (const gchar*)elm_object_text_get(widget);

   return name;
}

/**
 * @brief EailText instance initializer
 *
 * @param text EailText instance
 */
static void
eail_text_init(EailText *text)
{
}

/**
 * @brief EailText class initializer
 *
 * @param klass EailTextClass instance
 */
static void
eail_text_class_init(EailTextClass *klass)
{
   AtkObjectClass *atk_class = ATK_OBJECT_CLASS(klass);

   atk_class->initialize = eail_text_initialize;
   atk_class->get_name = eail_text_get_name;
}

/*
 * Implementation of the *AtkText* interface
 */

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
eail_text_get_text(AtkText   *text,
                   gint       start_offset,
                   gint       end_offset)
{
   gchar *string = NULL;
   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(text));

   if (widget)
     string = (gchar *)elm_object_text_get(widget);

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
eail_text_get_character_at_offset(AtkText    *text,
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
 * @brief Gets the text's length
 *
 * @param text AtkText instance
 * @return integer representing the text length
 */
static gint
eail_text_get_character_count(AtkText *text)
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
eail_text_get_default_attributes(AtkText *text)
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
eail_text_get_run_attributes(AtkText *text,
                             gint offset,
                             gint *start_offset,
                             gint *end_offset)
{
   AtkAttributeSet *at_set = NULL;
   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(text));
   gint len = eail_text_get_character_count(ATK_TEXT(text));

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
   Elm_Wrap_Type wrap_type = ELM_WRAP_NONE;
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
 * @brief AtkText interface initializer
 *
 * @param iface AtkTextIface instance
 */
static void
atk_text_interface_init(AtkTextIface *iface)
{
   iface->get_text = eail_text_get_text;
   iface->get_character_at_offset = eail_text_get_character_at_offset;
   iface->get_character_count = eail_text_get_character_count;
   iface->get_default_attributes = eail_text_get_default_attributes;
   iface->get_run_attributes = eail_text_get_run_attributes;
}
