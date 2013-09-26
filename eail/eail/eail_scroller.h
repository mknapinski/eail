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

#ifndef EAIL_SCROLLER_H
#define EAIL_SCROLLER_H

/**
 * @file eail_scroller.h
 *
 * @brief Header for EailScrollableWidget implementation
 */

#include "eail_scrollable_widget.h"
/**
 * @brief Returns a value corresponding to the type of EailScroller class
 */
#define EAIL_TYPE_SCROLLER              (eail_scroller_get_type())
/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailScroller
 * type
 *
 * @param obj AtkObject instance
 */
#define EAIL_SCROLLER(obj)              (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                         EAIL_TYPE_SCROLLER, EailScroller))
/**
 * @brief Macro upcasts a subclass (klass) to the EailScroller class
 *
 * @param klass subclass object
 */
#define EAIL_SCROLLER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass), \
                                         EAIL_TYPE_SCROLLER, EailScrollerClass))
/**
 * @brief Tests whether object (obj) is an instance of EailScroller class
 *
 * @param obj AtkObject instance
 */
#define EAIL_IS_SCROLLER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                         EAIL_TYPE_SCROLLER))
/**
 * @brief Tests whether given klass is a subclass of EailScroller
 *
 * @param klass klass object
 */
#define EAIL_IS_SCROLLER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                         EAIL_TYPE_SCROLLER))
/**
 * @brief Gets EailScroller class structure from an obj (class instance)
 *
 * @param obj object instance to get EailScroller class from
 */
#define EAIL_SCROLLER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                         EAIL_TYPE_SCROLLER, EailScrollerClass))
#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailScroller*/
typedef struct _EailScroller      EailScroller;
/** @brief Definition of object class for Atk EailScroller*/
typedef struct _EailScrollerClass EailScrollerClass;

/** @brief Definition of object structure for Atk EailScroller*/
struct _EailScroller
{
   /** @brief Parent widget whose functionality is being extended*/
    EailScrollableWidget parent;
};

/** @brief Definition of object class for Atk EailScroller*/
struct _EailScrollerClass
{
   /** @brief Parent object that functionality is being extended*/
   EailScrollableWidgetClass parent_class;
};

/** @brief Getter for EailScroller GType
 *
 * @returns GType for EailScroller implementation*/
GType   eail_scroller_get_type  (void);

#ifdef __cplusplus
}
#endif

#endif
