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
 * @file eail_ctxpopup.c
 * @brief Implementation of Ctxpopup widget
 */

#include <Elementary.h>

#include "eail_ctxpopup.h"
#include "eail_factory.h"
#include "eail_priv.h"

static void atk_action_interface_init(AtkActionIface *iface);

/**
 * @brief Define EailCtxpopup GObject type
 */
G_DEFINE_TYPE_WITH_CODE(EailCtxpopup,
                        eail_ctxpopup,
                        EAIL_TYPE_WIDGET,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_ACTION,
                                              atk_action_interface_init));

/**
 * @brief Initializer for AtkObject
 *
 * @param obj an AtkObject
 * @param data Initialization data
 */
static void
eail_ctxpopup_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_ctxpopup_parent_class)->initialize(obj, data);

   obj->role = ATK_ROLE_POPUP_MENU;
}

/**
 * @brief Destructor for EailCtxpopup class
 *
 * @param object GObject object to be finalized
 */
static void
eail_ctxpopup_finalize(GObject *object)
{
   EailCtxpopup *ctxpopup = EAIL_CTXPOPUP(object);

   if (ctxpopup->dismiss_description) free(ctxpopup->dismiss_description);

   G_OBJECT_CLASS(eail_ctxpopup_parent_class)->finalize(object);
}

/**
 * @brief EailCtxpopup GObject instance initializer
 *
 * @param ctxpopup an EailCtxpopup
 */
static void
eail_ctxpopup_init(EailCtxpopup *ctxpopup)
{
   ctxpopup->dismiss_description = NULL;
}

/**
 * @brief Helper function for getting nested content in "elm_popup" widget
 *
 * @param obj an AtkObject
 *
 * @returns nested widget content from "elm_popup" widget
 */
static Evas_Object *
_eail_get_nested_widget(AtkObject *obj)
{
   Evas_Object *ctxpopup_widget = NULL, *nested_widget = NULL;
   /* getting widget of ctxpopup class */
   ctxpopup_widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!ctxpopup_widget)
     {
        ERR("No widget found for EailCtxpopup object!");
     }

   nested_widget = elm_object_part_content_get(ctxpopup_widget, "default");

   return nested_widget;
}

/**
 * @brief Implementation AtkObject->get_n_children callback
 *
 * ATK doc says:\n
 * Gets the number of accessible children of the accessible.
 * @param obj an AtkObject
 *
 * @returns an integer representing the number of accessible children of
 * the accessible
 */
static gint
eail_ctxpopup_get_n_children(AtkObject *obj)
{
   Evas_Object *nested_widget = NULL;

   nested_widget = _eail_get_nested_widget(obj);
   if (nested_widget)
     return 1;

   return 0;
}

/**
 * @brief Implementation AtkObject->ref_child callback
 *
 * ATK doc says:\n
 * Gets a reference to the specified accessible child of the object. The
 * accessible children are 0-based so the first accessible child is at index 0,
 * the second at index 1 and so on.
 *
 * @param obj an AtkObject
 * @param i index of a child
 *
 * @returns an AtkObject representing the specified accessible child of the
 * accessible.
 */
static AtkObject *
eail_ctxpopup_ref_child(AtkObject *obj, gint i)
{
   Evas_Object *nested_widget = NULL;
   AtkObject *atk_obj;

   nested_widget = _eail_get_nested_widget(obj);
   atk_obj = eail_factory_get_accessible(nested_widget);

   if (atk_obj)
     g_object_ref(atk_obj);

   return atk_obj;
}

/**
 * @brief Initializer for EailPopup GObject class (defines callbacks for
 * base AtkObject)
 *
 * @param klass an EailCtxpopupClass
 */
static void
eail_ctxpopup_class_init(EailCtxpopupClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
   GObjectClass *gobject_class = G_OBJECT_CLASS(klass);

   class->initialize = eail_ctxpopup_initialize;
   class->get_n_children = eail_ctxpopup_get_n_children;
   class->ref_child = eail_ctxpopup_ref_child;
   gobject_class->finalize = eail_ctxpopup_finalize;
}

/*
 * Implementation of the *AtkAction* interface
 */

/**
 * @brief Implementation of get_n_actions from AtkAction interface
 *
 * @param action EailCtxpopup instance
 *
 * @return number of available actions
 */
static int
eail_ctxpopup_n_actions_get(AtkAction *action)
{
   return 1;
}

/**
 * @brief Implementation of get_description from AtkAction interface
 *
 * @param action EailCtxpopup instance
 * @param i action index
 *
 * @return action description
 */
static const char*
eail_ctxpopup_description_get(AtkAction *action,
                              gint i)
{
   EailCtxpopup *ctxpopup;
   const char *action_description;

   ctxpopup = EAIL_CTXPOPUP(action);
   if (!ctxpopup) return NULL;

   switch (i)
     {
      case 0:
         action_description = ctxpopup->dismiss_description;
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
 * @param action EailCtxpopup instance
 * @param i action index
 * @param description action description
 *
 * @return TRUE on success, FALSE otherwise
 */
static gboolean
eail_ctxpopup_description_set(AtkAction *action,
                              gint i,
                              const char *description)
{
   EailCtxpopup *ctxpopup;
   char **value;

   ctxpopup = EAIL_CTXPOPUP(action);
   if (!ctxpopup) return FALSE;

   switch (i)
     {
      case 0:
         value = &ctxpopup->dismiss_description;
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
 * @param action EailCtxpopup instance
 * @param i action index
 *
 * @return action name
 */
static const char*
eail_ctxpopup_action_name_get(AtkAction *action,
                              int i)
{
   const char* action_name;

   switch (i)
     {
      case 0:
         action_name = "dismiss";
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
 * @param action EailCtxpopup instance
 * @param i action index
 *
 * @return TRUE on success, FALSE otherwise
 */
static gboolean
eail_ctxpopup_do_action(AtkAction *action,
                        int i)
{
   Evas_Object *widget;

   widget = eail_widget_get_widget(EAIL_WIDGET(action));
   if (!widget) return FALSE;

   if ((elm_object_disabled_get(widget)) || (!evas_object_visible_get(widget)))
     return FALSE;

   const char *action_name = atk_action_get_name(action, i);
   if (!action_name) return FALSE;

   if (!g_strcmp0(action_name, "dismiss"))
     elm_ctxpopup_dismiss(widget);
   else
     return FALSE;

   return TRUE;
}

/**
 * @brief AtkAction interface initializer
 *
 * Function called upon instance creation. It initializes AtkAction interface
 * implementation i.e hooks method pointers in the interface structure
 * to the implementing class's implementation.
 *
 * @param iface an AtkActionIface
 */
static void
atk_action_interface_init(AtkActionIface *iface)
{
   g_return_if_fail(iface != NULL);

   iface->get_n_actions   = eail_ctxpopup_n_actions_get;
   iface->get_description = eail_ctxpopup_description_get;
   iface->set_description = eail_ctxpopup_description_set;
   iface->get_name        = eail_ctxpopup_action_name_get;
   iface->do_action       = eail_ctxpopup_do_action;
}
