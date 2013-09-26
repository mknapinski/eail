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
 * @file eail_hover.c
 * @brief EailHover implementation.
 */

#include <Elementary.h>

#include "eail_hover.h"
#include "eail_factory.h"

static void atk_action_interface_init(AtkActionIface *iface);

/**
 * @brief EailHover type definition
 */
G_DEFINE_TYPE_WITH_CODE(EailHover,
                        eail_hover,
                        EAIL_TYPE_WIDGET,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_ACTION,
                                              atk_action_interface_init));

/**
 * @brief EailHover initializer
 *
 * @param obj AtkObject instance
 * @param data initialization data
 */
static void
eail_hover_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_hover_parent_class)->initialize(obj, data);
   obj->role = ATK_ROLE_GLASS_PANE;
}

/**
 * @brief EailHover finalizer.
 *
 * Destroys the object and allocated resources.
 *
 * @param obj GObject instance
 */
static void
eail_hover_finalize(GObject *obj)
{
    EailHover *hover = EAIL_HOVER(obj);

    if (hover->click_description) free(hover->click_description);

    G_OBJECT_CLASS(eail_hover_parent_class)->finalize(obj);
}

/**
 * @brief EailHover instance initializer
 *
 * @param hover EailHover instance
 */
static void
eail_hover_init(EailHover *hover)
{
   hover->click_description = NULL;
}

/**
 * @brief EailHover class initializer.
 *
 * @param klass EailHoverClass instance
 */
static void
eail_hover_class_init(EailHoverClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
   GObjectClass *gobject_class = G_OBJECT_CLASS(klass);

   class->initialize = eail_hover_initialize;

   gobject_class->finalize = eail_hover_finalize;
}

/**
 * @brief Performs the specified action on the object.
 *
 * @param action AtkAction instance
 * @param i index of an action to be performed
 * @return TRUE on success, FALSE otherwise
 */
static gboolean
eail_hover_action_do_action(AtkAction *action,
                            gint i)
{
   Evas_Object *widget = NULL;
   const char *action_name = NULL;

   widget = eail_widget_get_widget(EAIL_WIDGET(action));
   if (!widget) return FALSE;

   if ((elm_object_disabled_get(widget)) || (!evas_object_visible_get(widget)))
     return FALSE;

   action_name = atk_action_get_name(action, i);
   if (!action_name) return FALSE;

   evas_object_smart_callback_call(widget, "clicked", NULL);

   return TRUE;
}

/**
 * @brief Gets number of actions available on the object.
 *
 * If there are more than one, the first one is considered the "default" action of the object.
 *
 * Implementation of get_n_actions from AtkAction interface.
 *
 * @param action AtkAction instance
 * @return integer containing the number of implemented actions
 */
static gint
eail_hover_action_get_n_actions(AtkAction *action)
{
   return 1;
}

/**
 * @brief Gets the description of the specified action of the object.
 *
 * Implementation of get_description from AtkAction interface.
 *
 * @param action AtkAction instance
 * @param i action number
 * @return string representing the description of the specified action
 */
static const gchar *
eail_hover_action_get_description(AtkAction *action,
                                  gint i)
{
   const char *action_description = NULL;
   EailHover *hover;

   hover = EAIL_HOVER(action);
   if (!hover) return NULL;

   switch (i)
     {
      case 0:
         action_description = hover->click_description;
         break;

      default:
         action_description = NULL;
         break;
     }

   return action_description;
}

/**
 * @brief Gets the name of the specified action of the object.
 *
 * Implementation of get_name from AtkAction interface.
 *
 * @param action AtkAction instance
 * @param i action number
 * @return string containing the name of the specified action
 */
static const gchar *
eail_hover_action_get_name(AtkAction *action,
                           gint i)
{
   const char* action_name = NULL;

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
 * @brief Sets a description of the specified action of the object.
 *
 * Implementation of set_description from AtkAction interface.
 *
 * @param action AtkAction instance
 * @param i action number
 * @param desc action description
 * @return TRUE if description was set successfully, FALSE otherwise
 */
static gboolean
eail_hover_action_set_description(AtkAction *action,
                                  gint i,
                                  const gchar *desc)
{
   EailHover *hover = NULL;
   char **value = NULL;

   hover = EAIL_HOVER(action);
   if (!hover) return FALSE;

   switch (i)
     {
      case 0:
         value = &hover->click_description;
         break;

      default:
         value = NULL;
         break;
     }

   if (value)
     {
        free(*value);
        *value = g_strdup(desc);

        return TRUE;
     }

   return FALSE;
}

/**
 * @brief AtkAction interface initializer
 *
 * @param iface AtkActionIface instance
 */
static void
atk_action_interface_init(AtkActionIface *iface)
{
   g_return_if_fail(iface != NULL);

   iface->do_action          = eail_hover_action_do_action;
   iface->get_n_actions      = eail_hover_action_get_n_actions;
   iface->get_description    = eail_hover_action_get_description;
   iface->get_name           = eail_hover_action_get_name;
   iface->set_description    = eail_hover_action_set_description;
}
