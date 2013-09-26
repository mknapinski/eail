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
 * @file eail_naviframe.c
 * @brief EailNaviframe implementation
 */

#include "eail_naviframe.h"
#include "eail_naviframe_page.h"
#include "eail_factory.h"
#include "eail_utils.h"
#include "eail_priv.h"

#define EAIL_NAVIFRAME_CLICK_ACTION "click" /**< @brief 'click' action name*/

/**
 * @brief Define EailNaviframe GObject type
 *
 * It extends EAIL_TYPE_WIDGET and implements ATK_TYPE_SELECTION and
 * ATK_TYPE_ACTION interfaces
 */
G_DEFINE_TYPE(EailNaviframe, eail_naviframe, EAIL_TYPE_ACTION_WIDGET);

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
eail_naviframe_n_children_get(AtkObject *obj)
{
   EailNaviframe *naviframe;
   Evas_Object *widget;
   Eina_List *list;
   int list_count;

   g_return_val_if_fail(EAIL_IS_NAVIFRAME(obj), -1);

   naviframe = EAIL_NAVIFRAME(obj);
   widget = eail_widget_get_widget(EAIL_WIDGET(naviframe));
   list = elm_naviframe_items_get(widget);
   list_count = eina_list_count(list);
   eina_list_free(list);

   return list_count;
}

/**
 * @brief 'click' action callback
 *
 * @param action AtkAction instance
 * @param data user data
 * @return TRUE on success, FALSE otherwise
 */
static gboolean
eail_naviframe_action_click(AtkAction *action, void *data)
{
   EailNaviframe *naviframe;
   Evas_Object *widget, *target;
   Elm_Object_Item *it;

   g_return_val_if_fail(EAIL_IS_NAVIFRAME(action), FALSE);

   naviframe = EAIL_NAVIFRAME(action);

   widget = eail_widget_get_widget(EAIL_WIDGET(naviframe));
   it = elm_naviframe_top_item_get(widget);
   if (!elm_naviframe_item_title_visible_get(it)) return FALSE;

   target = elm_object_item_widget_get(it);
   if (!target) return FALSE;

   evas_object_smart_callback_call(target, "title,clicked", it);

   return TRUE;
}

/**
 * @brief Initializes Action interface for Naviframe
 *
 * @param action_widget EailActionWidget instance
 */
static void eail_naviframe_actions_init(EailActionWidget *action_widget)
{
   eail_action_widget_action_append(action_widget,
                                    EAIL_NAVIFRAME_CLICK_ACTION, NULL,
                                    eail_naviframe_action_click);
}

/**
 * @brief Initializer for GObject EailNaviframe instance
 *
 * @param naviframe EailNaviframe instance
 */
static void
eail_naviframe_init(EailNaviframe *naviframe)
{
}

/**
 * @brief Returns naviframe-page implementation for given object item
 *
 * @param naviframe AtkObject instance
 * @param item Elm_Object_Item for naviframe-page
 *
 * @returns AtkObject representation of naviframe-page
 */
static AtkObject *
_eail_naviframe_get_naviframe_page_for_item(AtkObject *naviframe,
                                            Elm_Object_Item *item)
{
   EailFactoryObj *factory_obj = NULL;
   AtkObject *naviframe_page = NULL;

   factory_obj = eail_factory_find_obj_for_item(item);
   if (factory_obj)
     {
        if (!factory_obj->atk_obj)
          ERR("No atk obj created for factory item object");

        return factory_obj->atk_obj;
     }

   naviframe_page = eail_naviframe_page_new(naviframe, item);
   atk_object_initialize(naviframe_page, item);
   eail_factory_append_item_to_cache(ATK_OBJECT(naviframe_page), item);

   return naviframe_page;
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
 * @param i child index
 *
 * @returns AtkObject representing the specified accessible child of the
 * accessible
 */
static AtkObject *
eail_naviframe_ref_child(AtkObject *obj, gint i)
{
   Eina_List *list;
   EailNaviframe *naviframe;
   EailWidget *widget;
   Evas_Object *e_object;

   g_return_val_if_fail(EAIL_IS_NAVIFRAME(obj), NULL);

   naviframe = EAIL_NAVIFRAME(obj);
   widget = EAIL_WIDGET(naviframe);

   e_object = eail_widget_get_widget(widget);

   list = elm_naviframe_items_get(e_object);
   int list_count = eina_list_count(list);
   if (i >= list_count)
     {
        eina_list_free(list);
        return NULL;
     }

   AtkObject *child = _eail_naviframe_get_naviframe_page_for_item
                                                (obj, eina_list_nth(list, i));
   g_object_ref(child);
   eina_list_free(list);

   return child;
}

/**
 * @brief Destructor of naviframe object
 * @param obj GObject instance
 */
static void
eail_naviframe_finalize(GObject *obj)
{
   EailNaviframe *naviframe = EAIL_NAVIFRAME(obj);

   if (naviframe->click_description)
     free(naviframe->click_description);

   G_OBJECT_CLASS(eail_naviframe_parent_class)->finalize(obj);
}

/**
 * @brief Emits state-changed signals for naviframe child page if needed
 *
 * @param naviframe_page an AtkObject for naviframe page
 */
static void
_eail_naviframe_page_emit_changed_signals(AtkObject *naviframe_page)
{
   AtkStateSet *state_set = NULL;
   gboolean visible = FALSE;

   state_set = atk_object_ref_state_set(naviframe_page);
   visible = atk_state_set_contains_state(state_set, ATK_STATE_VISIBLE);

   atk_object_notify_state_change(naviframe_page, ATK_STATE_VISIBLE, visible);
   atk_object_notify_state_change(naviframe_page, ATK_STATE_SHOWING, visible);
   DBG("Emiting state_changed visible/showing: %d", visible);
}

/**
 * @brief Handles signals changes for naviframe and its children pages
 *
 * @param naviframe an AtkObject for naviframe
 */
static void
_eail_naviframe_emit_signals_changed(AtkObject *naviframe)
{
   gint n_children = 0, i = 0;
   AtkObject *navipage = NULL;

   n_children = eail_naviframe_n_children_get(naviframe);

   for (i = 0; i < n_children; ++i)
     {
        navipage = eail_naviframe_ref_child(naviframe, i);
        if (navipage)
          {
              DBG("Emitting state-changed for child %d", i);

              _eail_naviframe_page_emit_changed_signals(navipage);
              g_object_unref(navipage);
          }
     }
}

/**
 * @brief Handler for event which is raised when naviframe page is changed
 * ("transition,finished" event)
 *
 * @param data data passed to callback
 * @param obj Evas_Object that raised event
 * @param event_info additional event info
 */
void
_eail_naviframe_handle_page_changed_event(void *data,
                                          Evas_Object *obj,
                                          void *event_info)
{
   EailNaviframe *naviframe_obj = NULL;
   g_return_if_fail(EAIL_IS_NAVIFRAME(data));
   gint n_children = 0;

   eail_emit_atk_signal
                  (ATK_OBJECT(data), "visible-data-changed", ATK_TYPE_OBJECT);

   naviframe_obj = EAIL_NAVIFRAME(data);
   if (!naviframe_obj) return;

   _eail_naviframe_emit_signals_changed(ATK_OBJECT(naviframe_obj));

   n_children = eail_naviframe_n_children_get(ATK_OBJECT(naviframe_obj));
   DBG("N_CHILDREN: %d", n_children);

   if (n_children && n_children > naviframe_obj->child_count_last)
     {
         eail_emit_children_changed(TRUE, ATK_OBJECT(data), n_children - 1);
     }
   else if(n_children < naviframe_obj->child_count_last)
    {
         eail_emit_children_changed(FALSE, ATK_OBJECT(data), n_children);
    }

   naviframe_obj->child_count_last = n_children;
}

/**
 * @brief Initializer for AtkObjectClass
 *
 * @param obj AtkObject instance
 * @param data initialization data
 */
static void
eail_naviframe_initialize(AtkObject *obj, gpointer data)
{
   Evas_Object *nested_widget = NULL;
   EailNaviframe *naviframe_obj = NULL;
   ATK_OBJECT_CLASS(eail_naviframe_parent_class)->initialize(obj, data);

   obj->role = ATK_ROLE_PAGE_TAB_LIST;
   g_return_if_fail(EAIL_IS_WIDGET(obj));

   nested_widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!nested_widget)
     {
        ERR("No evas object inside EailWidget was found");
        return;
     }

   evas_object_smart_callback_add(nested_widget, "transition,finished",
                                _eail_naviframe_handle_page_changed_event, obj);

   eail_naviframe_actions_init(EAIL_ACTION_WIDGET(obj));

   naviframe_obj = EAIL_NAVIFRAME(obj);
   /* storing last numbers of children to be for checking if children-changed
    * signal has to be propagated */
   naviframe_obj->child_count_last = eail_naviframe_n_children_get(obj);
}

/**
 * @brief Initializer for GObject naviframe class
 *
 * Defines callbacks for base AtkObject.
 *
 * @param klass EailNaviframeClass instance
 */
static void
eail_naviframe_class_init(EailNaviframeClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
   GObjectClass *gobject_class = G_OBJECT_CLASS(klass);

   class->initialize = eail_naviframe_initialize;
   class->get_n_children = eail_naviframe_n_children_get;
   class->ref_child = eail_naviframe_ref_child;

   gobject_class->finalize = eail_naviframe_finalize;
}

