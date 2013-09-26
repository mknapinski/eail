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
 * @file eail_factory.c
 * @brief Implementation of factory of EAIL accessible widgets
 */

#include <Elementary.h>

#include "eail_factory.h"
#include "eail_widget.h"
#include "eail_window.h"
#include "eail_background.h"
#include "eail_box.h"
#include "eail_list.h"
#include "eail_genlist.h"
#include "eail_label.h"
#include "eail_button.h"
#include "eail_icon.h"
#include "eail_entry.h"
#include "eail_frame.h"
#include "eail_scroller.h"
#include "eail_slider.h"
#include "eail_action_slider.h"
#include "eail_inwin.h"
#include "eail_image.h"
#include "eail_check.h"
#include "eail_radio_button.h"
#include "eail_menu.h"
#include "eail_photo.h"
#include "eail_photocam.h"
#include "eail_separator.h"
#include "eail_spinner.h"
#include "eail_clock.h"
#include "eail_calendar.h"
#include "eail_grid.h"
#include "eail_route.h"
#include "eail_dayselector.h"
#include "eail_gengrid.h"
#include "eail_progressbar.h"
#include "eail_diskselector.h"
#include "eail_segment_control.h"
#include "eail_conformant.h"
#include "eail_colorselector.h"
#include "eail_notify.h"
#include "eail_naviframe.h"
#include "eail_datetime.h"
#include "eail_hover.h"
#include "eail_popup.h"
#include "eail_ctxpopup.h"
#include "eail_toolbar.h"
#include "eail_multibuttonentry.h"
#include "eail_web.h"
#include "eail_index.h"
#include "eail_fileselector.h"
#include "eail_fileselector_entry.h"
#include "eail_panes.h"
#include "eail_panel.h"
#include "eail_thumb.h"
#include "eail_mapbuf.h"
#include "eail_slideshow.h"
#include "eail_hoversel.h"
#include "eail_map.h"
#include "eail_glview.h"
#include "eail_bubble.h"
#include "eail_plug.h"
#include "eail_video.h"
#include "eail_flip.h"
#include "eail_flipselector.h"
#include "eail_layout.h"
#include "eail_table.h"
#include "eail_prefs.h"
#include "eail_item.h"
#include "eail_item_parent.h"
#include "eail_priv.h"

/** @brief Cache of created AtkObjects for re-using by multiple ATK clients. It holds
 * EailFactoryObj* objects inside */
static Eina_List *eail_cached_objs = NULL;

/**
 * @brief Creates an EailFactoryObj instance for the given widget and adds it to cache
 *
 * @param atk_obj AtkObject instance
 * @param evas_obj Evas_Object instance
 */
static void
_eail_factory_append_wdgt_to_cache(AtkObject *atk_obj, Evas_Object *evas_obj)
{
   EailFactoryObj *factory_obj = g_new0(EailFactoryObj, 1);
   factory_obj->atk_obj = atk_obj;
   factory_obj->evas_obj = evas_obj;

   eail_cached_objs = eina_list_append(eail_cached_objs, factory_obj);
}

/**
 * @brief Tries to find an AtkObject representing the given Evas_Object* in cache
 *
 * @param widget Evas_Object instance to find in cache
 *
 * @returns EailFactoryObj representing the given Evas_Object
 * or NULL if the widget's representation was not found
 */
static EailFactoryObj *
_eail_factory_find_obj_for_widget(const Evas_Object *widget)
{
   const Eina_List *l = NULL;
   EailFactoryObj *factory_obj = NULL;

   EINA_LIST_FOREACH(eail_cached_objs, l, factory_obj)
     {
        if (widget == factory_obj->evas_obj)
          {
             return factory_obj;
          }
     }

   /* no widget implementation in cache was found */
   return NULL;
}

/**
 * @brief Creates an accessible AtkObject for Evas_Object*
 *
 * Returned AtkObject should be unreferenced when no longer needed to free memory.
 *
 * @param widget Evas_Object* instance
 *
 * @returns AtkObject representing the accessible representation of the specified
 * Evas_Object
 */
static AtkObject *
_eail_factory_create_accessible(Evas_Object *widget)
{
   const char *type = NULL;
   AtkObject *accessible = NULL;

   type = elm_object_widget_type_get(widget);

   if (!strcmp(type, "elm_win"))
     {
        accessible = g_object_new(EAIL_TYPE_WINDOW, NULL);
     }
   else if (!strcmp(type, "elm_bg"))
     {
        accessible = g_object_new(EAIL_TYPE_BACKGROUND, NULL);
     }
   else if (!strcmp(type, "elm_box"))
     {
        accessible = g_object_new(EAIL_TYPE_BOX, NULL);
     }
   else if (!strcmp(type, "elm_list"))
     {
        accessible = g_object_new(EAIL_TYPE_LIST, NULL);
     }
   else if (!strcmp(type, "elm_genlist"))
     {
        accessible = g_object_new(EAIL_TYPE_GENLIST, NULL);
     }
   else if (!strcmp(type, "elm_label"))
     {
        accessible = g_object_new(EAIL_TYPE_LABEL, NULL);
     }
   else if (!strcmp(type, "elm_button"))
     {
        accessible = g_object_new(EAIL_TYPE_BUTTON, NULL);
     }
   else if (!strcmp(type, "elm_icon"))
     {
        accessible = g_object_new(EAIL_TYPE_ICON, NULL);
     }
   else if ((!strcmp(type, "entry")) || (!strcmp(type, "elm_entry")))
     {
        accessible = g_object_new(EAIL_TYPE_ENTRY, NULL);
     }
   else if (!strcmp(type, "elm_frame"))
     {
        accessible = g_object_new(EAIL_TYPE_FRAME, NULL);
     }
   else if (!strcmp(type, "elm_scroller"))
     {
        accessible = g_object_new(EAIL_TYPE_SCROLLER, NULL);
     }
   else if (!strcmp(type, "elm_inwin"))
     {
        accessible = g_object_new(EAIL_TYPE_INWIN, NULL);
     }
   else if (!strcmp(type, "elm_slider"))
     {
        accessible = g_object_new(EAIL_TYPE_SLIDER, NULL);
     }
   else if (!strcmp(type, "elm_actionslider"))
     {
        accessible = g_object_new(EAIL_TYPE_ACTION_SLIDER, NULL);
     }
   else if (!strcmp(type, "elm_image"))
     {
        accessible = g_object_new(EAIL_TYPE_IMAGE, NULL);
     }
   else if (!strcmp(type, "elm_check"))
     {
        accessible = g_object_new(EAIL_TYPE_CHECK, NULL);
     }
   else if (!strcmp(type, "elm_radio"))
     {
        accessible = g_object_new(EAIL_TYPE_RADIO_BUTTON, NULL);
     }
   else if (!strcmp(type, "elm_menu"))
     {
        accessible = g_object_new(EAIL_TYPE_MENU, NULL);
     }
   else if (!strcmp(type, "elm_photo"))
     {
        accessible = g_object_new(EAIL_TYPE_PHOTO, NULL);
     }
   else if (!strcmp(type, "elm_photocam"))
     {
        accessible = g_object_new(EAIL_TYPE_PHOTOCAM, NULL);
     }
   else if (!strcmp(type, "elm_separator"))
     {
        accessible = g_object_new(EAIL_TYPE_SEPARATOR, NULL);
     }
   else if (!strcmp(type, "elm_spinner"))
     {
        accessible = g_object_new(EAIL_TYPE_SPINNER, NULL);
     }
   else if (!strcmp(type, "elm_clock"))
     {
        accessible = g_object_new(EAIL_TYPE_CLOCK, NULL);
     }
   else if (!strcmp(type, "elm_calendar"))
     {
        accessible = g_object_new(EAIL_TYPE_CALENDAR, NULL);
     }
   else if (!strcmp(type, "elm_grid"))
     {
        accessible = g_object_new(EAIL_TYPE_GRID, NULL);
     }
   else if (!strcmp(type, "elm_route"))
     {
        accessible = g_object_new(EAIL_TYPE_ROUTE, NULL);
     }
   else if (!strcmp(type, "elm_dayselector"))
     {
        accessible = g_object_new(EAIL_TYPE_DAYSELECTOR, NULL);
     }
   else if (!strcmp(type, "elm_gengrid"))
     {
        accessible = g_object_new(EAIL_TYPE_GENGRID, NULL);
     }
   else if (!strcmp(type, "elm_progressbar"))
     {
        accessible = g_object_new(EAIL_TYPE_PROGRESSBAR, NULL);
     }
   else if (!strcmp(type, "elm_diskselector"))
     {
        accessible = g_object_new(EAIL_TYPE_DISKSELECTOR, NULL);
     }
   else if (!strcmp(type, "elm_segment_control"))
     {
        accessible = g_object_new(EAIL_TYPE_SEGMENT_CONTROL, NULL);
     }
   else if (!strcmp(type, "elm_conformant"))
     {
        accessible = g_object_new(EAIL_TYPE_CONFORMANT, NULL);
     }
   else if (!strcmp(type, "elm_notify"))
     {
        accessible = g_object_new(EAIL_TYPE_NOTIFY, NULL);
     }
   else if ((!strcmp(type, "popup")) || (!strcmp(type, "elm_popup")))
     {
        accessible = g_object_new(EAIL_TYPE_POPUP, NULL);
     }
   else if ((!strcmp(type, "ctxpopup")) || ((!strcmp(type, "elm_ctxpopup"))))
     {
        accessible = g_object_new(EAIL_TYPE_CTXPOPUP, NULL);
     }
   else if ((!strcmp(type, "toolbar")) || (!strcmp(type, "elm_toolbar")))
     {
        accessible = g_object_new(EAIL_TYPE_TOOLBAR, NULL);
     }
   else if (!strcmp(type, "elm_multibuttonentry"))
     {
        accessible = g_object_new(EAIL_TYPE_MULTIBUTTONENTRY, NULL);
     }
   else if (!strcmp(type, "elm_web"))
     {
        accessible = g_object_new(EAIL_TYPE_WEB, NULL);
     }
   else if (!strcmp(type, "elm_index"))
     {
        accessible = g_object_new(EAIL_TYPE_INDEX, NULL);
     }
   else if (!strcmp(type, "elm_fileselector"))
     {
        accessible = g_object_new(EAIL_TYPE_FILESELECTOR, NULL);
     }
   else if ((!strcmp(type, "elm_fileselector_entry")) ||
            (!strcmp(type, "fileselector_entry")))
     {
        accessible = g_object_new(EAIL_TYPE_FILESELECTOR_ENTRY, NULL);
     }
   else if (!strcmp(type, "elm_fileselector_button"))
     {
        /* NOTE: file selector button from accesibility point of view is
         * no different in handling than regular push button */
        accessible = g_object_new(EAIL_TYPE_BUTTON, NULL);
     }
   else if (!strcmp(type, "elm_colorselector"))
     {
        accessible = g_object_new(EAIL_TYPE_COLORSELECTOR, NULL);
     }
   else if (!strcmp(type, "elm_naviframe"))
     {
        accessible = g_object_new(EAIL_TYPE_NAVIFRAME, NULL);
     }
   else if (!strcmp(type, "elm_datetime"))
     {
        accessible = g_object_new(EAIL_TYPE_DATETIME, NULL);
     }
   else if (!strcmp(type, "elm_hover"))
     {
        accessible = g_object_new(EAIL_TYPE_HOVER, NULL);
     }
   else if (!strcmp(type, "elm_panes"))
     {
        accessible = g_object_new(EAIL_TYPE_PANES, NULL);
     }
   else if ((!strcmp(type, "panel")) || (!strcmp(type, "elm_panel")))
     {
        accessible = g_object_new(EAIL_TYPE_PANEL, NULL);
     }
   else if (!strcmp(type, "elm_thumb"))
     {
        accessible = g_object_new(EAIL_TYPE_THUMB, NULL);
     }
   else if (!strcmp(type, "elm_mapbuf"))
     {
        accessible = g_object_new(EAIL_TYPE_MAPBUF, NULL);
     }
   else if (!strcmp(type, "elm_slideshow"))
     {
        accessible = g_object_new(EAIL_TYPE_SLIDESHOW, NULL);
     }
   else if (!strcmp(type, "elm_hoversel"))
     {
        accessible = g_object_new(EAIL_TYPE_HOVERSEL, NULL);
     }
   else if (!strcmp(type, "elm_map"))
     {
        accessible = g_object_new(EAIL_TYPE_MAP, NULL);
     }
   else if (!strcmp(type, "elm_glview"))
     {
        accessible = g_object_new(EAIL_TYPE_GLVIEW, NULL);
     }
   else if (!strcmp(type, "elm_bubble"))
     {
        accessible = g_object_new(EAIL_TYPE_BUBBLE, NULL);
     }
   else if (!strcmp(type, "elm_plug"))
     {
        accessible = g_object_new(EAIL_TYPE_PLUG, NULL);
     }
   else if ((!strcmp(type, "elm_video")) || (!strcmp(type, "elm_player")))
     {
        accessible = g_object_new(EAIL_TYPE_VIDEO, NULL);
     }
   else if (!strcmp(type, "elm_flip"))
     {
        accessible = g_object_new(EAIL_TYPE_FLIP, NULL);
     }
   else if (!strcmp(type, "elm_flipselector"))
     {
        accessible = g_object_new(EAIL_TYPE_FLIPSELECTOR, NULL);
     }
   else if (!strcmp(type, "elm_layout"))
     {
        accessible = g_object_new(EAIL_TYPE_LAYOUT, NULL);
     }
   else if (!strcmp(type, "elm_table"))
     {
        accessible = g_object_new(EAIL_TYPE_TABLE, NULL);
     }
   else if (!strcmp(type, "elm_prefs"))
     {
        accessible = g_object_new(EAIL_TYPE_PREFS, NULL);
     }
   else
     {
        DBG("Unrecognized specific widget type: %s", type);
        DBG("Creating basic widget with NULL type.");
        accessible = g_object_new(EAIL_TYPE_WIDGET, NULL);
     }

   if (accessible)
     {
        atk_object_initialize(accessible, widget);
     }

   return accessible;
}


/**
 * @brief Creates a new EailItem instance with the given ATK role
 *
 * @param item Elm_Object_Item instance
 * @param parent AtkObject instance that holds EailItem instance
 * @param role AtkRole instance that will be used by EailItem
 *
 * @returns EailItem representing the new EailItem instance
 * or NULL in case of an error
 */
static AtkObject *
_eail_factory_create_item_atk_obj(Elm_Object_Item *item,
                                  AtkRole role,
                                  AtkObject *parent)
{
   AtkObject *obj;

   obj = g_object_new(EAIL_TYPE_ITEM, NULL);
   obj->role = role;
   atk_object_set_parent(obj, parent);
   atk_object_initialize(obj, item);

   return obj;
}

/**
 * @param atk_obj AtkObject instance
 * @param item Elm_Object_Item instance
 */
void
eail_factory_append_item_to_cache(AtkObject *atk_obj, Elm_Object_Item *item)
{
   EailFactoryObj *factory_obj = g_new0(EailFactoryObj, 1);
   factory_obj->atk_obj = atk_obj;
   factory_obj->obj_item = item;

   eail_cached_objs = eina_list_append(eail_cached_objs, factory_obj);
}

/**
 * @param item Elm_Object_Item* instance to be found
 * @returns EailFactoryObj instance or NULL if item's representation was not
 * found
 */
EailFactoryObj *
eail_factory_find_obj_for_item(const Elm_Object_Item *item)
{
   const Eina_List *l = NULL;
   EailFactoryObj *factory_obj = NULL;

   EINA_LIST_FOREACH(eail_cached_objs, l, factory_obj)
     {
        if (item == factory_obj->obj_item)
          {
             return factory_obj;
          }
     }

   /* no Atk Elm_Object_Item implementation in cache was found */
   return NULL;
}

/**
 *
 * If AtkObject was already created for the given item, instance will be returned
 * from cache instead of creating a new one (it is needed to allow the setting and
 * storing of name/description changes by multiple clients).
 *
 * @param item Elm_Object_Item instance that will be used as a source for the AtkObject
 * representation
 * @param role role for given item
 * @param parent AtkObject instance that has the given item inside its content (eg. for
 * list-item this will be EailList instance)
 *
 * @returns AtkObject representing the item or NULL in case of a failure
 */
AtkObject *
eail_factory_get_item_atk_obj(Elm_Object_Item *item,
                              AtkRole role,
                              AtkObject *parent)
{
   AtkObject *atk_obj;
   EailFactoryObj *factory_obj = NULL;

   if (!EAIL_IS_ITEM_PARENT(parent))
    {
       ERR("Parent does not implement ITEM_PARENT interface");
       return NULL;
    }

   /* first trying to get object from cache*/
   factory_obj = eail_factory_find_obj_for_item(item);
   if (factory_obj)
      {
         /* already found implementation, no need to create new obj, returning
          * existing one from cache */
          if (!factory_obj->atk_obj)
            {
               ERR("No atk obj found in eail_factory_widget!");
               return NULL;
            }

         return factory_obj->atk_obj;
      }

   /* if not in cache then creating and then adding to cache for future use*/
   atk_obj = _eail_factory_create_item_atk_obj(item, role, parent);
   eail_factory_append_item_to_cache(atk_obj, item);

   return atk_obj;
}

/**
 *
 * @param item Elm_Object_Item* instance to be deregistered
 */
void
eail_factory_unregister_item_from_cache(Elm_Object_Item *item)
{
   EailFactoryObj *factory_obj = NULL;

   factory_obj = eail_factory_find_obj_for_item(item);
   if (!factory_obj) return;

   eail_cached_objs = eina_list_remove(eail_cached_objs, factory_obj);

   g_free(factory_obj);
}

/**
 *
 * @param widget Evas_Object* instance to be deregistered
 */
void
eail_factory_unregister_wdgt_from_cache(Evas_Object *widget)
{
   EailFactoryObj *factory_obj = NULL;

   factory_obj = _eail_factory_find_obj_for_widget(widget);
   if (!factory_obj) return;

   eail_cached_objs = eina_list_remove(eail_cached_objs, factory_obj);

   g_free(factory_obj);
}

/**
 * @param widget Evas_Object* instance
 *
 * The returned AtkObject should be unreferenced when no longer needed
 * to free memory.
 *
 * @returns AtkObject which is the accessible representation of the specified
 * Evas_Object
 */
AtkObject *
eail_factory_get_accessible(Evas_Object *widget)
{
   EailFactoryObj *factory_widget = NULL;
   AtkObject *atk_obj = NULL;

   if (!widget)
     {
        DBG("Cannot create accessible for NULL-widget");
        return NULL;
     }

   if (!elm_object_widget_check(widget))
     {
        DBG("%s is NOT elementary object widget!",
            evas_object_type_get(widget));
        return NULL;
     }

   /* looking for object in cache*/
   factory_widget = _eail_factory_find_obj_for_widget(widget);
   if (factory_widget)
     {
        /* already found implementation, no need to create new obj, returning
         * existing one from cache */
         if (!factory_widget->atk_obj)
           {
              ERR("No atk obj found in eail_factory_widget!");
              return NULL;
           }

         return factory_widget->atk_obj;
     }

   /* not found in cache, need to create new instance for atk obj
    * representation*/
   atk_obj = _eail_factory_create_accessible(widget);
   if (!atk_obj)
     {
        ERR("No atk obj created in factory");
        return NULL;
     }

   /* appending obj for future re-using*/
   _eail_factory_append_wdgt_to_cache(atk_obj, widget);

   return atk_obj;
}

/**
 * @param role an AtkRole for objects that have to be found
 *
 * @returns an Eina_List filled with AtkObject * objects
 */
Eina_List *
eail_factory_find_objects_with_role(AtkRole role)
{
   const Eina_List *l = NULL;
   Eina_List *results_list = NULL;
   EailFactoryObj *factory_obj = NULL;

   EINA_LIST_FOREACH(eail_cached_objs, l, factory_obj)
     {
        AtkObject *atk_obj = factory_obj->atk_obj;
        if (atk_obj && role == atk_obj->role)
          results_list = eina_list_append(results_list, atk_obj);
     }

   return results_list;
}
