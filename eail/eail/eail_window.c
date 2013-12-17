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
 * @file eail_window.c
 * @brief EailWindow Implementation
 */

#include <Elementary.h>
#include "eail_window.h"
#include "eail_factory.h"
#include "eail_utils.h"
#include "eail_dynamic_content.h"
#include "eail_priv.h"

static void eail_window_actions_init(EailActionWidget *widget);
static void atk_window_interface_init (AtkWindowIface *iface);
static void atk_component_interface_init(AtkComponentIface *iface);
static void eail_dynamic_content_interface_init(EailDynamicContentIface *iface);

/**
 * @brief Focus signal name
 */
#define EAIL_WINDOW_FOCUS_NAME "focus,in"

#define EAIL_WIN_ACTION_MAXIMIZE "maximize" /**< 'maximize' action name */
#define EAIL_WIN_ACTION_MINIMIZE "minimize" /**< 'minimize' action name */
#define EAIL_WIN_ACTION_RESTORE "restore" /**< 'restore' action name */

/**
 * @brief EailWindow type definition
 */
G_DEFINE_TYPE_WITH_CODE(EailWindow,
                        eail_window,
                        EAIL_TYPE_ACTION_WIDGET,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_WINDOW,
                                              atk_window_interface_init)
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_COMPONENT,
                                            atk_component_interface_init)
                        G_IMPLEMENT_INTERFACE(EAIL_TYPE_DYNAMIC_CONTENT,
                                          eail_dynamic_content_interface_init));

/**
 * @brief Maximizes event handler
 *
 * @param data data passed to callback
 * @param obj Evas_Object instance that raised event
 * @param event_info additional event info
 */
void
_eail_window_handle_maximize_event(void *data,
                                   Evas_Object *obj,
                                   void *event_info)
{
   eail_emit_atk_signal(ATK_OBJECT(data), "maximize", EAIL_TYPE_WINDOW);
}

/**
 * @brief Minimizes event handler
 *
 * @param data data passed to callback
 * @param obj Evas_Object instance that raised event
 * @param event_info additional event info
 */
void
_eail_window_handle_minimize_event(void *data,
                                   Evas_Object *obj,
                                   void *event_info)
{
   eail_emit_atk_signal(ATK_OBJECT(data), "minimize", EAIL_TYPE_WINDOW);
}

/**
 * @brief Moves event handler
 *
 * @param data data passed to callback
 * @param obj Evas_Object instance that raised event
 * @param event_info additional event info
 */
void
eail_window_on_move(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
   g_return_if_fail(ATK_IS_OBJECT(data));

   eail_emit_atk_signal(ATK_OBJECT(data), "move", EAIL_TYPE_WINDOW);
}

/**
 * @brief Deactivate event handler
 *
 * @param data data passed to callback
 * @param obj Evas_Object instance that raised event
 * @param event_info additional event info
 */
void
eail_window_on_deactivate(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
   g_return_if_fail(ATK_IS_OBJECT(data));

   eail_emit_atk_signal(ATK_OBJECT(data), "deactivate", EAIL_TYPE_WINDOW);
}

/**
 * @brief Activate event handler
 *
 * @param data data passed to callback
 * @param obj Evas_Object instance that raised event
 * @param event_info additional event info
 */
void
eail_window_on_activate(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
   g_return_if_fail(ATK_IS_OBJECT(data));

   eail_emit_atk_signal(ATK_OBJECT(data), "activate", EAIL_TYPE_WINDOW);
}



/**
 * @brief Callback used for tracking resize-changes for window
 *
 * @param data data passed to callback
 * @param e Evas instance that has been shown
 * @param obj Evas_Object instance that has been shown
 * @param event_info additional event info
 */
void
eail_window_on_resize(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
   g_return_if_fail(ATK_IS_OBJECT(data));

   eail_emit_atk_signal(ATK_OBJECT(data), "resize", EAIL_TYPE_WINDOW);
}

/**
 * @brief Destroyed event handler for window
 *
 * @param data passed to callback
 * @param obj object that raised event
 * @param event_info additional event info
 */
void
_eail_window_handle_delete_event(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
   atk_object_notify_state_change(ATK_OBJECT(data), ATK_STATE_DEFUNCT, TRUE);
   eail_emit_atk_signal(ATK_OBJECT(data), "destroy", EAIL_TYPE_WINDOW);
   eail_factory_unregister_wdgt_from_cache(obj);
}

/**
 * @brief Restore event handler
 *
 * @param data passed to callback
 * @param obj object that raised event
 * @param event_info additional event info
 */
void
_eail_window_handle_restore_event(void *data,
                                   Evas_Object *obj,
                                   void *event_info)
{
   eail_emit_atk_signal(ATK_OBJECT(data), "restore", EAIL_TYPE_WINDOW);
}

/**
 * @brief Initializes window focus handler
 *
 * @param obj AtkObject instance
 */
static void
eail_window_init_focus_handler(AtkObject *obj)
{
   Evas_Object *nested_widget = NULL;
   g_return_if_fail(EAIL_IS_WIDGET(obj));

   nested_widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!nested_widget)
     {
        ERR("No evas object inside EailWidget was found");
        return;
     }

   evas_object_smart_callback_add(nested_widget, "maximized",
                                  _eail_window_handle_maximize_event, obj);
   evas_object_smart_callback_add(nested_widget, "iconified",
                                  _eail_window_handle_minimize_event, obj);
   evas_object_smart_callback_add(nested_widget, "unmaximized",
                                  _eail_window_handle_restore_event, obj);
   evas_object_smart_callback_add(nested_widget, "normal",
                                  _eail_window_handle_restore_event, obj);

   /* evas object events (not smart callbacks) */
   evas_object_event_callback_add(nested_widget, EVAS_CALLBACK_RESIZE,
                                  eail_window_on_resize, obj);
   evas_object_event_callback_add(nested_widget, EVAS_CALLBACK_MOVE,
                                  eail_window_on_move, obj);
   evas_object_event_callback_add(nested_widget, EVAS_CALLBACK_FOCUS_OUT,
                                  eail_window_on_deactivate, obj);
   evas_object_event_callback_add(nested_widget, EVAS_CALLBACK_FOCUS_IN,
                                  eail_window_on_activate, obj);
   evas_object_event_callback_add(nested_widget, EVAS_CALLBACK_DEL,
                                  _eail_window_handle_delete_event, obj);
}

/**
 * @brief EailWindow initializer
 *
 * @param obj AtkObject instance
 * @param data initialization data
 */
static void
eail_window_initialize(AtkObject *obj, gpointer data)
{
   EailWindow *eail_win = NULL;
   ATK_OBJECT_CLASS(eail_window_parent_class)->initialize(obj, data);
   EAIL_WIDGET(obj)->layer = ATK_LAYER_WINDOW;

   if (!elm_object_widget_check((Evas_Object *) data)) return;

   obj->name = g_strdup(elm_win_title_get((Evas_Object *)data));
   obj->role = ATK_ROLE_WINDOW;
   obj->accessible_parent = atk_get_root();

   eail_window_init_focus_handler(obj);
   eail_window_actions_init(EAIL_ACTION_WIDGET(obj));

   eail_win = EAIL_WINDOW(obj);
   /* storing last numbers of children to be for checking if children-changed
    * signal has to be propagated */
   eail_win->child_count_last = atk_object_get_n_accessible_children(obj);
}

/**
 * @brief Gets the children list from given edje
 *
 * @param edje lowest (stacked) Evas object
 * @return Eina_List representing the children list
 */
static Eina_List *
_parse_edje(const Evas_Object *edje)
{
   const Evas_Object *content_part = NULL;
   const Evas_Object *menu_part = NULL;
   Eina_List *list = NULL;

   if (edje_object_part_exists(edje, "elm.swallow.menu"))
     menu_part = edje_object_part_swallow_get(edje, "elm.swallow.menu");
   if (edje_object_part_exists(edje, "elm.swallow.contents"))
     content_part = edje_object_part_swallow_get(edje, "elm.swallow.contents");
   if ((menu_part) &&
       (!strcmp(evas_object_type_get(menu_part), "Evas_Object_Box")))
        list = evas_object_box_children_get(menu_part);
   if ((content_part) &&
       (!strcmp(evas_object_type_get(content_part), "Evas_Object_Box")))
     {
        if (list)
          list = eina_list_merge(list,
                                 evas_object_box_children_get(content_part));
        else
          list = evas_object_box_children_get(content_part);
     }

   return list;
}

/**
 * @brief Gets widget's children
 *
 * @param widget EailWidget instance
 * @return Eina_List representing the list of widget's children
 */
static Eina_List *
eail_window_get_widget_children(EailWidget *widget)
{
   Evas_Object *o, *obj = eail_widget_get_widget(widget);
   Eina_List *win_childs = NULL;
   Eina_List *widgets = NULL;
   Evas *e;
   Eina_List *l;
   Evas_Object *item;

   /*in elementary >= 1.7.99 we get edje object if object are stacked
     in containers like box, grid etc we need to get children from this
     edje*/

   e = evas_object_evas_get(obj);
   o = evas_object_bottom_get(e);
   if (!g_strcmp0(evas_object_type_get(o), "edje"))
     {
        widgets = _parse_edje(o);
        EINA_LIST_FOREACH(widgets, l, item)
          {
             if(!elm_object_widget_check(item))
               widgets = eina_list_remove(widgets, item);
          }
     }
   /* Sometimes we have a mix of widgets grouped in containters with
    * those directly on elm_win objct. So list evas objects laying on
    * window to be sure we get everything */
   o = obj;
   while ((o = evas_object_below_get(o)))
     {
        if (elm_object_widget_check(o))
          {
             /*be sure that object belongs to window and not to other
              * container*/
             if (obj == elm_object_parent_widget_get(o))
               win_childs = eina_list_append(win_childs, o);
          }
     }

   if (win_childs)
     {
        /*reverse list to get correct order of widgets tree*/
        win_childs = eina_list_reverse(win_childs);
        /*merge window childs together with containers*/
        if (widgets)
          widgets = eina_list_merge(widgets, win_childs);
        else
          widgets = win_childs;
     }

   return widgets;
}

/**
 * @brief Gets the state set of the accessible
 *
 * @param obj AtkObject instance
 * @return AtkStateSet representing the state set of the accessible
 */
static AtkStateSet *
eail_window_ref_state_set(AtkObject *obj)
{
   double x, y;
   AtkStateSet *state_set;
   Eina_List *l, *children;
   Eina_Bool resizable = EINA_TRUE;
   Evas_Object *child, *widget = eail_widget_get_widget(EAIL_WIDGET(obj));

   if (!widget) return NULL;

   state_set= ATK_OBJECT_CLASS(eail_window_parent_class)->ref_state_set(obj);

   if (elm_win_focus_get(widget))
     {
        atk_state_set_add_state(state_set, ATK_STATE_ACTIVE);
     }

   if (elm_win_iconified_get(widget))
     {
        atk_state_set_add_state(state_set, ATK_STATE_ICONIFIED);
     }

   if (elm_win_modal_get(widget))
     {
        atk_state_set_add_state(state_set, ATK_STATE_MODAL);
     }

   children = eail_widget_get_widget_children(EAIL_WIDGET(obj));
   EINA_LIST_FOREACH(children, l, child)
     {
        evas_object_size_hint_weight_get(child, &x, &y);
        if (!float_equal(x, EVAS_HINT_EXPAND) || !float_equal(y, EVAS_HINT_EXPAND))
          {
             resizable = EINA_FALSE;
             break;
          }
     }
   eina_list_free(children);

   if (resizable)
     {
        atk_state_set_add_state(state_set, ATK_STATE_RESIZABLE);
     }

   return state_set;
}

/**
 * @brief Get the index in parent of the accessible
 *
 * @param obj AtkObject instance
 * @return Integer representing the index of the object in parent
 */
static int
eail_window_get_index_in_parent(AtkObject *obj)
{
    AtkObject *parent = atk_object_get_parent(obj);

    if(atk_object_get_n_accessible_children(parent) == 1)
      {
        return 0;
      }
    else
      {
        int i;
        for(i = 0; i < atk_object_get_n_accessible_children(parent); i++)
          {
            AtkObject *child = atk_object_ref_accessible_child(parent, i);
            if(child == obj)
              {
                g_object_unref(child);
                return i;
              }
            g_object_unref(child);
          }
      }

    return -1;
}

/**
 * @brief EailWindow instance initializer
 *
 * @param window EailWindow instance
 */
static void
eail_window_init(EailWindow *window)
{
}

/**
 * @brief EailWindow class initializer
 *
 * @param klass EailWindowClass instance
 */
static void
eail_window_class_init(EailWindowClass *klass)
{
   AtkObjectClass *atk_class = ATK_OBJECT_CLASS(klass);
   EailWidgetClass *widget_class = EAIL_WIDGET_CLASS(klass);

   widget_class->get_widget_children = eail_window_get_widget_children;

   atk_class->initialize = eail_window_initialize;
   atk_class->ref_state_set = eail_window_ref_state_set;
   atk_class->get_index_in_parent = eail_window_get_index_in_parent;
}

/**
 * @brief Handle for minimize action
 *
 * @param action AtkAction instance
 * @param data additional action data (not used here)
 *
 * @return TRUE if action was triggered successfully, FALSE otherwise
 */
static gboolean
eail_action_minimize(AtkAction *action, void *data)
{
   Evas_Object *widget;
   AtkObject *obj;

   g_return_val_if_fail(EAIL_IS_WINDOW(action), FALSE);

   widget = eail_widget_get_widget(EAIL_WIDGET(action));

   elm_win_iconified_set(widget, EINA_TRUE);

   obj = ATK_OBJECT(action);

   eail_emit_atk_signal(obj, "minimize", EAIL_TYPE_WINDOW);

   return TRUE;
}

/**
 * @brief Handle for maximize action
 *
 * @param action AtkAction instance
 * @param data additional action data (not used here)
 *
 * @return TRUE if action was triggered successfully, FALSE otherwise
 */
static gboolean
eail_action_maximize(AtkAction *action, void *data)
{
   Evas_Object *widget;
   AtkObject *obj;

   g_return_val_if_fail(EAIL_IS_WINDOW(action), FALSE);

   widget = eail_widget_get_widget(EAIL_WIDGET(action));

   elm_win_maximized_set(widget, EINA_TRUE);

   obj = ATK_OBJECT(action);

   eail_emit_atk_signal(obj, "maximize", EAIL_TYPE_WINDOW);
   return TRUE;
}

/**
 * @brief Handle for action restore
 *
 * @param action an AtkAction
 * @param data additional action data (not used here)
 *
 * @return TRUE if action was triggered successfully, FALSE otherwise
 */
static gboolean
eail_action_restore(AtkAction *action, void *data)
{
   Evas_Object *widget;
   AtkObject *obj;

   g_return_val_if_fail(EAIL_IS_WINDOW(action), FALSE);

   widget = eail_widget_get_widget(EAIL_WIDGET(action));

   if(elm_win_maximized_get(widget))
     {
        elm_win_maximized_set(widget, EINA_FALSE);
     }

   if(elm_win_iconified_get(widget))
     {
        elm_win_activate(widget);
     }

   obj = ATK_OBJECT(action);

   eail_emit_atk_signal(obj, "restore", EAIL_TYPE_WINDOW);
   return TRUE;
}

/**
 * @brief Adds window actions to the actions table
 *
 * @param action_widget widget that implements EailActionWidget interface
 */
static void
eail_window_actions_init(EailActionWidget *action_widget)
{
   eail_action_widget_action_append(action_widget,
                                    EAIL_WIN_ACTION_MAXIMIZE, NULL,
                                    eail_action_maximize);
   eail_action_widget_action_append(action_widget,
                                    EAIL_WIN_ACTION_MINIMIZE, NULL,
                                    eail_action_minimize);
   eail_action_widget_action_append(action_widget,
                                    EAIL_WIN_ACTION_RESTORE, NULL,
                                    eail_action_restore);
}

/**
 * @brief AtkWindow interface initializer
 *
 * It is empty because at the moment
 * AtkWindow is just about signals
 *
 * @param iface AtkWindowIface instance
 */
static void
atk_window_interface_init(AtkWindowIface *iface)
{
}

/**
 * @brief Returns the first found widget from EvasObjectBox
 *
 * @param evas_obj EvasObjectBox instance to search in
 *
 * @returns Evas_Object that represents the found widget
 * or NULL if none has been found
 */
static Evas_Object*
_eail_get_first_widget_from_evas_box(Evas_Object *evas_obj)
{
   Eina_List *children = NULL, *l;
   Evas_Object *it_evas_obj = NULL;

   children = evas_object_box_children_get(evas_obj);

   EINA_LIST_FOREACH(children, l, it_evas_obj)
     {
      if (elm_object_widget_check (it_evas_obj))
         {
            DBG("Widget has been found %s", evas_object_type_get(it_evas_obj));
            return it_evas_obj;
         }
     }

   return NULL;
}

/**
 * @brief Returns the first found widget from given edje object
 *
 * @param evas_obj edje object to search in
 *
 * @returns Evas_Object that represents the found widget
 * or NULL if none has been found
 */
static Evas_Object *
_eail_get_first_widget_from_edje(Evas_Object *evas_obj)
{
   Eina_List *members = NULL, *l;
   Evas_Object *it_evas_obj = NULL;

   members = evas_object_smart_members_get(evas_obj);
   EINA_LIST_FOREACH(members, l, it_evas_obj)
   {
      const char *type_name = evas_object_type_get(it_evas_obj);

      /* if widget found, then returning immediately. If not widget, then
       * looking for nested box*/
      if (elm_object_widget_check (it_evas_obj))
        return it_evas_obj;
      else if (0 == g_strcmp0(type_name, "Evas_Object_Box") )
        {
            Evas_Object *widget_from_box = _eail_get_first_widget_from_evas_box(it_evas_obj);
            if (widget_from_box)
              return widget_from_box;
        }
   }

   return NULL;
}

/**
 * @brief Gets a reference to the accessible child, if one exists, at the
 * coordinate point specified by x and y
 *
 * @param component AtkComponent instance
 * @param x x coordinate
 * @param y y coordinate
 * @param coord_type specifies whether the coordinates are relative to the
 * screen or to the components top level window
 *
 * @returns AtkObject representing the accessible child, if one exists
 */
static AtkObject *
eail_window_ref_accessible_at_point(AtkComponent *component,
                                    gint x,
                                    gint y,
                                    AtkCoordType coord_type)
{
   Evas_Object *widget = NULL, *evas_obj_at_coords = NULL;
   Evas *wdgt_evas = NULL;
   const char * found_obj_type = NULL;

   widget = eail_widget_get_widget(EAIL_WIDGET(component));

   if (!widget) return NULL;

   if (coord_type == ATK_XY_SCREEN) {
           int ee_x, ee_y;
           Ecore_Evas *ee= ecore_evas_ecore_evas_get(evas_object_evas_get(widget));

           ecore_evas_geometry_get(ee, &ee_x, &ee_y, NULL, NULL);
           x -= ee_x;
           y -= ee_y;
       }

   /* NOTE: it is crucial to get object from evas, NOT Evas_Object*/
   wdgt_evas = evas_object_evas_get(widget);

   evas_obj_at_coords = evas_object_top_at_xy_get
                              (wdgt_evas, x, y, EINA_FALSE, EINA_FALSE);
   if (!evas_obj_at_coords)
     {
       DBG("Not found any object at coords %d,%d", x, y);
       return NULL;
     }

   /* if edje object, then we have to parse that and extract widget from it*/
   found_obj_type = evas_object_type_get(evas_obj_at_coords);
   if (0 == g_strcmp0(found_obj_type, "edje"))
     {
         DBG("Edje object found...");
         evas_obj_at_coords = _eail_get_first_widget_from_edje(evas_obj_at_coords);
     }

   return eail_factory_get_accessible(evas_obj_at_coords);
}

/**
 * @brief AtkComponent interface initialization
 *
 * Initialization for ref_accessible_at_point callback used by clients to
 * reference object at given coordinates. Rest of implementation is default
 * so no overriding-callbacks is needed.
 *
 * @param iface EailWindow instance
 */
static void
atk_component_interface_init(AtkComponentIface *iface)
{
   iface->ref_accessible_at_point = eail_window_ref_accessible_at_point;
}

/**
 * @param dynamic_content_holder an EailDynamicContent object (EailWindow)
 */
void
eail_window_update_descendants(EailDynamicContent *dynamic_content_holder)
{
   gint n_children = 0;
   EailWindow *window = NULL;

   if (!EAIL_IS_WINDOW(dynamic_content_holder))
     {
        DBG("No EailWindow found. Returning");
        return;
     }

   window = EAIL_WINDOW(dynamic_content_holder);

   n_children = atk_object_get_n_accessible_children(ATK_OBJECT(window));
   if (n_children && n_children > window->child_count_last)
     {
        eail_emit_children_changed(TRUE, ATK_OBJECT(window), n_children - 1);

     }
   else if (n_children < window->child_count_last)
     {
         eail_emit_children_changed
                     (FALSE, ATK_OBJECT(window), window->child_count_last);
     }

   window->child_count_last = n_children;
}

/**
 * @brief Initializer for dynamic content interface, used for handling objects
 * children hierarchy changes
 *
 * @param iface an EailDynamicContentIface
 */
static void
eail_dynamic_content_interface_init(EailDynamicContentIface *iface)
{
   iface->update_hierarchy = eail_window_update_descendants;
}
