/*
 * Copyright (c) 2013 Samsung Electronics Co., Ltd.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
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
 * @file eail-explorer-ui.h
 *
 * @brief Header for Eail Explorer UI
 */

#include <Elementary.h>
#include <atk/atk.h>

#define EDJE_PATH DATA_PATH "/layout.edj"

#define EDJ_PATH_OBJ EDJE_PATH
#define EDJ_PATH_ACT EDJE_PATH
#define EDJ_PATH_CMP EDJE_PATH
#define EDJ_PATH_IMG EDJE_PATH
#define EDJ_PATH_SEL EDJE_PATH
#define EDJ_PATH_TBL EDJE_PATH
#define EDJ_PATH_VAL EDJE_PATH
#define EDJ_PATH_TXT EDJE_PATH
#define EDJ_PATH_ETXT EDJE_PATH
#define EDJ_PATH_MAIN EDJE_PATH

#define OBJECT "OBJECT"
#define ACTION "ACTION"
#define COMPONENT "COMPONENT"
#define IMAGE "IMAGE"
#define SELECTION "SELECTION"
#define TABLE "TABLE"
#define TEXT "TEXT"
#define EDITABLE_TEXT "EDITABLE TEXT"
#define VALUE "VALUE"


typedef struct _action_cb_struct{
	AtkObject *atk_obj;
	Eina_List *list;
} action_cb_struct;

void free_memory(void);
void refresh_layout(Evas_Object *toolbar);
void highlight_and_refresh(AtkObject *obj, Eina_Bool expand);
Eina_Bool is_eail_explorer_win(AtkObject *obj);
Eina_Bool _is_part_of_eail_explorer(AtkObject *obj);
Evas_Object* prepare_layout_action(Evas_Object *parent);
Evas_Object* prepare_layout_component(Evas_Object *parent);
Evas_Object* prepare_layout_object(Evas_Object *parent);
Evas_Object* prepare_layout_table(Evas_Object *parent);
Evas_Object* prepare_layout_selection(Evas_Object *parent);
Evas_Object* prepare_layout_image(Evas_Object *parent);
Evas_Object* prepare_layout_value(Evas_Object *parent);
Evas_Object* prepare_layout_text(Evas_Object *parent);
Evas_Object* prepare_layout_edit_text(Evas_Object *parent);
void print_actions(Evas_Object *eo, AtkObject *obj, Evas_Object *parent);
void print_component(Evas_Object *eo, AtkObject *obj, Evas_Object *parent);
void print_object(Evas_Object *eo, AtkObject *obj, Evas_Object *parent);
void print_selection(Evas_Object *eo, AtkObject *obj, Evas_Object *parent);
void print_table(Evas_Object *eo, AtkObject *obj, Evas_Object *tb, Evas_Object *parent);
void print_image(Evas_Object *eo, AtkObject *obj, Evas_Object *tb, Evas_Object *parent);
void print_value(Evas_Object *eo, AtkObject *obj, Evas_Object *toolbar, Evas_Object *parent);
void print_edit_text(Evas_Object *eo, AtkObject *obj, Evas_Object *parent);
void print_text(Evas_Object *eo, AtkObject *obj, Evas_Object *parent);
