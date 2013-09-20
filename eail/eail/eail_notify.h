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

#ifndef EAIL_NOTIFY_H
#define EAIL_NOTIFY_H

/**
 * @file eail_notify.h
 *
 * @brief Header for EailNotify implementation
 */

#include "eail_widget.h"
/**
 * @brief Returns a value corresponding to the type of EailNotify class
 */
#define EAIL_TYPE_NOTIFY             (eail_notify_get_type())
/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailNotify
 * type
 *
 * @param obj an AtkObject
 */
#define EAIL_NOTIFY(obj)             (G_TYPE_NOTIFY_INSTANCE_CAST((obj), \
                                            EAIL_TYPE_NOTIFY, EailNotify))
/**
 * @brief Macro upcasts a subclass (klass) to the EailNotify class
 *
 * @param klass subclass object
 */
#define EAIL_NOTIFY_CLASS(klass)     (G_TYPE_NOTIFY_CLASS_CAST((klass), \
                                            EAIL_TYPE_NOTIFY, EailNotifyClass))
/**
 * @brief Tests whether object (obj) is an instance of EailNotify class
 *
 * @param obj an AtkObject
 */
#define EAIL_IS_NOTIFY(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                            EAIL_TYPE_NOTIFY))
/**
 * @brief Tests whether given klass is a subclass of EailNotify
 *
 * @param klass klass object
 */
#define EAIL_IS_NOTIFY_CLASS(klass)  (G_TYPE_NOTIFY_CLASS_TYPE((klass), \
                                            EAIL_TYPE_NOTIFY))
/**
 * @brief Gets EailNotify class structure from an obj (class instance)
 *
 * @param obj object instance to get EailNotify class from
 */
#define EAIL_NOTIFY_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                            EAIL_TYPE_NOTIFY, EailNotifyClass))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailNotify*/
typedef struct _EailNotify EailNotify;
/** @brief Definition of object class for Atk EailNotify*/
typedef struct _EailNotifyClass EailNotifyClass;

/** @brief Definition of object structure for Atk EailNotify*/
struct _EailNotify
{
   /** @brief Parent object that functionality is being extended*/
   EailWidget parent;
};

/** @brief Definition of object class for Atk EailNotify*/
struct _EailNotifyClass
{
   /** @brief Parent class whose functionality is being extended*/
   EailWidgetClass parent_class;
};

/** @brief Getter for EailNotify GType
 *
 * @returns GType for EailNotify implementation*/
GType eail_notify_get_type(void);

#ifdef __cplusplus
}
#endif

#endif

