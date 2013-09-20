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
 * @file eail_progressbar.h
 *
 * @brief Header for EailProgressbar implementation
 */

#ifndef EAIL_PROGRESSBAR_H
#define EAIL_PROGRESSBAR_H

#include "eail_widget.h"
/**
 * @brief Returns a value corresponding to the type of EailProgressbar class
 */

#define EAIL_TYPE_PROGRESSBAR             (eail_progressbar_get_type())
/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailProgressbar
 * type
 *
 * @param obj an AtkObject
 */
#define EAIL_PROGRESSBAR(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                           EAIL_TYPE_PROGRESSBAR, EailProgressbar))
/**
 * @brief Macro upcasts a subclass (klass) to the EailProgressbar class
 *
 * @param klass subclass object
 */
#define EAIL_PROGRESSBAR_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST((klass), \
                                           EAIL_TYPE_PROGRESSBAR, EailProgressbarClass))
/**
 * @brief Tests whether object (obj) is an instance of EailProgressbar class
 *
 * @param obj an AtkObject
 */
#define EAIL_IS_PROGRESSBAR(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                           EAIL_TYPE_PROGRESSBAR))
/**
 * @brief Tests whether given klass is a subclass of EailProgressbar
 *
 * @param klass klass object
 */
#define EAIL_IS_PROGRESSBAR_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                           EAIL_TYPE_PROGRESSBAR))
/**
 * @brief Gets EailProgressbar class structure from an obj (class instance)
 *
 * @param obj object instance to get EailProgressbar class from
 */
#define EAIL_PROGRESSBAR_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                           EAIL_TYPE_PROGRESSBAR, EailProgressbarClass))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailProgressbar*/
typedef struct _EailProgressbar      EailProgressbar;
/** @brief Definition of object class for Atk EailProgressbar*/
typedef struct _EailProgressbarClass EailProgressbarClass;

/** @brief Definition of object structure for Atk EailProgressbar*/
struct _EailProgressbar
{
   /** @brief Parent object that functionality is being extended*/
   EailWidget parent;
};

/** @brief Definition of object class for Atk EailProgressbar*/
struct _EailProgressbarClass
{
   /** @brief Parent class whose functionality is being extended*/
   EailWidgetClass parent_class;
};

/** @brief Getter for EailProgressbar GType
 *
 * @returns GType for EailProgressbar implementation*/
GType eail_progressbar_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
