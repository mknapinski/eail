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
 * @file eail_dynamic_content.h
 *
 * @brief Header for EailDynamicContent implementation
 */

#ifndef EAIL_DYNAMIC_CONTENT_H
#define EAIL_DYNAMIC_CONTENT_H

#include <glib-object.h>

/**
 * @brief Returns a value corresponding to the type of EailDynamicContent interface
 */
#define EAIL_TYPE_DYNAMIC_CONTENT           (eail_dynamic_content_get_type())
/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailDynamicContent
 * interface
 *
 * @param obj an AtkObject
 */
#define EAIL_DYNAMIC_CONTENT(obj)  (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                 EAIL_TYPE_DYNAMIC_CONTENT, EailDynamicContent))
/**
 * @brief Tests whether object (obj) implements EailDynamicContent interface
 *
 * @param obj an AtkObject
 */
#define EAIL_IS_DYNAMIC_CONTENT(obj)        (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                         EAIL_TYPE_DYNAMIC_CONTENT))
/**
 * @brief Gets EailDynamicContent interface structure from an obj (class instance)
 *
 * @param obj object instance to get EailDynamicContent interface from
 */
#define EAIL_DYNAMIC_CONTENT_GET_IFACE(obj) (G_TYPE_INSTANCE_GET_INTERFACE((obj),\
                              EAIL_TYPE_DYNAMIC_CONTENT, EailDynamicContentIface))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailDynamicContent*/
typedef struct _EailDynamicContent      EailDynamicContent;
/** @brief Definition of interface structure for Atk EailDynamicContent*/
typedef struct _EailDynamicContentIface EailDynamicContentIface;

/** @brief Definition of interface structure for Atk EailDynamicContent*/
struct _EailDynamicContentIface
{
   GTypeInterface parent;/**< @brief parent interface that is being extended*/

   /**@brief callback definition for eail_dynamic_content_update_hierarchy func*/
   void (*update_hierarchy)            (EailDynamicContent   *parent);
};

GType eail_dynamic_content_get_type(void);
void eail_dynamic_content_update_hierarchy(EailDynamicContent *parent);

#ifdef __cplusplus
}
#endif

#endif
