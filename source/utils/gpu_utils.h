/*
 * This file is part of vitaGL
 * Copyright 2017, 2018, 2019, 2020 Rinnegatamante
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/* 
 * gpu_utils.h:
 * Header file for the GPU utilities exposed by gpu_utils.c
 */

#ifndef _GPU_UTILS_H_
#define _GPU_UTILS_H_

#include "mem_utils.h"

// Align a value to the requested alignment
#define ALIGN(x, a) (((x) + ((a)-1)) & ~((a)-1))

// Texture object status enum
enum {
	TEX_UNUSED,
	TEX_UNINITIALIZED,
	TEX_VALID
};

// Texture object struct
typedef struct {
	SceGxmTexture gxm_tex;
	void *data;
	SceUID palette_UID;
	SceUID depth_UID;
	uint8_t status;
	uint32_t type;
	void (*write_cb)(void *, uint32_t);
	SceGxmTextureFilter min_filter;
	SceGxmTextureFilter mag_filter;
	SceGxmTextureAddrMode u_mode;
	SceGxmTextureAddrMode v_mode;
	SceGxmTextureMipFilter mip_filter;
	uint32_t lod_bias;
	uint8_t mip_count;
	GLboolean use_mips;
	uint8_t ref_counter;
	GLboolean dirty;
#ifdef HAVE_UNPURE_TEXTURES
	int8_t mip_start;
#endif
} texture;

// Palette object struct
typedef struct {
	void *data;
} palette;

// Alloc a generic memblock into sceGxm mapped memory
void *gpu_alloc_mapped(size_t size, vglMemType type);
void *gpu_alloc_mapped_temp(size_t size);

// Alloc into sceGxm mapped memory a vertex USSE memblock
void *gpu_vertex_usse_alloc_mapped(size_t size, unsigned int *usse_offset);

// Dealloc from sceGxm mapped memory a vertex USSE memblock
void gpu_vertex_usse_free_mapped(void *addr);

// Alloc into sceGxm mapped memory a fragment USSE memblock
void *gpu_fragment_usse_alloc_mapped(size_t size, unsigned int *usse_offset);

// Dealloc from sceGxm mapped memory a fragment USSE memblock
void gpu_fragment_usse_free_mapped(void *addr);

// Calculate bpp for a requested texture format
int tex_format_to_bytespp(SceGxmTextureFormat format);

// Alloc a texture
void gpu_alloc_texture(uint32_t w, uint32_t h, SceGxmTextureFormat format, const void *data, texture *tex, uint8_t src_bpp, uint32_t (*read_cb)(void *), void (*write_cb)(void *, uint32_t), GLboolean fast_store);

// Alloc a compresseed texture
void gpu_alloc_compressed_texture(int32_t level, uint32_t w, uint32_t h, SceGxmTextureFormat format, uint32_t image_size, const void *data, texture *tex, uint8_t src_bpp, uint32_t (*read_cb)(void *));

// Dealloc a texture
void gpu_free_texture(texture *tex);

// Alloc a palette
palette *gpu_alloc_palette(const void *data, uint32_t w, uint32_t bpe);

// Dealloc a palette
void gpu_free_palette(palette *pal);

// Generate mipmaps for a given texture
void gpu_alloc_mipmaps(int level, texture *tex);

#endif
