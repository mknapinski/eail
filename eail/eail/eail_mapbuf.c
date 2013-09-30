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
 * @file eail_mapbuf.c
 * @brief EailMapbuf implementation
 */

#include <Elementary.h>

#include "eail_mapbuf.h"

/**
 * @brief EailMapbuf type definition
 */
G_DEFINE_TYPE(EailMapbuf, eail_mapbuf, EAIL_TYPE_WIDGET);

/**
 * @brief EailMapbuf initializer
 *
 * @param obj AtkObject instance
 * @param data initialization data
 */
static void
eail_mapbuf_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_mapbuf_parent_class)->initialize(obj, data);
   obj->role = ATK_ROLE_IMAGE_MAP;
}
/**
 * @brief Gets widget's children
 *
 * @param widget EailWidget instance
 * @return Eina_List containing widget's children
 */
static Eina_List*
eail_mapbuf_children_get(EailWidget *widget)
{
   Eina_List *list = NULL;
   Evas_Object *o, *object;

   g_return_val_if_fail(EAIL_IS_WIDGET(widget), NULL);

   object = eail_widget_get_widget(widget);

   o = elm_object_part_content_get(object, "default");
   if (!o) return NULL;

   list = eina_list_append(list, o);
   return list;
}

/**
 * @brief Gets object's state set
 *
 * The caller must unreference it when it is no longed needed.
 *
 * @param object AtkObject instance
 * @return AtkStateSet* containing object's state set
 */
static AtkStateSet*
eail_mapbuf_ref_state_set(AtkObject *object)
{
   AtkStateSet *state_set;
   Evas_Object *widget;

   g_return_val_if_fail(EAIL_IS_MAPBUF(object), NULL);
   widget = eail_widget_get_widget(EAIL_WIDGET(object));
   if (!widget) return NULL;

   state_set =
      ATK_OBJECT_CLASS(eail_mapbuf_parent_class)->ref_state_set(object);
   if (elm_mapbuf_enabled_get(widget))
     atk_state_set_add_state(state_set, ATK_STATE_ENABLED);

   return state_set;
}

/**
 * @brief EailMapbuf instance initializer
 *
 * @param mapbuf EailMapbuf instance
 */
static void
eail_mapbuf_init(EailMapbuf *mapbuf)
{
}

/**
 * @brief EailMapbuf class initializer
 *
 * @param klass EailMapbufClass instance
 */
static void
eail_mapbuf_class_init(EailMapbufClass *klass)
{
   AtkObjectClass *atk_class = ATK_OBJECT_CLASS(klass);
   EailWidgetClass *widget_class = EAIL_WIDGET_CLASS(klass);

   atk_class->initialize = eail_mapbuf_initialize;
   atk_class->ref_state_set = eail_mapbuf_ref_state_set;

   widget_class->get_widget_children = eail_mapbuf_children_get;
}
