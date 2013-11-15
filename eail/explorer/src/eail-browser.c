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
 * @file eail-browser.c
 *
 * @brief EAIL Explorer browser implementation
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include "eail-browser.h"
#include "eail-explorer-ui.h"
#include "eail-explorer-tr.h"

/** @brief Evas_Object for genlist browser declaration*/
static Evas_Object *genlist_browser = NULL;
/** @brief Elm_Genlist_Item_Class declaration for genlist items*/
static Elm_Genlist_Item_Class item_class;

/**
 * @brief Gets item label for first item in browser
 *
 * @param data Item data from append function, root object from atk
 * @param obj Genlist item
 * @param part The name string of one of the existing text parts.
 *
 * @return string representing the genlist item label
 */
static char* item_label_get(void *data, Evas_Object *obj, const char *part) {
	if(!ATK_IS_OBJECT(data)) return strdup("<dead>");

	AtkObject *atk_obj = (AtkObject *)data;
	const char *name = atk_object_get_name(atk_obj);
	const char *role = atk_role_get_name(atk_object_get_role(atk_obj));
	char *label;

	if(!name || !strlen(name)) {
		if (asprintf(&label, "%s", role) < 0) {
			EINA_LOG_ERR(ERR_MEMORY);
			return NULL;
		}
	}
	else {
		if (asprintf(&label, "%s [%s]", role , name) < 0) {
			EINA_LOG_ERR(ERR_MEMORY);
			return NULL;
		}
	}

	return label;
}

/**
 * @brief callback for "expand" event
 *
 * @param data UNUSED
 * @param o UNUSED
 * @param event_info Parameter is the genlist item that was indicated to expand
 */
static void
_expand_request_cb(void *data, Evas_Object *o, void *event_info)
{
	if(!event_info)
		return;
	elm_genlist_item_expanded_set((Elm_Object_Item *)event_info, EINA_TRUE);
}

/**
 * @brief callback for delete genlist item
 *
 * @param atk_obj atk object to delete
 * @param genlist UNUSED
 */
static void
_item_del(void *atk_obj, Evas_Object *genlist)
{
	if(ATK_IS_OBJECT(atk_obj))
		g_object_unref(atk_obj);
}

/**
 * @brief callback for "contract" event
 *
 * @param data UNUSED
 * @param o UNUSED
 * @param event_info The event_info parameter is the genlist item that was indicated to contract.
 */
static void
_contract_request_cb(void *data, Evas_Object *o, void *event_info)
{
	if(!event_info)
		return;
	elm_genlist_item_expanded_set((Elm_Object_Item *)event_info, EINA_FALSE);
}

/**
 * @brief callback generated when genlist item is selected
 *
 * When genlist item is selected highlight_and_refresh is called with atk object reference
 *
 * @param data Data passed from genlist adding items function, atk object
 * @param obj UNUSED
 * @param event_info UNUSED
 */
static void genlist_item_cb(void *data, Evas_Object *obj, void *event_info)
{
	highlight_and_refresh(data, EINA_FALSE);
}

/**
 * @brief callback for expanding the browser list
 *
 * @param data UNUSED
 * @param o Genlist Object UNUSED
 * @param event_info The event_info parameter is the genlist item that was indicated to expand.
 */
static void
_expanded_cb(void *data, Evas_Object *o, void *event_info)
{
	Elm_Object_Item *parent = (Elm_Object_Item *)event_info;
	AtkObject *obj = elm_object_item_data_get((Elm_Object_Item *)event_info);
	Elm_Genlist_Item_Type item_type;

	Elm_Object_Item *prev_item = parent;

	int i;
	for(i = 0; i < atk_object_get_n_accessible_children(obj); ++i){
		AtkObject *child = atk_object_ref_accessible_child(obj, i);
		Eina_Bool enabled = ATK_IS_OBJECT(child) && !is_eail_explorer_win(child);

		if(enabled && atk_object_get_n_accessible_children(child) > 0)
			item_type = ELM_GENLIST_ITEM_TREE;
		else
			item_type = ELM_GENLIST_ITEM_NONE;

		prev_item = elm_genlist_item_insert_after (genlist_browser, &item_class, child, event_info, prev_item, item_type, genlist_item_cb, child);
		if(!enabled)
			elm_object_item_disabled_set(prev_item, EINA_TRUE);
	}
}

/**
 * @brief contracts object list in browser panel
 *
 * @param data UNUSED
 * @param o Genlist Object UNUSED
 * @param event_info The event_info parameter is the genlist item that was indicated to contract.
 */
static void
_contracted_cb(void *data, Evas_Object *o, void *event_info)
{
	Elm_Object_Item *glit = event_info;
	elm_genlist_item_subitems_clear(glit);
}

/**
 * @brief creates browser panel
 *
 * Function creates browser panel, register proper callbacks and add root object to tree.
 *
 * @param layout Main layout of the application
 */
void create_browser(Evas_Object *layout)
{
	genlist_browser = elm_genlist_add(layout);
	elm_object_part_content_set(layout, "elm.browser", genlist_browser);
	evas_object_show(genlist_browser);

	item_class.item_style = "default";
	item_class.func.text_get = item_label_get;
	item_class.func.del = _item_del;

	evas_object_smart_callback_add(genlist_browser, "expand,request", _expand_request_cb, NULL);
	evas_object_smart_callback_add(genlist_browser, "contract,request", _contract_request_cb, NULL);
	evas_object_smart_callback_add(genlist_browser, "expanded", _expanded_cb, NULL);
	evas_object_smart_callback_add(genlist_browser, "contracted", _contracted_cb, NULL);
	elm_genlist_item_append(genlist_browser, &item_class, atk_get_root(), NULL, ELM_GENLIST_ITEM_TREE, genlist_item_cb, atk_get_root());
}

/**
 * @brief performs browser expand action
 *
 * @param item First item in genlist browser
 * @param focused_obj Atk Object which has focus
 */
static void rec_expand_browser(Elm_Object_Item * item, AtkObject *focused_obj)
{
	if (!item || !focused_obj) return;

	if(ELM_GENLIST_ITEM_TREE == elm_genlist_item_type_get(item))
		elm_genlist_item_expanded_set(item, EINA_TRUE);

	if(elm_object_item_data_get(item) == focused_obj) {
		Elm_Object_Item *tmp_item = NULL;
		Eina_List *path = NULL;

		for(tmp_item = item; NULL != tmp_item; tmp_item = elm_genlist_item_parent_get(tmp_item))
			path = eina_list_append(path, tmp_item);

		for(tmp_item = item; NULL != tmp_item; tmp_item = elm_genlist_item_prev_get(tmp_item))
			if(NULL == eina_list_data_find(path, tmp_item))
				elm_genlist_item_expanded_set(tmp_item, EINA_FALSE);

		elm_genlist_item_selected_set(item, EINA_TRUE);
		elm_genlist_item_show(item, ELM_GENLIST_ITEM_SCROLLTO_MIDDLE);
		eina_list_free(path);
		return;
	}

	rec_expand_browser(elm_genlist_item_next_get(item), focused_obj);
}

/**
 * @brief expands browser and finds the currently focused object in the tree
 *
 * @param obj AtkObject which has focus
 */
void expand_browser(AtkObject *obj)
{
	Elm_Object_Item * item = elm_genlist_first_item_get(genlist_browser);

	AtkObject *focused_window = obj;
	while(ATK_ROLE_WINDOW != atk_object_get_role(focused_window))
		focused_window = atk_object_get_parent(focused_window);

	refresh_browser();
	elm_genlist_item_expanded_set(item, EINA_TRUE);

	while(elm_object_item_data_get(item) != focused_window)
		item = elm_genlist_item_next_get(item);

	rec_expand_browser(item, obj);
}

/**
 * @brief Upadtes all contents of selected browser item
 */
void selected_item_update(void)
{
	Elm_Object_Item * item = elm_genlist_selected_item_get(genlist_browser);
	if(!item) return;
	elm_genlist_item_update(item);
}

/**
 * @brief free genlist browser object
 */
void free_browser(void)
{
	if(!genlist_browser)
		return;
	elm_genlist_clear(genlist_browser);
	evas_object_del(genlist_browser);
	genlist_browser = NULL;
}

/**
 * @brief refreshes list in object browser
 */
void refresh_browser(void)
{
	elm_genlist_item_expanded_set(elm_genlist_first_item_get(genlist_browser), EINA_FALSE);
}
