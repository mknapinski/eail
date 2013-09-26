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
 * @file eail_bubble.h
 *
 * @brief Header for EailBubble implementation
 */

#ifndef EAIL_BUBBLE_H
#define EAIL_BUBBLE_H

#include "eail_text.h"
/**
 * @brief Returns a value corresponding to the type of EailBubble class
 */
#define EAIL_TYPE_BUBBLE             (eail_bubble_get_type())
/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailBubble
 * type
 *
 * @param obj AtkObject instance
 */
#define EAIL_BUBBLE(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                      EAIL_TYPE_BUBBLE, EailBubble))
/**
 * @brief Macro upcasts a subclass (klass) to the EailBubble class
 *
 * @param klass subclass object
 */
#define EAIL_BUBBLE_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST((klass), \
                                      EAIL_TYPE_BUBBLE, EailBubbleClass))
/**
 * @brief Tests whether object (obj) is an instance of EailBubble class
 *
 * @param obj AtkObject instance
 */
#define EAIL_IS_BUBBLE(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                      EAIL_TYPE_BUBBLE))
/**
 * @brief Tests whether given klass is a subclass of EailBubble
 *
 * @param klass klass object
 */
#define EAIL_IS_BUBBLE_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                      EAIL_TYPE_BUBBLE))
/**
 * @brief Gets EailBubble class structure from an obj (class instance)
 *
 * @param obj object instance to get EailBubble class from
 */
#define EAIL_BUBBLE_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                      EAIL_TYPE_BUBBLE, EailBubbleClass))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailBubble*/
typedef struct _EailBubble      EailBubble;
/** @brief Definition of class structure for Atk EailBubble*/
typedef struct _EailBubbleClass EailBubbleClass;

/** @brief Definition of object structure for Atk EailBubble*/
struct _EailBubble
{
   /** @brief Parent widget whose functionality is being extended */
   EailText parent;
   /** @brief "click" action description*/
   char *click_description;
};

/** @brief Definition of class structure for Atk EailBubble*/
struct _EailBubbleClass
{
   /** @brief parent class whose functionality is being extended */
   EailTextClass parent_class;
};

/** @brief Getter for EailBubble GType
 *
 * @returns GType for EailBubble implementation
 * */
GType eail_bubble_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
