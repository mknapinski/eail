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
 * @file eail_app.c
 * @brief Implementation of elementary application
 */

#include <Elementary.h>

#include "eail_app.h"
#include "eail_factory.h"
#include "eail_dynamic_content.h"
#include "eail_utils.h"
#include "eail_priv.h"

/** @brief This is defined and filled in elementary.*/
extern Eina_List *_elm_win_list;

static void eail_dynamic_content_interface_init(EailDynamicContentIface *iface);

/**
 * @brief Definition of EailApp as GObject
 *
 * EailApp is extended ATK_TYPE_OBJECT with EAIL_TYPE_DYNAMIC_CONTENT iface
 * implemented
 */

G_DEFINE_TYPE_WITH_CODE(EailApp, eail_app, ATK_TYPE_OBJECT,
                    G_IMPLEMENT_INTERFACE(EAIL_TYPE_DYNAMIC_CONTENT,
                                          eail_dynamic_content_interface_init));

/**
 * @brief Initializer for AtkObjectClass
 *
 * @param obj an AtkObject to be initialized
 * @param data additional initialization data (Evas_Object*)
 */
static void
eail_app_initialize(AtkObject *obj, gpointer data)
{
   int argc;
   char **argv;
   EailApp *eail_app = NULL;

   ATK_OBJECT_CLASS(eail_app_parent_class)->initialize(obj, data);

   ecore_app_args_get(&argc, &argv);
   obj->name = argv ? g_strdup((gchar *)ecore_file_file_get(argv[0])) : NULL;
   obj->role = ATK_ROLE_APPLICATION;
   obj->accessible_parent = NULL;

   /* storing last numbers of children to be for checking if children-changed
     * signal has to be propagated */
   eail_app = EAIL_APP(obj);
   eail_app->child_count_last = atk_object_get_n_accessible_children(obj);
}

/**
 * @brief Destructor for EailApp object
 * @param obj obj to be freed
 */
static void
eail_app_class_finalize(GObject *obj)
{
   G_OBJECT_CLASS(eail_app_parent_class)->finalize(obj);
}

/**
 * @brief Implementation AtkObject->get_n_children callback
 *
 * ATK doc says:\n
 * Gets the number of accessible children of the accessible.
 *
 * @param obj an AtkObject (EailApp)
 *
 * @returns an integer representing the number of accessible children of
 * the accessible
 */
static gint
eail_app_get_n_children(AtkObject *obj)
{
   return eina_list_count(_elm_win_list);
}

/**
 * @brief Implementation AtkObject->ref_child callback
 *
 * ATK doc says:\n
 * Gets a reference to the specified accessible child of the object. The
 * accessible children are 0-based so the first accessible child is at index 0,
 * the second at index 1 and so on.
 *
 * @param obj an AtkObject (EailApp)
 * @param i index number
 *
 * @returns an AtkObject representing the specified accessible child of the
 * accessible.
 */
static AtkObject *
eail_app_ref_child(AtkObject *obj, gint i)
{
   AtkObject *child = NULL;

   if (eina_list_count(_elm_win_list) > i)
     {
        Evas_Object *tmp = eina_list_nth(_elm_win_list, i);
        child = eail_factory_get_accessible(tmp);
        g_object_ref(child);
     }

   return child;
}

/**
 * @brief Initializer for GObject class
 * @param app an object to be initialized
 */
static void
eail_app_init(EailApp *app)
{
}

/**
 * @brief Initializer for GObject EailAppClass class (defines callbacks for base
 * AtkObject)
 *
 * @param klass an EailAppClass class to be filled
 */
static void
eail_app_class_init(EailAppClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
   GObjectClass *g_object_class = G_OBJECT_CLASS(klass);

   class->initialize = eail_app_initialize;
   class->get_n_children = eail_app_get_n_children;
   class->ref_child = eail_app_ref_child;
   class->get_parent = NULL;

   g_object_class->finalize = eail_app_class_finalize;
}

/**
 * @param dynamic_content_holder an EailDynamicContent object (EailApp)
 */
void
eail_app_update_descendants(EailDynamicContent *dynamic_content_holder)
{
   DBG(">");
   gint n_children = 0;
   EailApp *app = NULL;
   if (!EAIL_IS_APP(dynamic_content_holder))
     {
        DBG("No EailApp found. Returning");
        return;
     }

   app = EAIL_APP(dynamic_content_holder);

   n_children = atk_object_get_n_accessible_children(ATK_OBJECT(app));
   if (n_children && n_children > app->child_count_last)
     {
         DBG("emit changed add");
        eail_emit_children_changed(TRUE, ATK_OBJECT(app), n_children - 1);
     }
   else if (n_children < app->child_count_last)
     {
         DBG("emit changed remove");
         eail_emit_children_changed
                     (FALSE, ATK_OBJECT(app), app->child_count_last);
     }

   app->child_count_last = n_children;
}

/**
 * @brief Initializer for dynamic content interface, used for handling objects
 * children hierarchy changes
 *
 * @param iface an EailDynamicContentIface
 */
static void
eail_dynamic_content_interface_init(EailDynamicContentIface *iface)
{
   iface->update_hierarchy = eail_app_update_descendants;
}
