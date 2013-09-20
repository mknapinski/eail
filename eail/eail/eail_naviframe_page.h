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
 * @file eail_naviframe_page.h
 *
 * @brief Header for EailnaviframePage implementation
 */

#ifndef EAIL_NAVIFRAME_PAGE_H
#define EAIL_NAVIFRAME_PAGE_H

#include <Elementary.h>
#include <atk/atk.h>
/**
 * @brief Returns a value corresponding to the type of EailNaviframePage class
 */
#define EAIL_TYPE_NAVIFRAME_PAGE            (eail_naviframe_page_get_type())
/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailNaviframePage
 * type
 *
 * @param obj an AtkObject
 */
#define EAIL_NAVIFRAME_PAGE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj),\
                                             EAIL_TYPE_NAVIFRAME_PAGE, EailNaviframePage))
/**
 * @brief Macro upcasts a subclass (klass) to the EailNaviframePage class
 *
 * @param klass subclass object
 */
#define EAIL_NAVIFRAME_PAGE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), \
                                             EAIL_TYPE_NAVIFRAME_PAGE, EailNaviframePageClass))
/**
 * @brief Tests whether object (obj) is an instance of EailNaviframePage class
 *
 * @param obj an AtkObject
 */
#define EAIL_IS_NAVIFRAME_PAGE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj),\
                                             EAIL_TYPE_NAVIFRAME_PAGE))
/**
 * @brief Tests whether given klass is a subclass of EailNaviframePage
 *
 * @param klass klass object
 */
#define EAIL_IS_NAVIFRAME_PAGE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), \
                                             EAIL_TYPE_NAVIFRAME_PAGE))
/**
 * @brief Gets EailNaviframePage class structure from an obj (class instance)
 *
 * @param obj object instance to get EailNaviframePage class from
 */
#define EAIL_NAVIFRAME_PAGE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), \
                                             EAIL_TYPE_NAVIFRAME_PAGE, EailNaviframePageClass))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailNaviframePage*/
typedef struct _EailNaviframePage      EailNaviframePage;
/** @brief Definition of object class for Atk EailNaviframePage*/
typedef struct _EailNaviframePageClass EailNaviframePageClass;

/** @brief Definition of object structure for Atk EailNaviframePage*/
struct _EailNaviframePage
{
   AtkObject parent; /*!< EailNaviframePage parent*/

   AtkObject *parent_naviframe; /*!< EailNaviframe to which page belongs to*/
   Evas_Object *naviframe; /*!< elm_naviframe instance*/
   Elm_Object_Item *page; /*!< elm_naviframe item instance*/

   Evas_Object *content[4]; /*!< holds content of page*/

   gint child_count; /*!< number of children*/
   const char *name; /*!< title of page*/
};

/** @brief Definition of object class for Atk EailNaviframePage*/
struct _EailNaviframePageClass
{
   AtkObjectClass parent_class; /*!< EailNaviframPage parent class*/
};

/**
 * @brief Getter for EailNaviframePage GType
 * @returns GType for EailNaviframePage implementation
 */
GType eail_naviframe_page_get_type(void);

/**
 * @brief Creates new EailNaviframPage object
 *
 * @param naviframe EailNaviframe parent
 * @param navi_tab_item item in parent naviframe object
 *
 * @returns new EailNaviframePage object
 */
AtkObject *eail_naviframe_page_new(AtkObject *naviframe,
                                   Elm_Object_Item *navi_tab_item);

#ifdef __cplusplus
}
#endif

#endif
