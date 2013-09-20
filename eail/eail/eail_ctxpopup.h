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
 * @file eail_ctxpopup.h
 *
 *  @brief Header for EailCtxpopup implementation
 */

#ifndef EAIL_CTXPOPUP_H
#define EAIL_CTXPOPUP_H

#include "eail_widget.h"
/**
 * @brief Returns a value corresponding to the type of EailCtxpopup class
 */

#define EAIL_TYPE_CTXPOPUP             (eail_ctxpopup_get_type())
/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailCtxpopup
 * type
 *
 * @param obj an AtkObject
 */
#define EAIL_CTXPOPUP(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                        EAIL_TYPE_CTXPOPUP, EailCtxpopup))
/**
 * @brief Macro upcasts a subclass (klass) to the EailCtxpopup class
 *
 * @param klass subclass object
 */
#define EAIL_CTXPOPUP_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST((klass), \
                                        EAIL_TYPE_CTXPOPUP, EailCtxpopupClass))
/**
 * @brief Tests whether object (obj) is an instance of EailCtxpopup class
 *
 * @param obj an AtkObject
 */
#define EAIL_IS_CTXPOPUP(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                        EAIL_TYPE_CTXPOPUP))
/**
 * @brief Tests whether given klass is a subclass of EailCtxpopup
 *
 * @param klass klass object
 */
#define EAIL_IS_CTXPOPUP_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                        EAIL_TYPE_CTXPOPUP))
/**
 * @brief Gets EailCtxpopup class structure from an obj (class instance)
 *
 * @param obj object instance to get EailCtxpopup class from
 */
#define EAIL_CTXPOPUP_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                        EAIL_TYPE_CTXPOPUP, EailCtxpopupClass))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailCtxpopup*/
typedef struct _EailCtxpopup      EailCtxpopup;

/** @brief Definition of class structure for Atk EailCtxpopup*/
typedef struct _EailCtxpopupClass EailCtxpopupClass;

/** @brief Definition of object structure for Atk EailCtxpopup*/
struct _EailCtxpopup
{
   EailWidget parent; /*!< parent class object*/

   char *dismiss_description; /*!< 'dismiss' action description*/
};

/** @brief Definition of class structure for Atk EailCtxpopup*/
struct _EailCtxpopupClass
{
   /** @brief parent class whose functionality is being extended */
   EailWidgetClass parent_class;
};

/** @brief Getter for EailCtxpopup GType
 *
 * @returns GType for EailCtxpopup implementation*/
GType eail_ctxpopup_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
