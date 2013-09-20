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
 * @file eail_plug.h
 *
 * @brief Header for EailPlug implementation
 */

#ifndef EAIL_PLUG_H
#define EAIL_PLUG_H

#include "eail_widget.h"
/**
 * @brief Returns a value corresponding to the type of EailPlug class
 */
#define EAIL_TYPE_PLUG             (eail_plug_get_type())
/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailPlug
 * type
 *
 * @param obj an AtkObject
 */
#define EAIL_PLUG(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                    EAIL_TYPE_PLUG, EailPlug))
/**
 * @brief Macro upcasts a subclass (klass) to the EailPlug class
 *
 * @param klass subclass object
 */
#define EAIL_PLUG_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST((klass), \
                                    EAIL_TYPE_PLUG, EailPlugClass))
/**
 * @brief Tests whether object (obj) is an instance of EailPlug class
 *
 * @param obj an AtkObject
 */
#define EAIL_IS_PLUG(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                    EAIL_TYPE_PLUG))
/**
 * @brief Tests whether given klass is a subclass of EailPlug
 *
 * @param klass klass object
 */
#define EAIL_IS_PLUG_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                    EAIL_TYPE_PLUG))
/**
 * @brief Gets EailPlug class structure from an obj (class instance)
 *
 * @param obj object instance to get EailPlug class from
 */
#define EAIL_PLUG_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                    EAIL_TYPE_PLUG, EailPlugClass))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailPlug*/
typedef struct _EailPlug      EailPlug;
/** @brief Definition of object class for Atk EailPlug*/
typedef struct _EailPlugClass EailPlugClass;

/** @brief Definition of object structure for Atk EailPlug*/
struct _EailPlug
{
   /** @brief Parent object that functionality is being extended*/
   EailWidget parent;

   char *click_description; /*!< @brief 'click' action description*/
};

/** @brief Definition of object class for Atk EailPlug*/
struct _EailPlugClass
{
   /** @brief Parent class whose functionality is being extended*/
   EailWidgetClass parent_class;
};

/** @brief Getter for EailPlug GType
 *
 * @returns GType for EailPlug implementation*/
GType eail_plug_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
