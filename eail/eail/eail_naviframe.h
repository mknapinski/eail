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

#ifndef EAIL_NAVIFRAME_H
#define EAIL_NAVIFRAME_H

/**
 * @file eail_naviframe.h
 *
 * @brief Header for EailNaviframe implementation
 */

#include "eail_action_widget.h"
/**
 * @brief Returns a value corresponding to the type of EailNaviframe class
 */
#define EAIL_TYPE_NAVIFRAME             (eail_naviframe_get_type())
/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailNaviframe
 * type
 *
 * @param obj an AtkObject
 */
#define EAIL_NAVIFRAME(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                         EAIL_TYPE_NAVIFRAME, EailNaviframe))
/**
 * @brief Macro upcasts a subclass (klass) to the EailNaviframe class
 *
 * @param klass subclass object
 */
#define EAIL_NAVIFRAME_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST((klass), \
                                         EAIL_TYPE_NAVIFRAME, EailNaviframeClass))
/**
 * @brief Tests whether object (obj) is an instance of EailNaviframe class
 *
 * @param obj an AtkObject
 */
#define EAIL_IS_NAVIFRAME(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                         EAIL_TYPE_NAVIFRAME))
/**
 * @brief Tests whether given klass is a subclass of EailNaviframe
 *
 * @param klass klass object
 */
#define EAIL_IS_NAVIFRAME_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                         EAIL_TYPE_NAVIFRAME))
/**
 * @brief Gets EailNaviframe class structure from an obj (class instance)
 *
 * @param obj object instance to get EailNaviframe class from
 */
#define EAIL_NAVIFRAME_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                         EAIL_TYPE_NAVIFRAME, EailNaviframeClass))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailNaviframe*/
typedef struct _EailNaviframe      EailNaviframe;
/** @brief Definition of object class for Atk EailNaviframe*/
typedef struct _EailNaviframeClass EailNaviframeClass;

/** @brief Definition of object structure for Atk EailNaviframe*/
struct _EailNaviframe
{
   EailActionWidget parent;/**< @brief widget that functionality is being extended*/
   char *click_description;/**< @brief string description for 'click' action*/
   int child_count_last;/**< @brief last reported cound of children */
};

/** @brief Definition of object class for Atk EailNaviframe*/
struct _EailNaviframeClass
{
   EailActionWidgetClass parent_class;/**< @brief class that is being extended*/
};

/**
 * @brief Getter for EailNaviframe GType
 *
 * @returns GType for EailNaviframe implementation
 */
GType eail_naviframe_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
