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
 * @brief Gets state set of the accessible.
 *
 * The caller must unreference it when it is no longer needed.
 *
 * @param accessible AtkObject instance
 * @return accessible state set
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
 * @brief Gets obj name
 *
 * Returns obj accessible name if assigned, obj text content otherwise
 *
 * @param obj an AtkObject
 * @returns accessible obj name if assigned, obj text content otherwise
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
 * @param klass EailLabel class
 */
static void
eail_label_class_init(EailLabelClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);

   class->initialize = eail_label_initialize;
   class->ref_state_set = eail_label_ref_state_set;
   class->get_name = eail_label_get_name;
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
 * @brief Gets character at offset
 *
 * @param text an AtkText
 * @param offset character offset
 * @return character at offset
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
 * @brief Gets text length
 *
 * @param text an AtkText
 * @return text length
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
_eail_label_add_attribute(AtkAttributeSet *attrib_set,
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
eail_label_get_default_attributes(AtkText *text)
{
   AtkAttributeSet *at_set = NULL;

   at_set = _eail_label_add_attribute
       (at_set, ATK_TEXT_ATTR_WRAP_MODE,
        g_strdup
        (atk_text_attribute_get_value(ATK_TEXT_ATTR_WRAP_MODE, 0)));

   at_set = _eail_label_add_attribute
       (at_set, ATK_TEXT_ATTR_EDITABLE,
        g_strdup
        (atk_text_attribute_get_value
         (ATK_TEXT_ATTR_EDITABLE, FALSE)));

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
   at_set = _eail_label_add_attribute
       (at_set, ATK_TEXT_ATTR_WRAP_MODE,
        g_strdup
        (atk_text_attribute_get_value
         (ATK_TEXT_ATTR_WRAP_MODE, wrap_type)));

   at_set = _eail_label_add_attribute
       (at_set, ATK_TEXT_ATTR_EDITABLE,
        g_strdup
        (atk_text_attribute_get_value
         (ATK_TEXT_ATTR_EDITABLE, FALSE)));

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
   iface->get_character_count = eail_label_get_character_count;
   iface->get_character_at_offset = eail_label_get_character_at_offset;
   iface->get_text = eail_label_get_text;
   iface->get_run_attributes = eail_label_get_run_attributes;
   iface->get_default_attributes = eail_label_get_default_attributes;
}

