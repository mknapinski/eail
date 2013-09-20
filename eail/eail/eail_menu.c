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
 * @file eail_menu.c
 * @brief Implementation of menu widget
 */

#include <Elementary.h>

#include "eail_menu.h"
#include "eail_item_parent.h"
#include "eail_factory.h"
#include "eail_priv.h"

static void eail_item_parent_interface_init(EailItemParentIface *iface);

/**
 * @brief Definition of EailMenu as GObject
 *
 * EailList is extended Widget with EAIL_TYPE_ITEM_PARENT implemented
 */
G_DEFINE_TYPE_WITH_CODE(EailMenu,
                        eail_menu,
                        EAIL_TYPE_WIDGET,
                        G_IMPLEMENT_INTERFACE(EAIL_TYPE_ITEM_PARENT,
                                              eail_item_parent_interface_init));

/*
 * Implementation of the *AtkObject* interface
 */

/**
 * @brief Initializer for AtkObjectClass
 */
static void
eail_menu_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_menu_parent_class)->initialize(obj, data);

   obj->role = ATK_ROLE_MENU;
}

/**
 * @brief Prepares Eina_List filled with Elm_Object_Item* objects
 * representing items in menu
 *
 * @param list an EailMenu
 *
 * @return filled list with list items. Call eina_list_free on that list when
 * results processing has been finished
 */
static Eina_List *
eail_menu_get_items(EailMenu *list)
{
   Eina_List *items = NULL;
   Elm_Object_Item *item;
   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(list));

   item = elm_menu_first_item_get(widget);
   while (item)
     {
        items = eina_list_append(items, item);
        item = elm_menu_item_next_get(item);
     }

   return items;
}

/**
 * @brief Implementation of AtkObject->get_n_children callback
 *
 * ATK doc says:\n
 * Gets the number of accessible children of the accessible.
 *
 * @param obj an AtkObject
 *
 * @returns an integer representing the number of accessible children of
 * the accessible
 */
static gint
eail_menu_get_n_children(AtkObject *obj)
{
   gint n_items;
   Eina_List *items;

   items = eail_menu_get_items(EAIL_MENU(obj));
   n_items = eina_list_count(items);

   eina_list_free(items);

   return n_items;
}

/**
 * @brief Implementation of AtkObject->ref_child callback
 *
 * ATK doc says:\n
 * Gets a reference to the specified accessible child of the object. The
 * accessible children are 0-based so the first accessible child is at index 0,
 * the second at index 1 and so on.
 *
 * @param obj an AtkObject
 * @param i child index
 *
 * @returns an AtkObject representing the specified accessible child of the
 * accessible.
 */
static AtkObject *
eail_menu_ref_child(AtkObject *obj, gint i)
{
   Eina_List *items;
   AtkObject *child = NULL;

   items = eail_menu_get_items(EAIL_MENU(obj));
   if (eina_list_count(items) > i)
     {
        child = eail_factory_get_item_atk_obj
                 (eina_list_nth(items, i), ATK_ROLE_LIST_ITEM, obj);

        g_object_ref(child);
     }

   eina_list_free(items);

   return child;
}


/**
 * @brief Gets name of EailItem
 *
 * Implementation of AtkObject->get_name callback
 *
 * ATK doc says:
 * Gets the accessible name of the accessible.
 *
 * @param obj an AtkObject
 *
 * @returns a character string representing the accessible description of
 * the accessible.
 */
static const gchar *
eail_menu_get_name(AtkObject *obj)
{
   AtkObject *parent = NULL;
   const gchar *atk_name = NULL;

   /* returning name from ATK default implementation if available */
   atk_name = ATK_OBJECT_CLASS(eail_menu_parent_class)->get_name(obj);
   if (atk_name)
     return atk_name;

   parent = atk_object_get_parent(obj);
   if (!parent) return NULL;

   return parent->name;
}

/**
 * @brief Initializer for GObject EailMenu instance
 * @param menu an EailMenu
 */
static void
eail_menu_init(EailMenu *menu)
{
}

/**
 * @brief Initializer for GObject class (defines callbacks for base AtkObject)
 * @param klass an EailMenuClass
 */
static void
eail_menu_class_init(EailMenuClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);

   class->initialize = eail_menu_initialize;
   class->get_n_children = eail_menu_get_n_children;
   class->ref_child = eail_menu_ref_child;
   class->get_name = eail_menu_get_name;
}

/* Child MenuItem handling  - implementation of EailItemParent interface*/

/**
 * @brief Get name of list child
 *
 * @param parent EailList instance
 * @param item EailList child instance
 *
 * @returns const gchar * representing name of the child
 */
static const gchar *
eail_menu_item_name_get(EailItemParent *parent, EailItem *item)
{
   Elm_Object_Item *it = eail_item_get_item(item);
   if (!it) return NULL;

   return elm_object_item_part_text_get(it, NULL);
}

/**
 * @brief Implementation of EailItemParent->get_evas_obj callback
 *
 * @param parent an EailItemParent
 * @param item an EailItem
 * @returns menu item
 */
Evas_Object *
eail_menu_item_get_evas_obj(EailItemParent   *parent,
                       EailItem         *item)
{
   Elm_Object_Item *it = eail_item_get_item(item);

   if (!it)
     {
        ERR("Error. Could not get Elm_Object_item from EailItem");
        return NULL;
     }

   return elm_menu_item_object_get(it);
}

/**
 * @brief Implementation of EailItemParent->get_actions_supported callback
 *
 * @param parent an EailItemParent
 * @param item an EailItem
 * @returns enum representing supported actions
 */
static gint
eail_menu_get_actions_supported(EailItemParent   *parent,
                                   EailItem         *item)
{
   return EAIL_ACTION_SUPPORTED_CLICK;
}

/**
 * @brief Prepares Eina_List filled with Elm_Object_Item* objects
 * representing items in the list
 * @param menu_item_obj child of menu widget
 *
 * @return filled list with menu items. Call eina_list_free on that list when
 * results processing has been finished
 */
static const Eina_List *
eail_menu_item_get_items(EailItem *menu_item_obj)
{
   const Eina_List *items = NULL;
   EailItem *eail_item = EAIL_ITEM(menu_item_obj);
   Elm_Object_Item *it = eail_item_get_item(eail_item);

   items = elm_menu_item_subitems_get(it);

   return items;
}

/**
 * @brief Implementation of AtkObject->get_n_children callback
 *
 * ATK doc says:\n
 * Gets the number of accessible children of the accessible.
 *
 * @param parent EAIL representation of menu widget
 * @param eail_item child of menu widget
 *
 * @returns an integer representing the number of accessible children of
 * the accessible
 */
static gint
eail_menu_item_get_n_children(EailItemParent *parent, EailItem *eail_item)
{
   gint n_items;
   const Eina_List *items;
   Evas_Object *inside_widget = NULL;

   items = eail_menu_item_get_items(eail_item);
   n_items = eina_list_count(items);

   /* increasing child items count for widgets that are stored inside
    * menu item (eg. icon or button). They will be appended at the end
    * of the children list in eail_menu_item_ref_child func*/
   inside_widget = elm_object_item_content_get(eail_item_get_item(eail_item));
   if (inside_widget)
     n_items++;

   return n_items;
}

/**
 * @brief Creates content widgets from given menu_item AtkObject
 *
 * @param menu_item_obj an AtkObject menu item
 * @returns AtkObject representing nested content in menu item
 */
static AtkObject *
_eail_menu_item_create_content_atk_obj(AtkObject *menu_item_obj)
{
   EailItem *eail_item = EAIL_ITEM(menu_item_obj);
   Evas_Object *content_widget = NULL;
   AtkObject *content_atk_obj = NULL;

   content_widget = elm_object_item_content_get(eail_item_get_item(eail_item));

   if (!content_widget)
     {
        ERR("Content widget could not be found in menu_item.");
        return NULL;
     }

   content_atk_obj = eail_factory_get_accessible(content_widget);

   return content_atk_obj;
}

/**
 * @brief Implementation of AtkObject->ref_child callback
 *
 * ATK doc says:\n
 * Gets a reference to the specified accessible child of the object. The
 * accessible children are 0-based so the first accessible child is at index 0,
 * the second at index 1 and so on.
 *
 * @param parent an EailItemParent
 * @param item an EailItem
 * @param i child index
 *
 * @returns an AtkObject representing the specified accessible child of the
 * accessible.
 */
static AtkObject *
eail_menu_item_ref_child(EailItemParent   *parent,
                         EailItem         *item,
                         gint             i)
{
   const Eina_List *items;
   AtkObject *child = NULL;
   Elm_Object_Item *elm_obj_item = NULL;

   items = eail_menu_item_get_items(item);
   if (eina_list_count(items) > i)
     {
        elm_obj_item = eina_list_nth(items, i);

        /* creating new menu item from Elm_Object_Item */
        child = eail_factory_get_item_atk_obj
                         (elm_obj_item, ATK_ROLE_LIST_ITEM, ATK_OBJECT(parent));
     }
   else if (eina_list_count(items) == i)
     {
        /* ref to widget inside menu item has to be referenced */
        child = _eail_menu_item_create_content_atk_obj(ATK_OBJECT(item));
     }

   if (child)
     g_object_ref(child);
   else
     ERR("Could not ref menu item child for index %d", i);

   return child;
}

/**
 * @brief Implementation of EailItemParent->is_content_get_supported callback
 *
 * @param parent an EailItemParent
 * @param item an EailItem
 * @returns FALSE
 */
gboolean
eail_menu_item_content_get_support(EailItemParent   *parent,
                                   EailItem         *item)
{
   return FALSE;
}

/**
 * @brief Initializes EailItemParentIface callbacks
 *
 * @param iface an EailItemParent interface
 */
static void
eail_item_parent_interface_init(EailItemParentIface *iface)
{
   iface->get_evas_obj = eail_menu_item_get_evas_obj;
   iface->get_item_name = eail_menu_item_name_get;
   iface->get_n_children = eail_menu_item_get_n_children;
   iface->ref_n_child = eail_menu_item_ref_child;
   iface->get_actions_supported = eail_menu_get_actions_supported;
   iface->is_content_get_supported = eail_menu_item_content_get_support;
}
