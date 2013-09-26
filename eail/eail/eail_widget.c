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
 * @file eail_widget.c
 * @brief EailWidget implementation
 */

#include <Ecore.h>
#include <Ecore_Evas.h>
#include <Elementary.h>

#include "eail_widget.h"
#include "eail_factory.h"
#include "eail_utils.h"
#include "eail_priv.h"

static void atk_component_interface_init(AtkComponentIface *iface);

/**
 * @brief EailWidget type definition
 */
G_DEFINE_TYPE_WITH_CODE(EailWidget, eail_widget, ATK_TYPE_OBJECT,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_COMPONENT,
                                              atk_component_interface_init));

/**
 * @brief Gets Evas_Object from EailWidget
 *
 * @param widget EailWidget instance
 * @return Evas_Object widget representing the EailWidget
 */
Evas_Object *
eail_widget_get_widget(EailWidget *widget)
{
    g_return_val_if_fail(EAIL_IS_WIDGET(widget), NULL);

    return widget->widget;
}

/**
 * @brief Gets EailWidget's children
 *
 * @param widget EailWidget instance
 * @return Eina_List representing the EailWidget's children list
 */
Eina_List *
eail_widget_get_widget_children(EailWidget *widget)
{
    EailWidgetClass *klass;

    g_return_val_if_fail(EAIL_IS_WIDGET(widget), NULL);

    klass = EAIL_WIDGET_GET_CLASS(widget);
    return klass->get_widget_children(widget);
}

/**
 * @brief Callback used for tracking focus-in changes for widgets
 *
 * @param data data passed to callback
 * @param e Evas instance that has been focused in
 * @param obj Evas_Object instance that has been focused in
 * @param event_info additional event info
 */
void
eail_widget_on_focused_in(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
   g_return_if_fail(ATK_IS_OBJECT(data));

   atk_object_notify_state_change(ATK_OBJECT(data), ATK_STATE_FOCUSED, TRUE);
   atk_focus_tracker_notify(ATK_OBJECT(data));
}

/**
 * @brief Callback used for tracking show-changes for widgets
 *
 * @param data data passed to callback
 * @param e Evas instance that has been shown
 * @param obj Evas_Object instance that has been shown
 * @param event_info additional event info
 */
void
eail_widget_on_show(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
   g_return_if_fail(ATK_IS_OBJECT(data));

   atk_object_notify_state_change(ATK_OBJECT(data), ATK_STATE_SHOWING, TRUE);
}

/**
 * @brief Callback used for tracking hide-changes for widgets
 *
 * @param data data passed to callback
 * @param e Evas instance that has been shown
 * @param obj Evas_Object instance that has been shown
 * @param event_info additional event info
 */
void
eail_widget_on_hide(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
   g_return_if_fail(ATK_IS_OBJECT(data));

   atk_object_notify_state_change(ATK_OBJECT(data), ATK_STATE_SHOWING, FALSE);
}

/**
 * @brief Callback used for tracking focus-out changes for widgets
 *
 * @param data data passed to callback
 * @param e Evas instance that has been focused out
 * @param obj Evas_Object instance that has been focused out
 * @param event_info additional event info
 */
void
eail_widget_on_focused_out(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
   g_return_if_fail(ATK_IS_OBJECT(data));

   DBG("><");
   atk_object_notify_state_change(ATK_OBJECT(data), ATK_STATE_FOCUSED, FALSE);
   atk_focus_tracker_notify(ATK_OBJECT(data));
   eail_notify_child_focus_changes();
}

/**
 * @brief Default callback for on_focused used for tracking focus changes of
 * smart widgets
 *
 * @param data data passed to callback
 * @param obj Evas_Object instance that has been shown
 * @param event_info additional event info
 */
void eail_widget_on_focused_smart(void *data,
                                  Evas_Object *obj,
                                  void *event_info)
{
   g_return_if_fail(ATK_IS_OBJECT(data));

   DBG("><");
   atk_object_notify_state_change(ATK_OBJECT(data), ATK_STATE_FOCUSED, TRUE);
   atk_focus_tracker_notify(ATK_OBJECT(data));
   eail_notify_child_focus_changes();
}

/**
 * @brief on_focused default callback used to tracking focus changes of
 * smart widgets
 *
 * @param data data passed to callback
 * @param obj an Evas_Object that has been shown
 * @param event_info additional event info
 */
void eail_widget_on_focused_out_smart(void *data,
                                      Evas_Object *obj,
                                      void *event_info)
{
   g_return_if_fail(ATK_IS_OBJECT(data));

   DBG("><");
   /* Not propagating further, using only for internal eail focus changes*/
   eail_notify_child_focus_changes();
}

/**
 * @brief EailWidget initializer
 *
 * @param obj AtkObject instance
 * @param data initialization data
 */
static void
eail_widget_initialize(AtkObject *obj, gpointer data)
{
    EailWidget *widget = EAIL_WIDGET(obj);

    ATK_OBJECT_CLASS(eail_widget_parent_class)->initialize(obj, data);

    widget->widget = (Evas_Object *)data;
    widget->layer = ATK_LAYER_WIDGET;

    obj->name = g_strdup(evas_object_name_get(widget->widget));
    obj->role = ATK_ROLE_UNKNOWN;

    if (!widget->widget) {
        ERR("No evas object inside EailWidget was found");
        return;
    }

    evas_object_event_callback_add(widget->widget, EVAS_CALLBACK_FOCUS_IN,
                                       eail_widget_on_focused_in, widget);
    evas_object_event_callback_add(widget->widget, EVAS_CALLBACK_FOCUS_OUT,
                                   eail_widget_on_focused_out, widget);

    evas_object_event_callback_add(widget->widget, EVAS_CALLBACK_SHOW,
                                   eail_widget_on_show, widget);
    evas_object_event_callback_add(widget->widget, EVAS_CALLBACK_HIDE,
                                   eail_widget_on_hide, widget);

    /* for window don't need that event, it would result double generating
     * focus-in event*/
    if (!ATK_IS_WINDOW(obj))
      evas_object_smart_callback_add
            (widget->widget, "focused", eail_widget_on_focused_smart, widget);

    evas_object_smart_callback_add
          (widget->widget, "unfocused", eail_widget_on_focused_out_smart, widget);

}

/**
 * @brief Default get_widget_children callback
 *
 * @param widget EailWidget instance
 * @return NULL
 */
static Eina_List *
eail_widget_get_real_widget_children(EailWidget *widget)
{
    return NULL;
}

/**
 * @brief Gets the number of children of obj
 *
 * @param obj AtkObject instance
 *
 * @returns integer representing the number of widget's children
 */
static gint
eail_widget_get_n_children(AtkObject *obj)
{
    gint n_children;
    Eina_List *children;

    children = eail_widget_get_widget_children(EAIL_WIDGET(obj));
    n_children = eina_list_count(children);

    eina_list_free(children);

    return n_children;
}

/**
 * @brief Gets reference to specified child of obj
 *
 * The caller must unreference it when it is no longer needed.
 *
 * @param obj AtkObject instance
 * @param i child index
 * @return AtkObject representing the specified child of obj
 */
static AtkObject *
eail_widget_ref_child(AtkObject *obj, gint i)
{
    Eina_List *children;
    AtkObject *child = NULL;

    children = eail_widget_get_widget_children(EAIL_WIDGET(obj));
    if (eina_list_count(children) > i) {
        child = eail_factory_get_accessible(eina_list_nth(children, i));
        g_object_ref(child);
    }

    eina_list_free(children);

    return child;
}

/**
 * @brief Gets the parent of obj
 *
 * @param obj AtkObject instance
 * @return AtkObject representing the parent of obj
 */
static AtkObject *
eail_widget_get_parent(AtkObject *obj)
{
    Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(obj));

    if (!widget) {
        return NULL;
    }

    if (obj->accessible_parent) {
        return obj->accessible_parent;
    }

    return eail_factory_get_accessible(elm_object_parent_widget_get(widget));
}

/**
 * @brief Gets the 0-based index of this accessible in its parent
 *
 * Returns -1 if the accessible does not have an accessible parent.
 *
 * @param obj AtkObject instance
 * @return integer representing obj's index in its parent
 */
static gint
eail_widget_get_index_in_parent(AtkObject *obj)
{
    gint index;
    Eina_List *l, *children;
    AtkObject *parent;
    Evas_Object *child;
    Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(obj));

    if (!widget) {
        return -1;
    }

    if (obj->accessible_parent)
    {
        parent = obj->accessible_parent;
    } else {
        parent = atk_object_get_parent(obj);
    }

    if (!parent) {
        return -1;
    }

    index = -1;
    children = eail_widget_get_widget_children(EAIL_WIDGET(parent));
    EINA_LIST_FOREACH(children, l, child) {
        ++index;
        if (child == widget) {
            break;
        }
    }

    eina_list_free(children);

    return index;
}

/**
 * @brief Gets the state set of obj
 *
 * @param obj AtkObject instance
 * @return AtkState representing the state set of obj
 */
static AtkStateSet *
eail_widget_ref_state_set(AtkObject *obj)
{
    AtkStateSet *state_set;
    Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(obj));

    state_set= ATK_OBJECT_CLASS(eail_widget_parent_class)->ref_state_set(obj);

    return eail_evas_obj_ref_state_set(widget, state_set);
}

/**
 * @brief Gets the attribute set of obj
 *
 * @param obj AtkObject instance
 * @return AtkAttributeSet representing obj's attribute set
 */
static AtkAttributeSet *
eail_widget_get_attributes(AtkObject *obj)
{
    AtkAttribute *attr;
    AtkAttributeSet *attributes;
    Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(obj));

    if (!widget) {
        return NULL;
    }

    attr = g_new(AtkAttribute, 1);
    attr->name = g_strdup("type");
    attr->value = g_strdup(elm_object_widget_type_get(widget));

    attributes = g_slist_append(NULL, attr);

    return attributes;
}

/**
 * @brief EailWidget instance initializer
 *
 * @param widget EailWidget instance
 */
static void
eail_widget_init(EailWidget *widget)
{
}

/**
 * @brief EailWidget finalizer
 *
 * Frees memory assigned to object.
 *
 * @param obj GObject instance
 */
static void
eail_widget_class_finalize(GObject *obj)
{
   EailWidget *eail_widget = EAIL_WIDGET(obj);
   Evas_Object *evas_widget = eail_widget_get_widget(eail_widget);

   if (evas_widget)
     eail_factory_unregister_wdgt_from_cache(evas_widget);

   G_OBJECT_CLASS(eail_widget_parent_class)->finalize(obj);
}

/**
 * @brief EailWidget class initializer
 *
 * Function called upon instance creation. It initializes AtkObject class
 * callbacks for EailWidget.
 *
 * @param klass EailWidgetClass instance
 */
static void
eail_widget_class_init(EailWidgetClass *klass)
{
    AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
    GObjectClass *g_object_class = G_OBJECT_CLASS(klass);

    klass->get_widget_children = eail_widget_get_real_widget_children;

    class->initialize = eail_widget_initialize;
    class->get_n_children = eail_widget_get_n_children;
    class->ref_child = eail_widget_ref_child;
    class->get_parent = eail_widget_get_parent;
    class->get_index_in_parent = eail_widget_get_index_in_parent;
    class->ref_state_set = eail_widget_ref_state_set;
    class->get_attributes = eail_widget_get_attributes;

    g_object_class->finalize = eail_widget_class_finalize;
}

/**
 * @brief Grabs focus of a component
 *
 * @param component AtkComponent instance
 * @return TRUE on success, FALSE otherwise
 */
static gboolean
eail_widget_grab_focus(AtkComponent *component)
{
    Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(component));

    return eail_evas_obj_grab_focus(widget);
}

/**
 * @brief Gets component's layer
 *
 * @param component AtkComponent instance
 * @return AtkLayer representing the component's layer
 */
static AtkLayer
eail_widget_get_layer(AtkComponent *component)
{
    return EAIL_WIDGET(component)->layer;
}

/**
 * @brief Gets the rectangle which gives the extent of the component
 *
 * @param component AtkComponent instance
 * @param [out] x upper left x coordinate of the rectangle
 * @param [out] y upper left y coordinate of the rectangle
 * @param [out] width width of the rectangle
 * @param [out] height height of the rectangle
 * @param coord_type coordinates type
 */
static void
eail_widget_get_extents(AtkComponent  *component,
                        gint          *x,
                        gint          *y,
                        gint          *width,
                        gint          *height,
                        AtkCoordType   coord_type)
{
    Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(component));

    *x = *y = G_MININT;
    *width = *height = -1;
    if (!widget) {
        return;
    }

    evas_object_geometry_get(widget, x, y, width, height);

    if (coord_type == ATK_XY_SCREEN) {
        int ee_x, ee_y;
        Ecore_Evas *ee= ecore_evas_ecore_evas_get(evas_object_evas_get(widget));

        ecore_evas_geometry_get(ee, &ee_x, &ee_y, NULL, NULL);
        *x += ee_x;
        *y += ee_y;
    }
}

/**
 * @brief Sets component position
 *
 * @param component AtkComponent instance
 * @param x upper left x coordinate
 * @param y upper left y coordinate
 * @param coord_type coordinates type
 * @return TRUE on success, FALSE otherwise
 */
static gboolean
eail_widget_set_position(AtkComponent *component,
                         gint         x,
                         gint         y,
                         AtkCoordType coord_type)
{
    Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(component));

    if (!widget) return FALSE;

    if (x < 0 || y < 0) return FALSE;

    if (coord_type == ATK_XY_SCREEN) {
        int ee_x, ee_y;
        Ecore_Evas *ee= ecore_evas_ecore_evas_get(evas_object_evas_get(widget));

        ecore_evas_geometry_get(ee, &ee_x, &ee_y, NULL, NULL);
        x -= ee_x;
        y -= ee_y;
    }

    evas_object_move(widget, x, y);

    return TRUE;
}

/**
 * @brief Sets component size
 *
 * @param component AtkComponent instance
 * @param width new width of component
 * @param height new height of component
 * @return TRUE on success, FALSE otherwise
 */
static gboolean
eail_widget_set_size(AtkComponent *component,
                     gint          width,
                     gint          height)
{
    Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(component));

    if (!widget) return FALSE;

    if (width <= 0 || height <= 0) return FALSE;

    evas_object_resize(widget, width, height);

    return TRUE;
}

/**
 * @brief Sets component extents
 *
 * @param component AtkComponent instance
 * @param x new upper left x coordinate
 * @param y new upper left y coordinate
 * @param width new width of component
 * @param height new height of component
 * @param coord_type coordinates type
 * @return TRUE on success, FALSE otherwise
 */
static gboolean
eail_widget_set_extents(AtkComponent  *component,
                        gint           x,
                        gint           y,
                        gint           width,
                        gint           height,
                        AtkCoordType   coord_type)
{
    Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(component));

    if (!widget) {
        return FALSE;
    }

    if (coord_type == ATK_XY_SCREEN) {
        int ee_x, ee_y;
        Ecore_Evas *ee= ecore_evas_ecore_evas_get(evas_object_evas_get(widget));

        ecore_evas_geometry_get(ee, &ee_x, &ee_y, NULL, NULL);
        x -= ee_x;
        y -= ee_y;
    }

    if (x < 0 || y < 0) {
        return FALSE;
    }

    evas_object_move(widget, x, y);
    evas_object_resize(widget, width, height);

    return TRUE;
}

/**
 * @brief Sets function to execute whenever component gets focus
 *
 * @param component AtkComponent instance
 * @param handler AtkFocusHandler instance
 * @returns integer representing the handler's id
 */
static guint
eail_widget_add_focus_handler(AtkComponent *component,
                              AtkFocusHandler handler)
{
   GSignalMatchType match_type;
   gulong ret;
   guint signal_id;

   match_type = G_SIGNAL_MATCH_ID;
   signal_id = g_signal_lookup("focus-event", ATK_TYPE_OBJECT);

   ret = g_signal_handler_find(component, match_type, signal_id, 0, NULL,
                               (gpointer)handler, NULL);
   if (!ret)
     {
        return g_signal_connect_closure_by_id(component,
                                              signal_id, 0,
                                              g_cclosure_new(
                                                  G_CALLBACK(handler), NULL,
                                                  (GClosureNotify)NULL),
                                              FALSE);
     }
   else
     {
        return 0;
     }
}

/**
 * @brief AtkComponent interface initialization
 *
 * @param iface EailNaviframPage instance
 */
static void
atk_component_interface_init(AtkComponentIface *iface)
{
   iface->grab_focus = eail_widget_grab_focus;
   iface->get_layer = eail_widget_get_layer;
   iface->get_extents = eail_widget_get_extents;
   iface->set_position = eail_widget_set_position;
   iface->set_size = eail_widget_set_size;
   iface->set_extents = eail_widget_set_extents;
   iface->add_focus_handler = eail_widget_add_focus_handler;
}
