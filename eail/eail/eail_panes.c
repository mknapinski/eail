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
 * @file eail_panes.c
 * @brief EailPanes implementation
 */

#include <Elementary.h>

#include "eail_panes.h"
#include "eail_factory.h"
#include "eail_priv.h"

static void atk_value_interface_init(AtkValueIface *iface);

/**
 * @brief EailPanes type definition
 */
G_DEFINE_TYPE_WITH_CODE(EailPanes,
                        eail_panes,
                        EAIL_TYPE_WIDGET,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_VALUE,
                                              atk_value_interface_init));

/**
 * @brief EailPanes initializer
 *
 * @param obj an AtkObject
 * @param data initialization data
 */
static void
eail_panes_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_panes_parent_class)->initialize(obj, data);
   obj->role = ATK_ROLE_SPLIT_PANE;
}

/**
 * @brief EailPanes instance initializer
 *
 * @param panes an EailPanes
 */
static void
eail_panes_init(EailPanes *panes)
{
}

/**
 * @brief Gets obj children number
 *
 * @param obj an AtkObject
 * @returns children number
 */
static gint
eail_panes_n_children_get(AtkObject *obj)
{
   g_return_val_if_fail(EAIL_IS_PANES(obj), 0);

   return 2;
}

/**
 * @brief Gets obj state set
 *
 * @param obj an AtkObject
 * @return obj state set
 */
static AtkStateSet*
eail_panes_ref_state_set(AtkObject *obj)
{
   AtkStateSet *state_set;
   Evas_Object *widget;

   state_set = ATK_OBJECT_CLASS(eail_panes_parent_class)->ref_state_set(obj);
   widget = eail_widget_get_widget(EAIL_WIDGET(obj));

   if (widget == NULL) return state_set;

   if (elm_panes_horizontal_get(widget))
     atk_state_set_add_state(state_set, ATK_STATE_HORIZONTAL);
   else
     atk_state_set_add_state(state_set, ATK_STATE_VERTICAL);

   return state_set;
}

/**
 * @brief Gets obj i-th child reference
 *
 * The caller must unreference it when it is no longer needed
 *
 * @param obj an AtkObject
 * @param i child index
 * @return child reference
 */
static AtkObject*
eail_panes_ref_child(AtkObject *obj, int i)
{
   Evas_Object *e_object, *widget;
   AtkObject *child;

   g_return_val_if_fail(EAIL_IS_PANES(obj), NULL);
   g_return_val_if_fail((i < 2), NULL);

   e_object = eail_widget_get_widget(EAIL_WIDGET(obj));
   switch (i)
     {
      case 0:
         widget = elm_object_part_content_get(e_object, "left");
         child = eail_factory_get_accessible(widget);
         break;
      case 1:
         widget = elm_object_part_content_get(e_object, "right");
         child = eail_factory_get_accessible(widget);
         break;
      default:
         child = NULL;
         break;
     }

   if (child)
     g_object_ref(child);

   return child;
}

/**
 * @brief an EailPanes class initializer
 *
 * @param klass an EailPanes class
 */
static void
eail_panes_class_init(EailPanesClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);

   class->initialize = eail_panes_initialize;
   class->get_n_children = eail_panes_n_children_get;
   class->ref_child = eail_panes_ref_child;
   class->ref_state_set = eail_panes_ref_state_set;
}

/**
 * @brief Gets obj maximum value
 *
 * @param obj an AtkValue
 * @param value obj maxiumum value
 */
static void
eail_panes_maximum_value_get(AtkValue *obj, GValue *value)
{
   g_return_if_fail(EAIL_IS_PANES(obj));

   memset(value, 0, sizeof(GValue));
   g_value_init(value, G_TYPE_DOUBLE);
   g_value_set_double(value, 1.0);
}

/**
 * @brief Gets obj minimum value
 *
 * @param obj an AtkValue
 * @param value obj minimum value
 */
static void
eail_panes_minimum_value_get(AtkValue *obj, GValue *value)
{
   g_return_if_fail(EAIL_IS_PANES(obj));

   memset(value, 0, sizeof(GValue));
   g_value_init(value, G_TYPE_DOUBLE);
   g_value_set_double(value, 0.0);
}

/**
 * @brief Sets obj current value
 *
 * @param obj an AtkValue
 * @param value obj new value
 * @return TRUE if value is set successfully, FALSE otherwise
 */
static gboolean
eail_panes_current_value_set(AtkValue *obj, const GValue *value)
{
   Evas_Object *widget;
   gdouble new_value;

   g_return_val_if_fail(EAIL_IS_PANES(obj), FALSE);

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return FALSE;

   if (G_VALUE_HOLDS_DOUBLE(value))
        new_value = g_value_get_double(value);
   else
     return FALSE;

   if ((new_value >= 0.0) && (new_value <= 1.0))
     {
        elm_panes_content_left_size_set(widget, new_value);
        return TRUE;
     }
   else
     return FALSE;
}

/**
 * @brief Gets obj current value
 *
 * @param obj an AtkValue
 * @param value obj current value
 */
static void
eail_panes_current_value_get(AtkValue *obj, GValue *value)
{
   Evas_Object *widget;
   gdouble current_value;

   g_return_if_fail(EAIL_IS_PANES(obj));
   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return;

   current_value = elm_panes_content_left_size_get(widget);
   memset(value, 0, sizeof(GValue));
   g_value_init(value, G_TYPE_DOUBLE);
   g_value_set_double(value, current_value);
}

/**
 * @brief AtkValue interface initializer
 *
 * @param iface an AtkValueIface
 */
static void
atk_value_interface_init(AtkValueIface *iface)
{
   g_return_if_fail(iface != NULL);

   iface->get_current_value = eail_panes_current_value_get;
   iface->set_current_value = eail_panes_current_value_set;
   iface->get_maximum_value = eail_panes_maximum_value_get;
   iface->get_minimum_value = eail_panes_minimum_value_get;
}
