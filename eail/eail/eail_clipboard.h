#ifndef EAIL_CLIPBOARD
#define EAIL_CLIPBOARD

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
 * @file eail_clipboard.h
 *
 * @brief Header for EailClipboard implementation
 */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @brief Gets clipboard text
 */
const char *eail_clipboard_get_text(void);

/**
 * @brief Sets clipboard text
 */
void eail_clipboard_set_text(const char *text);

/**
 * @brief Frees memory allocated by clipboard
 */
void eail_clipboard_free(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* EAIL_CLIPBOARD */
