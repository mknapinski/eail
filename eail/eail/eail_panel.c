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
 * @file eail_panel.c
 * @brief EailPanel implementation
 */

#include <atk/atk.h>
#include <Elementary.h>
#include "eail_panel.h"

/**
 * @brief EailPanel 'toggle' action name
 */
#define EAIL_PANEL_TOGGLE_ACTION "toggle"

/**
 * @brief Definition of EailPanel as GObject
 *
 * EailFileselectorEntry is extended EAIL_TYPE_WIDGET with ATK_TYPE_ACTION
 * interface implemented
 */
G_DEFINE_TYPE(EailPanel, eail_panel, EAIL_TYPE_ACTION_WIDGET);

/**
 * @brief Gets a reference to the state set of the accessible
 *
 * The caller must unreference it when it is no longer needed.
 *
 * Implementation of AtkObject->ref_state_set callback.
 *
 * @param obj AtkObject instance
 *
 * @returns AtkStateSet representing the state set of the
 * accessible
 */
static AtkStateSet*
eail_panel_ref_state_set(AtkObject *obj)
{
   Evas_Object *widget;
   Eina_Bool hidden;
   AtkStateSet *state_set;

   g_return_val_if_fail(EAIL_IS_PANEL(obj), NULL);

   state_set = ATK_OBJECT_CLASS(eail_panel_parent_class)->ref_state_set(obj);
   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   hidden = elm_panel_hidden_get(widget);
   if (!hidden)
     atk_state_set_add_state(state_set, ATK_STATE_VISIBLE);

   return state_set;
}

/**
 * @brief Gets accessible children of a panel
 *
 * Implementation of eail_widget_get_widget_children callback from
 * EailWidget.
 *
 * @param widget EailWidget instance
 *
 * @returns Eina_List representing a list of the panel widget's children
 */
static Eina_List*
eail_panel_children_get(EailWidget *widget)
{
   Evas_Object *obj, *child;
   Eina_List *list;

   g_return_val_if_fail(EAIL_IS_PANEL(widget), NULL);

   obj = eail_widget_get_widget(widget);
   if (obj == NULL) return NULL;
   child = elm_object_part_content_get(obj, "default");
   if (child == NULL) return NULL;

   list = NULL;
   list = eina_list_append(list, child);

   return list;
}

/**
 * @brief Initializes the Action interface for panel
 *
 * @param action AtkAction instance
 * @param data data passed to callback
 * @returns TRUE on success, FALSE otherwise
 */
static gboolean
eail_panel_action_toggle(AtkAction *action, void *data)
{
   Evas_Object *widget;

   g_return_val_if_fail(EAIL_IS_PANEL(action), FALSE);

   widget = eail_widget_get_widget(EAIL_WIDGET(action));
   if (!widget) return FALSE;

   elm_panel_toggle(widget);

   return TRUE;
}

/**
 * @brief Hooks EailPanel actions callbacks
 *
 * @param action_widget EailActionWidget instance
 */
static void
eail_panel_actions_init(EailActionWidget *action_widget)
{
   eail_action_widget_action_append(action_widget,
                                    EAIL_PANEL_TOGGLE_ACTION, NULL,
                                    eail_panel_action_toggle);
}

/**
 * @brief Initializer for AtkObjectClass
 *
 * @param obj AtkObject instance
 * @param data initialization data
 */
static void
eail_panel_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_panel_parent_class)->initialize(obj, data);
   obj->role = ATK_ROLE_PANEL;

   eail_panel_actions_init(EAIL_ACTION_WIDGET(obj));
}

/**
 * @brief Initializer for GObject class
 *
 * @param panel EailPanel instance
 */
static void
eail_panel_init(EailPanel *panel)
{
   panel->toggle_description = NULL;
}

/**
 * @brief Destructor for panel object
 *
 * @param g_object GObject instance to be finalized
 */
static void eail_panel_finalize(GObject *g_object)
{
   EailPanel *panel = EAIL_PANEL(g_object);
   if (panel->toggle_description)
     {
        free(panel->toggle_description);
        panel->toggle_description = NULL;
     }
   G_OBJECT_CLASS(g_object)->finalize(g_object);
}

/**
 * @brief Initializer for GObject panel class
 *
 * Defines callbacks for base AtkObject.
 *
 * @param klass EailPanelClass instance
 */
static void
eail_panel_class_init(EailPanelClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
   EailWidgetClass *widget_class = EAIL_WIDGET_CLASS(klass);
   GObjectClass *g_object_class = G_OBJECT_CLASS(klass);

   g_object_class->finalize = eail_panel_finalize;

   widget_class->get_widget_children = eail_panel_children_get;

   class->initialize = eail_panel_initialize;
   class->ref_state_set = eail_panel_ref_state_set;
}
