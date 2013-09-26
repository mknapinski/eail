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
 * @file eail_bubble.c
 * @brief EailBubble implementation
 */

#include <Elementary.h>

#include "eail_bubble.h"

static void atk_action_interface_init(AtkActionIface *iface);

/**
 * @brief Definition of EailBubble type
 */
G_DEFINE_TYPE_WITH_CODE(EailBubble,
                        eail_bubble,
                        EAIL_TYPE_TEXT,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_ACTION,
                                              atk_action_interface_init));

/*
 * Implementation of the *AtkObject* interface
 */

/**
 * @brief EailBubble object initialization
 *
 * @param obj EailBubble instance
 * @param data user set additional initialization data
 */
static void
eail_bubble_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_bubble_parent_class)->initialize(obj, data);

   obj->role = ATK_ROLE_FILLER;
}

/**
 * @brief Class destructor
 *
 * @param object GObject instance
 */
static void
eail_bubble_finalize(GObject *object)
{
   EailBubble *bubble = EAIL_BUBBLE(object);

   if (bubble->click_description) free(bubble->click_description);

   G_OBJECT_CLASS(eail_bubble_parent_class)->finalize(object);
}

/**
 * @brief EailBubble instance initialization
 *
 * @param bubble EailBubble instance
 */
static void
eail_bubble_init(EailBubble *bubble)
{
   bubble->click_description = NULL;
}

/**
 * @brief Gets the list of widget's accessible children
 *
 * @param widget EailWidget instance
 *
 * @return Eina_List representing the list of accessible children
 * or NULL if widget has no children
 */
static Eina_List *
eail_bubble_get_widget_children(EailWidget *widget)
{
   Eina_List *list = NULL;
   Evas_Object *child, *obj;

   obj = eail_widget_get_widget(EAIL_WIDGET(widget));

   if (obj)
     {
        child = elm_object_part_content_get(obj, "default");
        if (child && elm_object_widget_check(child))
          list = eina_list_append(list, child);

        child = elm_object_part_content_get(obj, "icon");
        if (child && elm_object_widget_check(child))
          list = eina_list_append(list, child);
     }

   return list;
}

/**
 * @brief GObject type initialization function
 *
 * @param klass EailBubbleClass instance
 */
static void
eail_bubble_class_init(EailBubbleClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
   EailWidgetClass *widget_class = EAIL_WIDGET_CLASS(klass);
   GObjectClass *gobject_class = G_OBJECT_CLASS(klass);

   class->initialize = eail_bubble_initialize;
   widget_class->get_widget_children = eail_bubble_get_widget_children;
   gobject_class->finalize = eail_bubble_finalize;
}

/*
 * Implementation of the *AtkAction* interface
 */

/**
 * @brief Gets the number of accessible actions available on the object
 *
 * Implementation of get_n_actions from AtkAction interface.
 *
 * @param action EailBubble instance
 *
 * @returns integer representing the number of available actions
 */
static int
eail_bubble_n_actions_get(AtkAction *action)
{
   return 1;
}

/**
 * @brief Gets the description string of the specified action
 *
 * Implementation of get_description from AtkAction interface.
 *
 * @param action EailBubble instance
 * @param i action index
 *
 * @return string representing the specified action's description
 */
static const char*
eail_bubble_description_get(AtkAction *action,
                            gint i)
{
   EailBubble *bubble;
   const char *action_description;

   bubble = EAIL_BUBBLE(action);
   if (!bubble) return NULL;

   switch (i)
     {
      case 0:
         action_description = bubble->click_description;
         break;
      default:
         action_description = NULL;
         break;
     }

   return action_description;
}

/**
 * @brief Sets a description of the specified action of the object
 *
 * Implementation of set_description from AtkAction interface.
 *
 * @param action AtkAction instance
 * @param i action index
 * @param description action description
 *
 * @return TRUE on success, FALSE otherwise
 */
static gboolean
eail_bubble_description_set(AtkAction *action,
                            gint i,
                            const char *description)
{
   EailBubble *bubble;
   char **value;

   bubble = EAIL_BUBBLE(action);
   if (!bubble) return FALSE;

   switch (i)
     {
      case 0:
         value = &bubble->click_description;
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
 * @brief Gets the name string of the specified action
 *
 * Implementation of get_name from AtkAction interface.
 *
 * @param action AtkAction instance
 * @param i action index
 *
 * @return string representing the specified action's name
 */
static const char*
eail_bubble_action_name_get(AtkAction *action,
                            int i)
{
   const char* action_name;

   switch (i)
     {
      case 0:
         action_name = "click";
         break;
      default:
         action_name = NULL;
         break;
     }

   return action_name;
}

/**
 * @brief Performs the specified action on the object
 *
 * Implementation of do_action from AtkAction interface.
 *
 * @param action AtkAction instance
 * @param i action index
 *
 * @return TRUE on success, FALSE otherwise
 */
static gboolean
eail_bubble_do_action(AtkAction *action,
                      int i)
{
   Evas_Object *widget;

   widget = eail_widget_get_widget(EAIL_WIDGET(action));
   if (!widget) return FALSE;

   if ((elm_object_disabled_get(widget)) || (!evas_object_visible_get(widget)))
     return FALSE;

   const char *action_name = atk_action_get_name(action, i);
   if (!action_name) return FALSE;

   evas_object_smart_callback_call(widget, "clicked", NULL);

   return TRUE;
}

/**
 * @brief AtkAction interface initializer
 *
 * @param iface action interface to be filled
 **/
static void
atk_action_interface_init(AtkActionIface *iface)
{
   g_return_if_fail(iface != NULL);

   iface->get_n_actions   = eail_bubble_n_actions_get;
   iface->get_description = eail_bubble_description_get;
   iface->set_description = eail_bubble_description_set;
   iface->get_name        = eail_bubble_action_name_get;
   iface->do_action       = eail_bubble_do_action;
}
