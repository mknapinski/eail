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
 * @file eail_flip.c
 *
 * @brief EailFlip implementation
 */

#include <Elementary.h>

#include "eail_flip.h"
#include "eail_utils.h"
#include "eail_priv.h"

static void atk_action_interface_init(AtkActionIface *iface);

/**
 * @brief Define EailFlip GObject type
 */
G_DEFINE_TYPE_WITH_CODE(EailFlip,
                        eail_flip,
                        EAIL_TYPE_WIDGET,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_ACTION,
                                              atk_action_interface_init));

/**
 * @brief handler for event which is raised when flips visible data is changed
 *
 * @param data passed to callback
 * @param obj object that raised event
 * @param event_info additional event info
 */
void
_eail_flip_handle_changed_event(void *data,
                                Evas_Object *obj,
                                void *event_info)
{
   eail_emit_atk_signal
                  (ATK_OBJECT(data), "visible-data-changed", ATK_TYPE_OBJECT);
}

/**
 * @brief EailFlip object initialization
 *
 * @param obj EailFlip object
 * @param data user set additional initialization data
 */
static void
eail_flip_initialize(AtkObject *obj, gpointer data)
{
   Evas_Object *nested_widget = NULL;
   ATK_OBJECT_CLASS(eail_flip_parent_class)->initialize(obj, data);

   obj->role = ATK_ROLE_PAGE_TAB_LIST;

   g_return_if_fail(EAIL_IS_WIDGET(obj));
   nested_widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!nested_widget)
     {
        ERR("No evas object inside EailWidget was found");
        return;
     }

   evas_object_smart_callback_add(nested_widget, "animate,done",
                                  _eail_flip_handle_changed_event, obj);
}

/**
 * @brief Class destructor
 *
 * @param object object instance
 */
static void
eail_flip_finalize(GObject *object)
{
   EailFlip *flip = EAIL_FLIP(object);

   if (flip->flip_description) free(flip->flip_description);

   G_OBJECT_CLASS(eail_flip_parent_class)->finalize(object);
}

/**
 * @brief EailFlip instance initialization
 *
 * @param flip EailFlip instance
 */
static void
eail_flip_init(EailFlip *flip)
{
   flip->flip_description = NULL;
}

/**
 * @brief Gets widget children
 *
 * @param widget EailWidget object
 *
 * @return list of children, NULL if no children
 */
static Eina_List *
eail_flip_get_widget_children(EailWidget *widget)
{
   Eina_List *list = NULL;
   Evas_Object *child, *obj;

   obj = eail_widget_get_widget(EAIL_WIDGET(widget));

   if (obj)
     {
        if (EINA_TRUE == elm_flip_front_visible_get(obj))
          child = elm_object_part_content_get(obj, "front");
        else
          child = elm_object_part_content_get(obj, "back");

        if (child && elm_object_widget_check(child))
          list = eina_list_append(list, child);
     }

   return list;
}

/**
 * @brief GObject type initialization function
 *
 * @param klass EailFlip class
 */
static void
eail_flip_class_init(EailFlipClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
   EailWidgetClass *widget_class = EAIL_WIDGET_CLASS(klass);
   GObjectClass *gobject_class = G_OBJECT_CLASS(klass);

   class->initialize = eail_flip_initialize;
   widget_class->get_widget_children = eail_flip_get_widget_children;
   gobject_class->finalize = eail_flip_finalize;
}

/*
 * Implementation of the *AtkAction* interface
 */

/**
 * @brief Implementation of get_n_actions from AtkAction interface
 *
 * @param action EailFlip instance
 * @returns number of implemented actions
 */
static int
eail_flip_n_actions_get(AtkAction *action)
{
   return 1;
}

/**
 * @brief Implementation of get_description from AtkAction interface
 *
 * @param action EailFlip instance
 * @param i action index
 *
 * @return action description
 */
static const char*
eail_flip_description_get(AtkAction *action,
                          gint i)
{
   EailFlip *flip;
   const char *action_description;

   flip = EAIL_FLIP(action);
   if (!flip) return NULL;

   switch (i)
     {
      case 0:
         action_description = flip->flip_description;
         break;
      default:
         action_description = NULL;
         break;
     }

   return action_description;
}

/**
 * @brief Implementation of set_descritpion from AtkAction interface
 *
 * @param action EailFlip instance
 * @param i action index
 * @param description action description
 *
 * @return TRUE on success, FALSE otherwise
 */
static gboolean
eail_flip_description_set(AtkAction *action,
                          gint i,
                          const char *description)
{
   EailFlip *flip;
   char **value;

   flip = EAIL_FLIP(action);
   if (!flip) return FALSE;

   switch (i)
     {
      case 0:
         value = &flip->flip_description;
         break;
      default:
         value = NULL;
         break;
     }

   if (value)
     {
        free(*value);
        *value = g_strdup(description);
        return TRUE;
     }

   return FALSE;
}

/**
 * @brief Implementation of get_name from AtkAction interface
 *
 * @param action EailFlip instance
 * @param i action index
 *
 * @return action name
 */
static const char*
eail_flip_action_name_get(AtkAction *action,
                         int i)
{
   const char* action_name;

   switch (i)
     {
      case 0:
         action_name = "flip";
         break;
      default:
         action_name = NULL;
         break;
     }

   return action_name;
}

/**
 * @brief Implementation of do_action from AtkAction interface
 *
 * @param action EailFlip instance
 * @param i action index
 *
 * @return TRUE on success, FALSE otherwise
 */
static gboolean
eail_flip_do_action(AtkAction *action,
                    int i)
{
   Evas_Object *widget;

   widget = eail_widget_get_widget(EAIL_WIDGET(action));
   if (!widget) return FALSE;

   if ((elm_object_disabled_get(widget)) || (!evas_object_visible_get(widget)))
     return FALSE;

   const char *action_name = atk_action_get_name(action, i);
   if (!action_name) return FALSE;

   if (EINA_TRUE == elm_flip_front_visible_get(widget))
     elm_flip_go_to(widget, EINA_FALSE, ELM_FLIP_CUBE_DOWN);  // turn down
   else
     elm_flip_go_to(widget, EINA_TRUE, ELM_FLIP_CUBE_UP);     // turn up

   return TRUE;
}

/**
 * @brief AtkAction interface initializer
 *
 * @param iface an AtkActionIface
 */
static void
atk_action_interface_init(AtkActionIface *iface)
{
   g_return_if_fail(iface != NULL);

   iface->get_n_actions   = eail_flip_n_actions_get;
   iface->get_description = eail_flip_description_get;
   iface->set_description = eail_flip_description_set;
   iface->get_name        = eail_flip_action_name_get;
   iface->do_action       = eail_flip_do_action;
}
