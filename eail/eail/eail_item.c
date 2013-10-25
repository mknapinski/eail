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
 * @file eail_item.c
 * @brief EailItem implementation
 *
 * In general - this object represents all kinds of objects that have form of
 * a Elm_Obj_Item* and need ATK accessible representation. It stores
 * Elm_Obj_Item* inside - unlike Evas_Object like those components that are
 * extending EailWidget.
*/

#include "eail_item.h"
#include "eail_item_parent.h"
#include "eail_factory.h"
#include "eail_utils.h"
#include "eail_priv.h"

static void atk_component_interface_init(AtkComponentIface *iface);
static void atk_action_interface_init(AtkActionIface *iface);
static void atk_text_interface_init(AtkTextIface *iface);

#define EAIL_ITEM_CLICK_NAME "click" /**< @brief 'click' action name*/
#define EAIL_ITEM_PRESS_NAME "press" /**< @brief 'press' action name*/
#define EAIL_ITEM_RELEASE_NAME "release" /**< @brief 'release' action name*/
#define EAIL_ITEM_PART_FIRST "start" /**< @brief 'start' action name*/
#define EAIL_ITEM_PART_SECOND "end" /**< @brief 'end' action name*/
#define EAIL_ITEM_PART_ICON "elm.swallow.icon" /**< @brief icon item part*/
#define EAIL_ITEM_PART_END "elm.swallow.end" /**< @brief end item part*/
#define EAIL_TXT_SEPARATOR " " /**< @brief separator for text content*/

/**
 * @brief EailItem GObject definition
 *
 * It extends ATK_TYPE_OBJECT class and implements ATK_TYPE_COMPONENT,
 * ATK_TYPE_TEXT and ATK_TYPE_ACTION interfaces
 */
G_DEFINE_TYPE_WITH_CODE(EailItem,
                        eail_item,
                        ATK_TYPE_OBJECT,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_COMPONENT,
                                              atk_component_interface_init)
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_ACTION,
                                              atk_action_interface_init)
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_TEXT,
                                              atk_text_interface_init));

/**
 * @brief Initializer for AtkObjectClass
 *
 * @param obj AtkObject instance
 * @param data additional initialization data
 */
static void
eail_item_initialize(AtkObject *obj, gpointer data)
{
   EailItem *item = EAIL_ITEM(obj);

   ATK_OBJECT_CLASS(eail_item_parent_class)->initialize(obj, data);

   item->item = (Elm_Object_Item *)data;

   /* NOTE: initializing role is being done only in eail_item_new(..) */
}

/**
 *
 * @param item EailItem to take nested Elm_Object_Item from
 *
 * @returns Elm_Object_Item * representing nested item or NULL if an error occured
 */
Elm_Object_Item *
eail_item_get_item(EailItem *item)
{
   g_return_val_if_fail(EAIL_IS_ITEM(item), NULL);

   return item->item;
}

/*
 * Implementation of the *AtkObject* interface
 */

/**
 * @brief Gets the accessible name of the accessible.
 *
 * Implementation of AtkObject->get_name callback.
 *
 * @param obj AtkObject for EailItem
 * @returns string representing the accessible description of
 * the accessible.
 */
static const gchar *
eail_item_get_name(AtkObject *obj)
{
   const gchar *atk_name_default = NULL;
   AtkObject *parent = atk_object_get_parent(obj);

   if (!parent) return NULL;

   /* returning name from default atk implementation when it available (it
    * means that it was set by user)*/
   atk_name_default = ATK_OBJECT_CLASS(eail_item_parent_class)->get_name(obj);
   if (atk_name_default) return atk_name_default;

   return eail_item_parent_get_item_name(EAIL_ITEM_PARENT(parent),
                                         EAIL_ITEM(obj));
}

/**
 * @brief Gets the role of the accessible
 *
 * Implementation of AtkObject->get_role callback.
 *
 * @param obj AtkObject for EailItem
 * @returns AtkRole representing the parameter's role
 */
static AtkRole
eail_item_get_role(AtkObject *obj)
{
   AtkObject *parent = atk_object_get_parent(obj);

   if (!parent) return ATK_ROLE_INVALID;

   return eail_item_parent_get_item_role(EAIL_ITEM_PARENT(parent),
                                         EAIL_ITEM(obj));
}

/**
 * @brief Gets the 0-based index of this accessible in its parent.
 *
 * Implementation of AtkObject->get_index_in_parent callback.
 *
 * @param obj AtkObject for EailItem
 * @returns integer representing the index of the accessible in its parent or -1 if the accessible does not have an accessible parent
 */
static gint
eail_item_get_index_in_parent(AtkObject *obj)
{
   AtkObject *parent = atk_object_get_parent(obj);

   if (!parent) return -1;

   return eail_item_parent_get_item_index_in_parent(EAIL_ITEM_PARENT(parent),
                                                    EAIL_ITEM(obj));
}

/**
 * @brief Gets a reference to the state set of the accessible.
 *
 * The caller must unreference it when it is no longer needed.
 *
 * Implementation of AtkObject->ref_state_set callback.
 *
 * @param obj AtkObject for EailItem
 * @returns AtkStateSet containing a reference to the state set of the
 * accessible
 */
static AtkStateSet *
eail_item_ref_state_set(AtkObject *obj)
{
   AtkStateSet *state_set;
   AtkObject *parent = atk_object_get_parent(obj);
   EailItem *item = EAIL_ITEM(obj);
   Elm_Object_Item *it = eail_item_get_item(item);

   state_set = ATK_OBJECT_CLASS(eail_item_parent_class)->ref_state_set(obj);

   if (!parent || !it)
     {
        atk_state_set_add_state(state_set, ATK_STATE_DEFUNCT);
        return state_set;
     }

   if (!elm_object_item_disabled_get(it))
     {
        atk_state_set_add_state(state_set, ATK_STATE_SENSITIVE);
        atk_state_set_add_state(state_set, ATK_STATE_ENABLED);
     }

   return eail_item_parent_ref_item_state_set(EAIL_ITEM_PARENT(parent),
                                              item, state_set);
}


/**
 * @brief Initializer for GObject class
 * @param item EailItem instance
 */
static void
eail_item_init(EailItem *item)
{
}

/**
 * @brief Finalizes implementation for GObject class
 * @param obj AtkObject for EailItem instance that needs to be finalized
 */
static void
eail_item_class_finalize(GObject *obj)
{
   EailItem *eail_item = EAIL_ITEM(obj);
   Elm_Object_Item *obj_item = eail_item_get_item(eail_item);

   if (obj_item)
     eail_factory_unregister_item_from_cache(obj_item);

   G_OBJECT_CLASS(eail_item_parent_class)->finalize(obj);
}

/**
 * @brief Helper function used for adding Evas_Object* content to the item's list
 * if the content is available.
 *
 * @param eail_obj_item item used for browsing for given content part
 * @param items list of items that will be extended by the content part
 * @param part_str name of content part to be found
 *
 * @returns Eina_List filled with Evas_Object* objects
 */
static Eina_List *
_eail_item_append_part_if_exist(Elm_Object_Item *eail_obj_item,
                                Eina_List *items,
                                gchar *part_str)
{
   Evas_Object *content = NULL;

   content = elm_object_item_part_content_get
                                         (eail_obj_item, part_str);
   if (content)
     items = eina_list_append(items, content);

   return items;
}

/**
 * @brief Checks if content_get is supported by the given EailItem object
 *
 * @param atk_object AtkObject instance
 *
 * @returns TRUE if content_get is supported, FALSE otherwise
 */
static gboolean
_eail_item_is_content_get_supported(AtkObject *atk_object)
{
   AtkObject *parent = NULL;


   parent = atk_object_get_parent(atk_object);
   g_return_val_if_fail(parent, FALSE);

   return eail_item_parent_is_is_content_get_supported
                              (EAIL_ITEM_PARENT(parent),EAIL_ITEM(atk_object));
}

/**
 * @brief Gets list of item's content parts with well documented content strings
 *
 * @param eail_item item to get content from
 *
 * @returns Eina_List filled with the content parts that have been found
 */
static Eina_List *
_eail_item_get_basic_parts(EailItem *eail_item)
{
   Eina_List *items = NULL;
   Elm_Object_Item *obj_item = NULL;

   /* if content_get is not supported then content from basic parts will be
    * always empty. This checking is being done, because elementary library
    * don't want to ask for some widget usign content_get API */
   if (!_eail_item_is_content_get_supported(ATK_OBJECT(eail_item)))
     return items;

   obj_item = eail_item_get_item(EAIL_ITEM(eail_item));
   items = _eail_item_append_part_if_exist
                                       (obj_item, items, EAIL_ITEM_PART_FIRST);
   items = _eail_item_append_part_if_exist
                                       (obj_item, items, EAIL_ITEM_PART_SECOND);
   items = _eail_item_append_part_if_exist
                                       (obj_item, items, EAIL_ITEM_PART_ICON);
   items = _eail_item_append_part_if_exist
                                       (obj_item, items, EAIL_ITEM_PART_END);

   return items;
}

/**
 * @brief Helper function for adding unique entries from one list to another
 *
 * @param item_list list which unique items will be appended to
 * @param additional_items source list with items to be added to item_list
 *
 * @returns Eina_List filled with unique entries from both lists
 */
static Eina_List *
_eail_add_unique_listsparts(Eina_List *item_list,
                            Eina_List *additional_items)
{
   Eina_List *l = NULL;
   Elm_Object_Item *item_edj;

   EINA_LIST_FOREACH(additional_items, l, item_edj)
    {
      if (!eina_list_data_find(item_list, item_edj))
        item_list = eina_list_append(item_list, item_edj);
    }

   return item_list;
}

/**
 * @brief Gets content part items from the given EailItem object
 *
 * List should be freed when results will be processed.
 *
 * @param eail_item EailItem object used to get content from
 *
 * @returns Eina_List containing content parts for the given item.
 */
static Eina_List *
_eail_item_get_part_items(EailItem *eail_item)
{
   Eina_List *items = NULL, *edje_items = NULL;
   Elm_Object_Item *obj_item = NULL;

   obj_item = eail_item_get_item(eail_item);
   g_return_val_if_fail(obj_item, NULL);

   /* parts from well documented default content parts - there are being used
    * mostly by desktop applications. Some of these parts are not listed in
    * edje objects (used below), so results from both list need to be merged
    * to list full item content */
   items = _eail_item_get_basic_parts(eail_item);

   /* content parts from taken from edje object*/
   edje_items = eail_get_edje_parts_for_item(obj_item);

   /* adding unique edje items to content list*/
   items = _eail_add_unique_listsparts(items, edje_items);

   eina_list_free(edje_items);

   return items;
}

/**
 * @brief Gets the number of accessible children of the accessible.
 *
 * Implementation AtkObject->get_n_children callback.
 *
 * @param obj AtkObject (EailItem) instance
 *
 * @returns integer representing the number of accessible children of
 * the accessible
 */
static gint
eail_item_get_n_children(AtkObject *obj)
{
   gint n_items, parent_n_items;
   Eina_List *items;
   AtkObject *parent = atk_object_get_parent(obj);

   if (!parent) return 0;
   parent_n_items = eail_item_parent_get_n_children(EAIL_ITEM_PARENT(parent),
                                                    EAIL_ITEM(obj));
   /* if there is item parent impl, then returning item count
    * from it*/
   if (parent_n_items != -1) return parent_n_items;

   items = _eail_item_get_part_items(EAIL_ITEM(obj));
   n_items = eina_list_count(items);

   eina_list_free(items);

   return n_items;
}

/**
 * @brief Gets a reference to the specified accessible child of the object.
 *
 * The accessible children are 0-based so the first accessible child is at index 0,
 * the second at index 1 and so on.
 *
 * Implementation of AtkObject->ref_child callback.
 *
 * @param obj AtkObject for EailItem instance
 * @param i index of item to reference
 *
 * @returns AtkObject representing the specified accessible child of the
 * accessible
 */
static AtkObject *
eail_item_ref_child(AtkObject *obj, gint i)
{
   Eina_List *items;
   AtkObject *child = NULL;
   AtkObject *parent = atk_object_get_parent(obj);
   AtkObject *ref_child_from_parent = NULL;

   if (!parent) return NULL;

   ref_child_from_parent = eail_item_parent_ref_n_child
                              (EAIL_ITEM_PARENT(parent), EAIL_ITEM(obj), i);

   /* if there is item parent implementation for ref child, then returning
    * object using that parent implementation*/
   if (ref_child_from_parent)
      {
        g_object_ref(ref_child_from_parent);
        return ref_child_from_parent;
      }

   items = _eail_item_get_part_items(EAIL_ITEM(obj));
   if (eina_list_count(items) > i)
     child = eail_factory_get_accessible(eina_list_nth(items, i));
   else
      ERR("Tried to ref child with index %d out of bounds!", i);

   eina_list_free(items);

   if (child)
     g_object_ref(child);
   else
     DBG("Child could not created in factory");

   return child;
}

/**
 * @brief Initializer for GObject class
 *
 * Defines callbacks for base AtkObject.
 *
 * @param klass EailItemClass instance
 */
static void
eail_item_class_init(EailItemClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
   GObjectClass *g_object_class = G_OBJECT_CLASS(klass);

   class->initialize = eail_item_initialize;
   class->get_name = eail_item_get_name;
   class->get_role = eail_item_get_role;
   class->get_index_in_parent = eail_item_get_index_in_parent;
   class->ref_state_set = eail_item_ref_state_set;
   class->get_n_children = eail_item_get_n_children;
   class->ref_child = eail_item_ref_child;

   g_object_class->finalize = eail_item_class_finalize;
}

/*
 * Implementation of the *AtkComponent* interface
 */

/**
 * @brief Grabs focus for this component.
 *
 * Implementation of AtkComponent->grab_focus callback.
 *
 * @param component AtkComponent (EailItem) instance
 * @returns TRUE on success, FALSE otherwise.
 */
static gboolean
eail_item_grab_focus(AtkComponent *component)
{
   AtkObject *obj = ATK_OBJECT(component);
   AtkObject *parent = atk_object_get_parent(obj);

   if (!parent) return FALSE;

   return eail_item_parent_grab_item_focus(EAIL_ITEM_PARENT(parent),
                                           EAIL_ITEM(obj));
}

/**
 * @brief Gets the rectangle which gives the extent of the component.
 *
 * Implementation of AtkComponent->get_extents callback.
 *
 * @param component AtkComponent instance
 * @param [out] x rectangle upper left x coordinate
 * @param [out] y rectangle upper left y coordinate
 * @param [out] width width of the rectangle
 * @param [out] height height of the rectangle
 * @param coord_type specifies whether the coordinates are relative to the
 * screen or to the components top level window
 */
static void
eail_item_get_extents(AtkComponent    *component,
                      gint            *x,
                      gint            *y,
                      gint            *width,
                      gint            *height,
                      AtkCoordType     coord_type)
{
   AtkObject *obj = ATK_OBJECT(component);
   AtkObject *parent = atk_object_get_parent(obj);

   *x = *y = G_MININT;
   *width = *height = -1;

   if (!parent) return;

   eail_item_parent_get_item_extents(EAIL_ITEM_PARENT(parent),
                                     EAIL_ITEM(obj),
                                     x, y, width, height, coord_type);
}

/**
 * @brief AtkComponent interface initialization
 *
 * @param iface EailNaviframPage instance
 */
static void
atk_component_interface_init(AtkComponentIface *iface)
{
   iface->grab_focus  = eail_item_grab_focus;
   iface->get_extents = eail_item_get_extents;
}

/*
 * AtkAction interface init
 */

/**
 * @brief Gets actions supported by given EailItem object
 *
 * @param action AtkAction instance
 *
 * @returns EailActionSupported enum entry that shows what kind of actions are
 * supported
 */
static gint
_eail_item_get_actions_supported(AtkAction *action)
{
   AtkObject *parent = NULL;

   parent = atk_object_get_parent(ATK_OBJECT(action));
   g_return_val_if_fail(parent, FALSE);

   return eail_item_parent_get_actions_supported
                                 (EAIL_ITEM_PARENT(parent),EAIL_ITEM(action));

}

/**
 * @brief Returns the number of implemented ATK actions.
 *
 * Implementation of AtkActionIface get_n_actions callback.
 *
 * @param action object that implements AtkAction interface
 * @returns integer representing number of implemented actions
 */
static int
eail_item_n_actions_get(AtkAction *action)
{
   int actions_num = 0;
   /* if parent item does not support click action, then return no action */
   if (_eail_item_get_actions_supported(action) == EAIL_ACTION_SUPPORTED_NONE)
     return 0;

   if (_eail_item_get_actions_supported(action) & EAIL_ACTION_SUPPORTED_CLICK)
     actions_num++;

   if (_eail_item_get_actions_supported(action) & EAIL_ACTION_SUPPORTED_PRESS)
     actions_num++;

   if (_eail_item_get_actions_supported(action) & EAIL_ACTION_SUPPORTED_RELEASE)
     actions_num++;

   return actions_num;
}

/**
 * @brief Returns the accessible name of the specified action
 *
 * @param action object that implements AtkAction interface
 * @param i index (number) of action
 * @returns string containing the accessible name of the specified action
 */
static const char*
eail_item_action_name_get(AtkAction *action, int i)
{
   const char* action_name;

   /* if parent item does not support click action, then return no action */
   if (_eail_item_get_actions_supported(action) == EAIL_ACTION_SUPPORTED_NONE)
     return NULL;

   switch (i)
     {
      case 0:
         /*"click": the user clicked the item*/
         action_name = EAIL_ITEM_CLICK_NAME;
         break;
      case 1:
         /*"press": the user pressed the item*/
         action_name = EAIL_ITEM_PRESS_NAME;
         break;
      case 2:
         /*"release": the user pressed the item*/
         action_name = EAIL_ITEM_RELEASE_NAME;
         break;
      default:
         action_name = NULL;
         break;
     }

   return action_name;
}

/**
 * @brief Gets the clickable Evas_Object for the given EailItem
 *
 * @param atk_item EailItem instance
 *
 * @returns clickable Evas_Object or NULL of no clickable content was found
 */
static Evas_Object *
_eail_item_get_clickable_evas_obj(AtkObject *atk_item)
{
   AtkObject *parent = NULL;
   Evas_Object *widget = NULL;
   Eina_List *parts = NULL;

   parent = atk_object_get_parent(atk_item);
   g_return_val_if_fail(parent, FALSE);

   widget = eail_item_parent_get_evas_obj
                           (EAIL_ITEM_PARENT(parent), EAIL_ITEM(atk_item));
   if (widget) return widget;

   /* if not supported from parent, then trying to get content widget nested
    * in item */
   parts = _eail_item_get_part_items(EAIL_ITEM(atk_item));
   if (parts && eina_list_count(parts) > 0)
     {
        /* getting first widget from content */
        widget = eina_list_nth(parts, 0);
     }
   eina_list_free(parts);
   if (widget) return widget;

   /* if no nested widget, then getting evas clickable area */
   return eail_get_edje_obj_from_item
                           (eail_item_get_item(EAIL_ITEM(atk_item)));
}


/**
 * @brief Performs an action with the given name on given item
 *
 * @param atk_item item object to perform the action on
 * @param action_name name of the action (eg. 'click', 'press'...)
 *
 * @returns TRUE if operation was successful, FALSE otherwise
 */
static gboolean
_eail_item_perform_action(AtkObject *atk_item, const gchar *action_name)
{
   AtkObject *parent = NULL;
   Evas_Object *widget = NULL;
   int x, y;

   parent = atk_object_get_parent(atk_item);
   g_return_val_if_fail(parent, FALSE);

   widget =_eail_item_get_clickable_evas_obj(atk_item);

   if (!widget)
     {
        ERR("No widget for click found");
        return FALSE;
     }

   /* getting coordinates of center of the widget to make sure, that
    * click will be performed on active widget area */
   eail_get_coords_widget_center(widget, &x, &y);

   if (0 == g_strcmp0(action_name, EAIL_ITEM_CLICK_NAME))
     {
       DBG("Calling 'click' on item");
       eail_mouse_click_on_coords(widget, x, y);
     }
   else if (0 == g_strcmp0(action_name, EAIL_ITEM_PRESS_NAME))
     {
        DBG("Calling 'press' on item");
        eail_mouse_press_on_coords(widget, x, y);
     }
   else if (0 == g_strcmp0(action_name, EAIL_ITEM_RELEASE_NAME))
     {
        DBG("Calling 'release' on item");
        eail_mouse_release_on_coords(widget, x, y);
     }
   else
     {
        DBG("Action name not found: %s", action_name);
        return FALSE;
     }

   return TRUE;
}

/**
 * \brief Launches action with given index
 *
 * @param action AtkAction instance
 * @param i index (number) of the action
 *
 * @returns TRUE if action was successfully launched, FALSE otherwise
 */
static gboolean
eail_item_do_action(AtkAction *action, int i)
{
   const char *action_name = atk_action_get_name(action, i);

   /* if parent item does not support click action, then return immediately */
   if (_eail_item_get_actions_supported(action) == EAIL_ACTION_SUPPORTED_NONE)
     return FALSE;

   if (action_name == NULL) return FALSE;

   return _eail_item_perform_action(ATK_OBJECT(action), action_name);;
}

/**
 * @brief Initializer for AtkActionIface
 * @param iface AtkActionIface instance to fill
 */
static void
atk_action_interface_init(AtkActionIface *iface)
{
   g_return_if_fail(iface != NULL);

   iface->do_action     = eail_item_do_action;
   iface->get_n_actions = eail_item_n_actions_get;
   iface->get_name      = eail_item_action_name_get;
}

/**
 * @brief Gets text content from item
 *
 * Implementation of AtkTextIface->get_text callback.
 *
 * Use g_free() to free the returned.
 *
 * @param text AtkText instance
 * @param start_offset start position
 * @param end_offset end position, or -1 for the end of the string.
 *
 * @returns a newly allocated string containing the text from start_offset
 * up to, but not including end_offset.
 * string.
 */
static gchar*
eail_item_get_text(AtkText   *text,
                   gint       start_offset,
                   gint       end_offset)
{
   Eina_Strbuf *buf = NULL;
   Elm_Object_Item *obj_item = NULL;
   Eina_List *string_parts = NULL, *l = NULL;
   gchar *string_part = NULL;
   gchar *ret_str = NULL;
   gboolean first_part = TRUE;

   obj_item = eail_item_get_item(EAIL_ITEM(text));
   g_return_val_if_fail(obj_item, NULL);

   string_parts = eail_item_get_content_strings(obj_item);
   if (!string_parts) return NULL;

   buf = eina_strbuf_new();
   EINA_LIST_FOREACH(string_parts, l, string_part)
    {
      if (!first_part)
        eina_strbuf_append(buf, EAIL_TXT_SEPARATOR);

      eina_strbuf_append(buf, string_part);
      first_part = FALSE;
    }

   /* ret_str is newly allocated */
   ret_str = eail_get_substring
                     (eina_strbuf_string_get(buf), start_offset, end_offset);

   eina_list_free(string_parts);
   eina_strbuf_free(buf);

   return ret_str;
}

/**
 * @brief Gets character from an item at the given offset
 *
 * Implementation of AtkTextIface->get_character_at_offset callback.
 *
 * @param text AtkText instance
 * @param offset offset to get the character from
 *
 * @returns char located at offset
 */
static gunichar
eail_item_get_character_at_offset(AtkText *text, gint offset)
{
   gunichar character = '\0';
   gchar* time_str = NULL;

   time_str = eail_item_get_text(text, 0, -1);

   if (time_str)
     {
         character = g_utf8_get_char
                              (g_utf8_offset_to_pointer(time_str, offset));
         g_free(time_str);
     }

   return character;
}

/**
 * @brief Gets the character count from text content in item
 *
 * Implementation of AtkTextIface->get_character_count callback.
 *
 * @param text AtkText instance
 *
 * @returns integer representing the character count
 */
static gint
eail_item_get_character_count(AtkText *text)
{
   gint count = 0;
   gchar* str = NULL;

   str = eail_item_get_text(text, 0, -1);

   if (str)
      {
         count = g_utf8_strlen(str, -1);
         g_free(str);
      }

   return count;
}

/**
 * @brief Initializer for AtkTextinterface
 * @param iface AtkTextIface instance to fill
 */
static void
atk_text_interface_init(AtkTextIface *iface)
{
   iface->get_text = eail_item_get_text;
   iface->get_character_at_offset = eail_item_get_character_at_offset;
   iface->get_character_count = eail_item_get_character_count;
}
