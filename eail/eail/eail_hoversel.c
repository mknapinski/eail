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
 * @file eail_hoversel.c
 * @brief EailHoversel implementation.
 */

#include <Elementary.h>

#include "eail_hoversel.h"
#include "eail_factory.h"
#include "eail_item_parent.h"
#include "eail_priv.h"

static void atk_action_interface_init(AtkActionIface *iface);
static void eail_item_parent_interface_init(EailItemParentIface *iface);

/**
 * @brief EailHoversel type definition
 */
G_DEFINE_TYPE_WITH_CODE(EailHoversel,
                        eail_hoversel,
                        EAIL_TYPE_WIDGET,
                        G_IMPLEMENT_INTERFACE(EAIL_TYPE_ITEM_PARENT,
                                              eail_item_parent_interface_init)
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_ACTION,
                                              atk_action_interface_init));

/* AtkObject */
static AtkStateSet *eail_hoversel_ref_state_set(AtkObject *obj);
static gint eail_hoversel_get_n_children(AtkObject *obj);
static AtkObject *eail_hoversel_ref_child(AtkObject *obj,
                                          gint i);
static void eail_hoversel_initialize(AtkObject *obj,
                                     gpointer data);
static void eail_hoversel_finalize(GObject *obj);

/* AtkAction */
static const char *eail_hoversel_description_get(AtkAction *action,
                                                 gint i);
static gboolean eail_hoversel_description_set(AtkAction *action,
                                              gint i,
                                              const char *description);
static const char *eail_hoversel_action_name_get(AtkAction *action,
                                                 int i);
static int eail_hoversel_n_actions_get(AtkAction *action);
static gboolean eail_hoversel_do_action(AtkAction *action,
                                        int i);

/* EailItemParent */

/* Helper functions */
static const Eina_List *eail_hoversel_items_get(EailHoversel *obj);

/*
 * Implementation of the *AtkObject* interface
 */

/**
 * @brief EailHoverser initializer
 *
 * @param obj AtkObject instance
 * @param data initialization data
 */
static void
eail_hoversel_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_hoversel_parent_class)->initialize(obj, data);
   obj->role = ATK_ROLE_GLASS_PANE;
}

/**
 * @brief EailHoversel finalizer.
 *
 * Destroys object and allocated resources.
 *
 * @param obj object that needs to be finalized
 */
static void
eail_hoversel_finalize(GObject *obj)
{
    EailHoversel *hoversel = EAIL_HOVERSEL(obj);

    if (hoversel->shrink_description) free(hoversel->shrink_description);
    if (hoversel->expand_description) free(hoversel->expand_description);

    G_OBJECT_CLASS(eail_hoversel_parent_class)->finalize(obj);
}

/**
 * @brief EailHoversel instance initializer.
 *
 * @param hoversel EailHoversel instance
 */
static void
eail_hoversel_init(EailHoversel *hoversel)
{
   hoversel->shrink_description = NULL;
   hoversel->expand_description = NULL;
}

/**
 * @brief Gets the state set of the accessible.
 *
 * The caller must unreference it when it is no longer needed.
 *
 * @param accessible EailHoversel instance
 * @return AtkStateSet containing the state set of the accessible
 */
static AtkStateSet*
eail_hoversel_ref_state_set(AtkObject *accessible)
{
   AtkStateSet *state_set = NULL;
   Evas_Object *widget = NULL;

   state_set =
       ATK_OBJECT_CLASS(eail_hoversel_parent_class)->ref_state_set(accessible);
   if (!state_set)
     {
        printf("NULL STATE SET");
     }

   widget = eail_widget_get_widget(EAIL_WIDGET(accessible));
   if (!widget)
     {
        atk_state_set_add_state(state_set, ATK_STATE_DEFUNCT);
        return state_set;
     }

   if (elm_hoversel_expanded_get(widget))
     {
        atk_state_set_add_state(state_set, ATK_STATE_EXPANDED);

        if (elm_hoversel_horizontal_get(widget))
          atk_state_set_add_state(state_set, ATK_STATE_HORIZONTAL);
        else
          atk_state_set_add_state(state_set, ATK_STATE_VERTICAL);
     }
   else
     {
        atk_state_set_add_state(state_set, ATK_STATE_DEFAULT);
     }

   return state_set;
}

/**
 * @brief Gets the number of accessible children of obj
 *
 * @param obj AtkObject instance
 * @return integer containing the number of obj's children
 */
static gint
eail_hoversel_get_n_children(AtkObject *obj)
{
   const Eina_List *list;

   g_return_val_if_fail(EAIL_IS_HOVERSEL(obj), 0);

   list = eail_hoversel_items_get(EAIL_HOVERSEL(obj));

   return eina_list_count(list);
}

/**
 * @brief Gets a reference to the specified accessible child of the object.
 *
 * The accessible children are 0-based so the first accessible child is at index 0, the second at index 1 and so on.
 *
 * Implementation of ref_child from AtkObject interface.
 *
 * The caller must unreference it when it is no longer needed.
 *
 * @param obj AtkObject instance
 * @param i child index
 * @return AtkObject containing the reference to the specified accessible child
 */
static AtkObject *
eail_hoversel_ref_child(AtkObject *obj,
                            gint i)
{
   const Eina_List *items;
   AtkObject *child = NULL;

   g_return_val_if_fail(EAIL_IS_HOVERSEL(obj), NULL);

   items = eail_hoversel_items_get(EAIL_HOVERSEL(obj));
   if (eina_list_count(items) > i)
     {
        child = eail_factory_get_item_atk_obj
                          (eina_list_nth(items, i), ATK_ROLE_LABEL, obj);

        g_object_ref(child);
     }
   else
     ERR("Tried to ref child with index %d out of bounds!", i);

   return child;
}

/**
 * @brief EailHoversel class initializer.
 *
 * @param klass EailHoverselClass instance
 */
static void
eail_hoversel_class_init(EailHoverselClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
   GObjectClass *object_class = G_OBJECT_CLASS(klass);

   class->initialize = eail_hoversel_initialize;
   class->ref_state_set = eail_hoversel_ref_state_set;
   class->get_n_children = eail_hoversel_get_n_children;
   class->ref_child = eail_hoversel_ref_child;

   object_class->finalize = eail_hoversel_finalize;
}

/*
 * Implementation of the *AtkAction* interface
 */

/**
 * @brief AtkAction interface initializer.
 *
 * @param iface EailHoversel instance
 */
static void
atk_action_interface_init(AtkActionIface *iface)
{
   g_return_if_fail(iface != NULL);

   iface->do_action          = eail_hoversel_do_action;
   iface->get_n_actions      = eail_hoversel_n_actions_get;
   iface->get_description    = eail_hoversel_description_get;
   iface->get_name           = eail_hoversel_action_name_get;
   iface->set_description    = eail_hoversel_description_set;
}

/**
 * @brief Gets the description of the specified action of the object.
 *
 * Implementation of get_description from AtkAction interface.
 *
 * @param action EailHoversel instance
 * @param i action index
 * @return string representing the description of the specified action
 */
static const char*
eail_hoversel_description_get(AtkAction *action,
                              gint i)
{
   EailHoversel *hoversel;
   const char *action_description;

   hoversel = EAIL_HOVERSEL(action);
   if (!hoversel) return NULL;

   switch (i)
     {
       case 0:
           action_description = hoversel->shrink_description;
           break;
       case 1:
           action_description = hoversel->expand_description;
           break;
       default:
           action_description = NULL;
           break;
     }

   return action_description;
}

/**
 * @brief Sets a description of the specified action of the object.
 *
 * Implementation of set_description from AtkAction interface.
 *
 * @param action AtkAction instance
 * @param i index of the action to be performed
 * @param description action desciption
 * @return TRUE if description was set successfully, FALSE otherwise
 */
static gboolean
eail_hoversel_description_set(AtkAction *action,
                              gint i,
                              const char *description)
{
   EailHoversel *hoversel;
   char **value;

   hoversel = EAIL_HOVERSEL(action);
   if (!hoversel) return FALSE;

   switch (i)
     {
       case 0:
           value = &hoversel->shrink_description;
           break;
       case 1:
           value = &hoversel->expand_description;
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
 * @brief Gets the name of the specified action of the object.
 *
 * Implementation of get_name from AtkAction interface.
 *
 * @param action AtkAction instance
 * @param i index of the action to be performed
 * @return string representing the name of the specified action
 */
static const char*
eail_hoversel_action_name_get(AtkAction *action,
                              int i)
{
   const char* action_name;

   switch (i)
     {
       case 0:
           action_name = "shrink";
           break;
       case 1:
           action_name = "expand";
           break;
       default:
           action_name = NULL;
           break;
     }

   return action_name;
}

/**
 * @brief Gets the number of accessible actions available on the object.
 *
 * If there are more than one, the first one is considered the "default" action of the object.
 *
 * Implementation of get_n_actions from AtkAction interface.
 *
 * @param action AtkAction instance
 * @return integer containing the number of implemented actions
 */
static int
eail_hoversel_n_actions_get(AtkAction *action)
{
   return 2;
}

/**
 * @brief Performs the specified action on the object.
 *
 * Implementation of do_action from AtkAction interface.
 *
 * @param action AtkAction instance
 * @param i action index
 * @return TRUE on success, FALSE otherwise
 */
static gboolean
eail_hoversel_do_action(AtkAction *action,
                        int i)
{
   Evas_Object *widget;

   widget = eail_widget_get_widget(EAIL_WIDGET(action));
   if (!widget) return FALSE;

   if ((elm_object_disabled_get(widget)) || (!evas_object_visible_get(widget)))
     return FALSE;

   const char *action_name = atk_action_get_name(action, i);
   if (!action_name) return FALSE;

   if (!g_strcmp0(action_name, "expand"))
     {
        if (elm_hoversel_expanded_get(widget))
          return FALSE;

        elm_hoversel_hover_begin(widget);
        atk_object_notify_state_change(ATK_OBJECT(action), ATK_STATE_EXPANDED, TRUE);
     }
   else if (!g_strcmp0(action_name, "shrink"))
     {
        if (!elm_hoversel_expanded_get(widget))
          return FALSE;

        elm_hoversel_hover_end(widget);
        atk_object_notify_state_change(ATK_OBJECT(action), ATK_STATE_EXPANDED, FALSE);
     }

   return TRUE;
}

/*
 * Implementation of the *EailItemParent* interface
 */

/**
 * @brief Gets the name of hoversel's child
 *
 * @param parent parent instance
 * @param item child instance
 *
 * @returns string representing the name of the child
 */
static const gchar *
eail_hoversel_item_name_get(EailItemParent *parent, EailItem *item)
{
   Elm_Object_Item *it = eail_item_get_item(item);
   if (!it) return NULL;

   return elm_object_item_part_text_get(it, NULL);
}

/**
 * @brief Checks if content get is supported
 *
 * Implementation of EailItemParent->is_content_get_supported callback.
 *
 * @param parent EailItemParent instance
 * @param item EailItem instance
 * @returns FALSE as there is no content support
 */
gboolean
eail_hoversel_content_get_support(EailItemParent   *parent,
                                  EailItem         *item)
{
   return FALSE;
}

/**
 * @brief EailItemParent interface initializer
 *
 * @param iface EailItemParentIface instance
 */
static void
eail_item_parent_interface_init(EailItemParentIface *iface)
{
   iface->get_item_name = eail_hoversel_item_name_get;
   iface->is_content_get_supported = eail_hoversel_content_get_support;
}

/*
 * Helper functions
 */

/**
 * @brief Gets obj's items list.
 *
 * @param obj EailHoversel instance
 * @return Eina_List containing obj's items
 */
static const Eina_List *
eail_hoversel_items_get(EailHoversel *obj)
{
   Evas_Object *widget;
   const Eina_List *items = NULL;

   g_return_val_if_fail(EAIL_HOVERSEL(obj), NULL);

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return NULL;

   items = elm_hoversel_items_get(widget);
   if (!items) return NULL;

   return items;
}
