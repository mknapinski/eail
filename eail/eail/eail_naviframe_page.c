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
 * @file eail_naviframe_page.c
 * @brief EailNaviframePage implementation
 */

#include "eail_naviframe_page.h"
#include "eail_naviframe.h"
#include "eail_factory.h"
#include "eail_widget.h"
#include "eail_priv.h"

static void atk_component_interface_init(AtkComponentIface *iface);

/**
 * Defines EailNaviframePage type
 */
G_DEFINE_TYPE_WITH_CODE(EailNaviframePage,
                        eail_naviframe_page,
                        ATK_TYPE_OBJECT,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_COMPONENT,
                                              atk_component_interface_init));

/**
 * @brief Creates a new instance of an accessible naviframe page
 *
 * @param naviframe AtkObject instance
 * @param navi_tab_item Elm_Object_Item instance
 * @returns AtkObject representing the accessible naviframe page
 */
AtkObject *
eail_naviframe_page_new(AtkObject *naviframe, Elm_Object_Item *navi_tab_item)
{
   AtkObject *atk_object;
   EailNaviframePage *page;
   Evas_Object *o, *widget;

   g_return_val_if_fail(EAIL_IS_NAVIFRAME(naviframe), NULL);

   widget = eail_widget_get_widget(EAIL_WIDGET(naviframe));

   g_return_val_if_fail(widget != NULL, NULL);

   atk_object = g_object_new(EAIL_TYPE_NAVIFRAME_PAGE, NULL);
   page = EAIL_NAVIFRAME_PAGE(atk_object);

   page->naviframe = widget;

   page->page = navi_tab_item;

   page->child_count = 0;

   o = elm_object_item_part_content_get(page->page, "prev_btn");
   if (o && elm_object_widget_check(o))
     page->content[page->child_count++] = o;

   o = elm_object_item_part_content_get(page->page, "icon");
   if (o && elm_object_widget_check(o))
     page->content[page->child_count++] = o;

   o = elm_object_item_part_content_get(page->page, "next_btn");
   if (o && elm_object_widget_check(o))
     page->content[page->child_count++] = o;

   o = elm_object_item_part_content_get(page->page, "default");
   if (o && elm_object_widget_check(o))
     page->content[page->child_count++] = o;

   page->parent_naviframe = naviframe;

   atk_object->layer = ATK_LAYER_WIDGET;

   return atk_object;
}

/**
 * @brief Returns the accessible name if assigned, title or subtitle otherwise
 *
 * Implementation of atk_object_get_name from AtkObject.
 *
 * @param obj AtkObject instance
 * @returns string representing the accessible name if assigned, title or subtitle otherwise
 */
static const char *
eail_naviframe_page_name_get(AtkObject *obj)
{
   const gchar *atk_name = NULL;

   g_return_val_if_fail(EAIL_IS_NAVIFRAME_PAGE(obj), NULL);

   /* returning name from ATK default implementation if available */
   atk_name = ATK_OBJECT_CLASS(eail_naviframe_page_parent_class)->get_name(obj);
   if (atk_name) return atk_name;

   EailNaviframePage *page = EAIL_NAVIFRAME_PAGE(obj);
   if (page->name != NULL) return page->name;

   const char *title = elm_object_item_part_text_get(page->page, "default");
   const char *subtitle = elm_object_item_part_text_get(page->page, "subtitle");

   if (subtitle != NULL)
     page->name = eina_stringshare_printf("%s, %s", title, subtitle);
   else
     page->name = eina_stringshare_add(title);

   return page->name;
}

/**
 * @brief Initiates EailNaviframePage
 *
 * @param naviframe_page EailNaviframePage instance
 */
static void
eail_naviframe_page_init(EailNaviframePage *naviframe_page)
{
}

/**
 * @brief Initializes EailNaviframPage
 *
 * @param obj AtkObject instance
 * @param data user data
 */
static void
eail_naviframe_page_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_naviframe_page_parent_class)->initialize(obj, data);

   obj->role = ATK_ROLE_PAGE_TAB;
   obj->layer = ATK_LAYER_WIDGET;
}

/**
 * @brief Finalizes object
 *
 * @param obj AtkObject instance
 */
static void
eail_naviframe_page_finalize(GObject *obj)
{
   EailNaviframePage *page = EAIL_NAVIFRAME_PAGE(obj);

   eina_stringshare_del(page->name);

   if (page->page)
     eail_factory_unregister_item_from_cache(page->page);

   G_OBJECT_CLASS(eail_naviframe_page_parent_class)->finalize(obj);
}

/**
 * @brief Gets the number of accessible children of obj
 *
 * @param obj AtkObject instance
 *
 * @returns integer representing the number of children
 */
static gint
eail_naviframe_page_n_children_get(AtkObject *obj)
{
   EailNaviframePage *page;

   g_return_val_if_fail(EAIL_IS_NAVIFRAME_PAGE(obj), 0);
   page = EAIL_NAVIFRAME_PAGE(obj);

   return page->child_count;
}

/**
 * @brief Gets obj's parent
 *
 * @param obj AtkObject instance
 *
 * @returns AtkObject representing the parent of obj
 */
static AtkObject *
eail_naviframe_page_parent_get(AtkObject *obj)
{
   EailNaviframePage *page;

   g_return_val_if_fail(EAIL_IS_NAVIFRAME_PAGE(obj), NULL);

   page = EAIL_NAVIFRAME_PAGE(obj);

   return ATK_OBJECT(page->parent_naviframe);
}

/**
 * @brief Gets a reference to the specified child of obj
 *
 * @param obj AtkObject instance
 * @param i child index
 *
 * @returns AtkObject representing the specified child
 */
static AtkObject *
eail_naviframe_page_ref_child(AtkObject *obj, gint i)
{
   EailNaviframePage *page;
   AtkObject *atk_obj_child = NULL;

   g_return_val_if_fail(EAIL_IS_NAVIFRAME_PAGE(obj), NULL);

   page = EAIL_NAVIFRAME_PAGE(obj);
   if (i >= page->child_count) return NULL;

   atk_obj_child = eail_factory_get_accessible(page->content[i]);

   g_object_ref(atk_obj_child);
   return atk_obj_child;
}

/**
 * @brief Gets the index of obj in parent object
 *
 * @param obj AtkObject instance
 *
 * @returns integer representing the index of object
 */
static gint
eail_naviframe_page_index_in_parent_get(AtkObject *obj)
{
   EailNaviframePage *page = NULL;
   Eina_List *list = NULL;
   gint pos = -1, i = 0;

   if (!EAIL_IS_NAVIFRAME_PAGE(obj))
     {
        ERR("Not a naviframe page");
        return pos;
     }

   page = EAIL_NAVIFRAME_PAGE(obj);
   if (!page->naviframe) return pos;

   list = elm_naviframe_items_get(page->naviframe);
   for (i = 0; i < eina_list_count(list); ++i)
     {
        if (page->page == eina_list_nth(list, i))
          {
             pos = i;
             break;
          }
     }

   eina_list_free(list);
   return pos;
}

/**
 * @brief Gets the state set of the accessible object
 *
 * @param obj AtkObject instance
 *
 * @returns AtkStateSet representing the state set of the accessible object
 */
static AtkStateSet *
eail_naviframe_page_ref_state_set(AtkObject *obj)
{
   EailNaviframePage *page;
   AtkStateSet *state_set;
   Evas_Object *widget;

   g_return_val_if_fail(EAIL_IS_NAVIFRAME_PAGE(obj), NULL);

   page = EAIL_NAVIFRAME_PAGE(obj);
   if (!page->naviframe) return NULL;

   widget = elm_object_part_content_get(page->naviframe, "default");

   state_set = atk_state_set_new();

   /* only item-page on top is visible*/
   if (elm_naviframe_top_item_get(page->naviframe) == page->page)
     {
        atk_state_set_add_state(state_set, ATK_STATE_SHOWING);
        atk_state_set_add_state(state_set, ATK_STATE_VISIBLE);
     }

   if (!elm_object_disabled_get(widget))
     atk_state_set_add_state(state_set, ATK_STATE_ENABLED);

   return state_set;
}

/**
 * @brief EailNaviframePage class initializer
 *
 * @param klass EailNaviframePageClass instance
 */
static void
eail_naviframe_page_class_init(EailNaviframePageClass *klass)
{
   GObjectClass *g_object_class = G_OBJECT_CLASS(klass);
   AtkObjectClass *atk_class = ATK_OBJECT_CLASS(klass);

   atk_class->initialize = eail_naviframe_page_initialize;
   atk_class->get_name = eail_naviframe_page_name_get;
   atk_class->get_parent = eail_naviframe_page_parent_get;
   atk_class->get_n_children = eail_naviframe_page_n_children_get;
   atk_class->ref_child = eail_naviframe_page_ref_child;
   atk_class->ref_state_set = eail_naviframe_page_ref_state_set;
   atk_class->get_index_in_parent = eail_naviframe_page_index_in_parent_get;

   g_object_class->finalize = eail_naviframe_page_finalize;
}

/**
 * @brief Gets the rectangle which gives the extent of the component
 *
 * Implementation of get_extents from AtkComponent interface.
 *
 * @param component AtkComponent instance
 * @param [out] x x coordinate
 * @param [out] y y coordinate
 * @param [out] width width of the rectangle
 * @param [out] height height of the rectangle
 * @param coord_type specifies whether the coordinates are relative to the screen or to the component's top level window
 */
static void
eail_naviframe_page_get_extents(AtkComponent *component,
                                gint *x,
                                gint *y,
                                gint *width,
                                gint *height,
                                AtkCoordType coord_type)
{
   EailNaviframePage *page;
   Evas_Object *widget;
   g_return_if_fail(EAIL_IS_NAVIFRAME_PAGE(component));

   page = EAIL_NAVIFRAME_PAGE(component);
   *x = *y = *width = *height = G_MININT;
   if (!page->naviframe) return;

   widget = page->naviframe;

   evas_object_geometry_get(widget, x, y, width, height);
   if (coord_type == ATK_XY_SCREEN)
     {
        int ee_x, ee_y;

        Ecore_Evas *ee = ecore_evas_ecore_evas_get(
           evas_object_evas_get(widget));
        ecore_evas_geometry_get(ee, &ee_x, &ee_y, NULL, NULL);
        *x += ee_x;
        *y += ee_y;
     }
}

/**
 * @brief AtkComponent interface initialization
 *
 * @param iface AtkComponentIface instance
 */
static void atk_component_interface_init(AtkComponentIface *iface)
{
   g_return_if_fail(iface != NULL);

   iface->get_extents = eail_naviframe_page_get_extents;
}
