/** Copyright (C) 2008-2011 Josh Ventura
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

#include <string>
#include <cstring>
using namespace std;

#include "Graphics_Systems/graphics_mandatory.h"
#include "Collision_Systems/collision_mandatory.h"
#include "Widget_Systems/widgets_mandatory.h"
#include "spritestruct.h"
#include "libEGMstd.h"
#include "IMGloading.h"
#include "estring.h"

#ifdef DEBUG_MODE
  #include "../Widget_Systems/widgets_mandatory.h"
  #define get_sprite(spr,id,r) \
    if (id < -1 or size_t(id) > enigma::sprite_idmax or !enigma::spritestructarray[id]) { \
      show_error("Cannot access sprite with id " + toString(id), false); \
      return r; \
    } const enigma::sprite *const spr = enigma::spritestructarray[id];
  #define get_spritev(spr,id) \
    if (id < -1 or size_t(id) > enigma::sprite_idmax or !enigma::spritestructarray[id]) { \
      show_error("Cannot access sprite with id " + toString(id), false); \
      return; \
    } const enigma::sprite *const spr = enigma::spritestructarray[id];
  #define get_sprite_null(spr,id,r) \
    if (id < -1 or size_t(id) > enigma::sprite_idmax) { \
      show_error("Cannot access sprite with id " + toString(id), false); \
      return r; \
    } const enigma::sprite *const spr = enigma::spritestructarray[id];
  #define get_sprite_mutable(spr,id,r) \
    if (id < -1 or size_t(id) > enigma::sprite_idmax or !enigma::spritestructarray[id]) { \
      show_error("Cannot access sprite with id " + toString(id), false); \
      return r; \
    } enigma::sprite *const spr = enigma::spritestructarray[id];
  #define get_spritev_mutable(spr,id) \
    if (id < -1 or size_t(id) > enigma::sprite_idmax or !enigma::spritestructarray[id]) { \
      show_error("Cannot access sprite with id " + toString(id), false); \
      return; \
    } enigma::sprite *const spr = enigma::spritestructarray[id];
#else
  /// Retrieve a sprite by the given index, doing bound/null checking if in debug mode and bailing with result r if an error occurs.
  #define get_sprite(spr,id,r) \
    const enigma::sprite *const spr = enigma::spritestructarray[id];
  /// Retrieve a sprite by the given index, doing bound/null checking if in debug mode and bailing with no result if an error occurs.
  #define get_spritev(spr,id) \
    const enigma::sprite *const spr = enigma::spritestructarray[id];
  /// Retrieve a sprite by the given index, doing bounds checking (but not null checking) if in debug mode and bailing with result r if an error occurs.
  #define get_sprite_null(spr,id,r) \
    const enigma::sprite *const spr = enigma::spritestructarray[id];
  /// Retrieve a sprite (and a lock thereto) by the given index, doing bound/null checking if in debug mode and bailing with result r if an error occurs.
  #define get_sprite_mutable(spr,id,r) \
    enigma::sprite *const spr = enigma::spritestructarray[id];
  /// Retrieve a sprite (and a lock thereto) by the given index, doing bound/null checking if in debug mode and bailing with no result if an error occurs.
  #define get_spritev_mutable(spr,id) \
    enigma::sprite *const spr = enigma::spritestructarray[id];
#endif

namespace enigma {
  sprite** spritestructarray;
	extern size_t sprite_idmax;
	sprite::sprite(): texturearray(NULL), texbordxarray(NULL), texbordyarray(NULL) {}
  sprite::sprite(unsigned int x): texturearray(new unsigned int[x]), texbordxarray(new double[x]), texbordyarray(new double[x]) {}
}

int sprite_add(string filename, int imgnumb, bool precise, bool transparent, bool smooth, bool preload, int x_offset, int y_offset)
{
    if (filename.find_first_of("\\/") == string::npos)
        filename = get_working_directory() + filename;
	enigma::sprite *spr = enigma::spritestructarray[enigma::sprite_idmax] = new enigma::sprite;
    enigma::sprite_add_to_index(spr, filename, imgnumb, transparent, smooth, x_offset, y_offset);
	return enigma::sprite_idmax++;
}

int sprite_add(string filename, int imgnumb, bool transparent, bool smooth, int x_offset, int y_offset)
{
    if (filename.find_first_of("\\/") == string::npos)
        filename = get_working_directory() + filename;
	enigma::sprite *spr = enigma::spritestructarray[enigma::sprite_idmax] = new enigma::sprite;
    enigma::sprite_add_to_index(spr, filename, imgnumb, transparent, smooth, x_offset, y_offset);
	return enigma::sprite_idmax++;
}

bool sprite_replace(int ind, string filename, int imgnumb, bool precise, bool transparent, bool smooth, bool preload, int x_offset, int y_offset, bool free_texture)
{
    if (filename.find_first_of("\\/") == string::npos)
        filename = get_working_directory() + filename;
    get_sprite_mutable(spr,ind,false);
    if (free_texture)
        for (int ii = 0; ii < spr->subcount; ii++)
            enigma::graphics_delete_texture(spr->texturearray[ii]);

    delete[] spr->texturearray;
    delete[] spr->texbordxarray;
    delete[] spr->texbordyarray;
    enigma::sprite_add_to_index(spr, filename, imgnumb, transparent, smooth, x_offset, y_offset);
    return true;
}

bool sprite_replace(int ind, string filename, int imgnumb, bool transparent, bool smooth, int x_offset, int y_offset, bool free_texture)
{
    if (filename.find_first_of("\\/") == string::npos)
        filename = get_working_directory() + filename;
    get_sprite_mutable(spr,ind,false);
    if (free_texture)
        for (int ii = 0; ii < spr->subcount; ii++)
            enigma::graphics_delete_texture(spr->texturearray[ii]);

    delete[] spr->texturearray;
    delete[] spr->texbordxarray;
    delete[] spr->texbordyarray;
    enigma::sprite_add_to_index(spr, filename, imgnumb, transparent, smooth, x_offset, y_offset);
    return true;
}

void sprite_set_offset(int ind, int xoff, int yoff)
{
    get_spritev_mutable(spr,ind);
    spr->xoffset = xoff;
    spr->yoffset = yoff;
}

void sprite_delete(int ind, bool free_texture)
{
    get_spritev_mutable(spr,ind);
    if (free_texture)
        for (int ii = 0; ii < spr->subcount; ii++)
            enigma::graphics_delete_texture(spr->texturearray[ii]);

    delete enigma::spritestructarray[ind];
    enigma::spritestructarray[ind] = NULL;
}

int sprite_duplicate(int copy_sprite)
{
    get_sprite_mutable(spr_copy,copy_sprite,-1);
    enigma::sprite *spr = enigma::spritestructarray[enigma::sprite_idmax] = new enigma::sprite;
    spr->id = enigma::sprite_idmax;
    enigma::sprite_add_copy(spr, spr_copy);
	return enigma::sprite_idmax++;
}

void sprite_assign(int ind, int copy_sprite, bool free_texture)
{
    get_spritev_mutable(spr,ind);
    get_spritev_mutable(spr_copy,copy_sprite);
    if (free_texture)
        for (int ii = 0; ii < spr->subcount; ii++)
            enigma::graphics_delete_texture(spr->texturearray[ii]);

    delete[] spr->texturearray;
    delete[] spr->texbordxarray;
    delete[] spr->texbordyarray;
    enigma::sprite_add_copy(spr, spr_copy);
}

void sprite_set_alpha_from_sprite(int ind, int copy_sprite, bool free_texture)
{
    get_spritev_mutable(spr,ind);
    get_spritev_mutable(spr_copy,copy_sprite);
    for (int i = 0; i < spr->subcount; i++)
        enigma::graphics_replace_texture_alpha_from_texture(spr->texturearray[i], spr_copy->texturearray[0]);
}

void sprite_merge(int ind, int copy_sprite)
{
    get_spritev_mutable(spr,ind);
    get_spritev_mutable(spr_copy,copy_sprite);
    int i = 0, j = 0, t_subcount = spr->subcount + spr_copy->subcount;
    unsigned int *t_texturearray = new unsigned int[t_subcount];
    double *t_texbordxarray = new double[t_subcount], *t_texbordyarray = new double[t_subcount];
    while (i < spr->subcount)
    {
        t_texturearray[i] = spr->texturearray[i];
        t_texbordxarray[i] = spr->texbordxarray[i];
        t_texbordyarray[i] = spr->texbordyarray[i];
        i++;
    }
    while (j < spr_copy->subcount)
    {
        t_texturearray[i] = enigma::graphics_duplicate_texture(spr_copy->texturearray[j]);
        t_texbordxarray[i] = spr_copy->texbordxarray[j];
        t_texbordyarray[i] = spr_copy->texbordyarray[j];
        i++; j++;
    }
    spr->subcount = t_subcount;
    spr->width    = (spr->width > spr_copy->width)?spr->width:spr_copy->width;
    spr->height   = (spr->height > spr_copy->height)?spr->height:spr_copy->height;
    delete[] spr->texturearray;
    delete[] spr->texbordxarray;
    delete[] spr->texbordyarray;
    spr->texturearray = t_texturearray;
    spr->texbordxarray = t_texbordxarray;
    spr->texbordyarray = t_texbordyarray;
}

void sprite_set_bbox(int ind, int left, int top, int right, int bottom)
{
    get_spritev_mutable(spr,ind);
    spr->bbox.left = left;
    spr->bbox.top = top;
    spr->bbox.right = right;
    spr->bbox.bottom = bottom;
    spr->bbox_relative.left = left - spr->xoffset;
    spr->bbox_relative.top = top - spr->yoffset;
    spr->bbox_relative.right = right - spr->xoffset;
    spr->bbox_relative.bottom = bottom - spr->yoffset;
}

/* These functions are primarily for use of the engine. Experienced users
 * who are familiar with C++ can make use of these, but they were made to
 * use at load time with data read from the executable. These both expect
 * RAW format, RGB only.
 */
#include "nlpo2.h"

namespace enigma
{
  //Allocates and zero-fills the array at game start
  void sprites_init()
  {
    spritestructarray = new sprite*[sprite_idmax+1];
    for (unsigned i = 0; i < sprite_idmax; i++)
      spritestructarray[i] = NULL;
  }

    void spritestructarray_reallocate()
    {
        sprite** spriteold = spritestructarray;
        spritestructarray = new sprite*[sprite_idmax+2];
        for (size_t i = 0; i < sprite_idmax; i++) spritestructarray[i] = spriteold[i]; delete[] spriteold;
    }

    //Adds an empty sprite to the list
    int sprite_new_empty(unsigned sprid, unsigned subc, int w, int h, int x, int y, int bbt, int bbb, int bbl, int bbr, bool pl, bool sm)
    {
        sprite *as = new sprite(subc);
        spritestructarray[sprid] = as;

        as->id = sprid;
        as->subcount = subc;
        as->width  = w;
        as->height = h;
        as->bbox.bottom  = bbb;
          as->bbox.left  = bbl;
          as->bbox.top   = bbt;
          as->bbox.right = bbr;
        as->bbox_relative.bottom  = bbb - y;
          as->bbox_relative.left  = bbl - x;
          as->bbox_relative.top   = bbt - y;
          as->bbox_relative.right = bbr - x;
        as->xoffset = x;
        as->yoffset = y;

        as->texturearray = new unsigned int[subc];
        as->texbordxarray = new double[subc];
        as->texbordyarray = new double[subc];
        as->colldata = new void*[subc];

        if (sprite_idmax < sprid+1)
          sprite_idmax = sprid+1;

        return sprid;
    }

    void sprite_add_to_index(sprite *ns, string filename, int imgnumb, bool transparent, bool smooth, int x_offset, int y_offset)
    {
        int width,height,fullwidth,fullheight;

        char *pxdata = load_bitmap(filename,&width,&height,&fullwidth,&fullheight);
        unsigned texture = graphics_create_texture(fullwidth, fullheight, pxdata);
        delete[] pxdata;

        //ns.pixeldata=(void**) malloc(sizeof(void*));
        //ns.pixeldata[0]=bitmapbuffer;
        ns->id = sprite_idmax;
        ns->subcount  = 1;
        ns->width     = width;
        ns->height    = height;
        ns->xoffset   = (int)x_offset;
        ns->yoffset   = (int)y_offset;
        ns->texturearray = new unsigned int[1];
        ns->texturearray[0] = texture;
        ns->texbordxarray = new double[1];
        ns->texbordxarray[0] = (double) width/fullwidth;
        ns->texbordyarray = new double[1];
        ns->texbordyarray[0] = (double) height/fullheight;
    }

    void sprite_add_copy(sprite *spr, sprite *spr_copy)
    {
        spr->subcount  = spr_copy->subcount;
        spr->width     = spr_copy->width;
        spr->height    = spr_copy->height;
        spr->xoffset   = spr_copy->xoffset;
        spr->yoffset   = spr_copy->yoffset;
        spr->texturearray = new unsigned int[spr_copy->subcount];
        spr->texbordxarray = new double[spr_copy->subcount];
        spr->texbordyarray = new double[spr_copy->subcount];
        for (int i = 0; i < spr->subcount; i++)
        {
            spr->texturearray[i] = graphics_duplicate_texture(spr_copy->texturearray[i]);
            spr->texbordxarray[i] = spr_copy->texbordxarray[i];
            spr->texbordyarray[i] = spr_copy->texbordyarray[i];
        }
    }

  #if COLLIGMA
  collCustom* generate_bitmask(unsigned char* pixdata,int x,int y,int w,int h)
  {
    collCustom* thenewmask = new collCustom(w,h,x,y,x,y);
    for(int xp=0;xp<w*h;xp++)
      collbitSet(*thenewmask,xp % w,xp/w,pixdata[xp*4+3]>0); //the width times the number of rows, the current column*4, then 3
    return thenewmask;
  }
  #endif


  //Adds a subimage to an existing sprite from the exe
  void sprite_set_subimage(int sprid, int imgindex, int x,int y, unsigned int w,unsigned int h,unsigned char*chunk, unsigned char*collision_data, collision_type ct)
  {
    unsigned int fullwidth = nlpo2dc(w)+1, fullheight = nlpo2dc(h)+1;
    char *imgpxdata = new char[4*fullwidth*fullheight+1], *imgpxptr = imgpxdata;
    unsigned int rowindex,colindex;
    for (rowindex = 0; rowindex < h; rowindex++)
    {
      for(colindex = 0; colindex < w; colindex++)
      {
        *imgpxptr++ = *chunk++;
        *imgpxptr++ = *chunk++;
        *imgpxptr++ = *chunk++;
        *imgpxptr++ = *chunk++;
      }
      memset(imgpxptr, 0, (fullwidth-colindex) << 2);
      imgpxptr += (fullwidth-colindex) << 2;
    }
    memset(imgpxptr,0,(fullheight-h) * fullwidth);

    unsigned texture = graphics_create_texture(fullwidth,fullheight,imgpxdata);

    sprite* sprstr = spritestructarray[sprid];

    sprstr->texturearray[imgindex] = texture;
    sprstr->texbordxarray[imgindex] = (double) w/fullwidth;
    sprstr->texbordyarray[imgindex] = (double) h/fullheight;
    sprstr->colldata[imgindex] = get_collision_mask(sprstr,collision_data,ct);
    
    delete[] imgpxdata;
  }
}

int sprite_get_width(int sprite)
{
	get_sprite(spr,sprite,32);
	return spr->width;
}

int sprite_get_height(int sprite)
{
	get_sprite(spr,sprite,32);
	return spr->height;
}

double sprite_get_texture_width_factor(int sprite, int subimg)
{
	get_sprite(spr,sprite,32);
	return spr->texbordxarray[subimg];
}

double sprite_get_texture_height_factor(int sprite, int subimg)
{
	get_sprite(spr,sprite,32);
	return spr->texbordyarray[subimg];
}

int sprite_get_bbox_bottom(int sprite)
{
	get_sprite(spr,sprite,32);
	return spr->bbox.bottom;
}
int sprite_get_bbox_left(int sprite)
{
	get_sprite(spr,sprite,0);
	return spr->bbox.left;
}
int sprite_get_bbox_right(int sprite)
{
	get_sprite(spr,sprite,32);
	return spr->bbox.right;
}
int sprite_get_bbox_top(int sprite)
{
	get_sprite(spr,sprite,0);
	return spr->bbox.top;
}

int sprite_get_bbox_bottom_relative(int sprite)
{
	get_sprite(spr,sprite,32);
	return spr->bbox_relative.bottom;
}
int sprite_get_bbox_left_relative(int sprite)
{
	get_sprite(spr,sprite,0);
	return spr->bbox_relative.left;
}
int sprite_get_bbox_right_relative(int sprite)
{
	get_sprite(spr,sprite,32);
	return spr->bbox_relative.right;
}
int sprite_get_bbox_top_relative(int sprite)
{
	get_sprite(spr,sprite,0);
	return spr->bbox_relative.top;
}

#ifdef DEBUG_MODE
  bbox_rect_t dummy_bbox = {32,0,32,0};
#endif
const bbox_rect_t &sprite_get_bbox(int sprite)
{
	get_sprite(spr,sprite,dummy_bbox);
	return spr->bbox;
}
const bbox_rect_t &sprite_get_bbox_relative(int sprite)
{
	get_sprite(spr,sprite,dummy_bbox);
	return spr->bbox_relative;
}


//TODO: IMPLEMENT
//sprite_get_bbox_mode
//sprite_get_name
//sprite_get_precise
//sprite_get_preload
//sprite_get_smooth
//sprite_get_transparent

int sprite_get_number(int sprite)
{
  if (sprite == -1) return 0;
	get_sprite(spr,sprite,0);
	return spr->subcount;
}

int sprite_get_texture(int sprite,int subimage)
{
	get_sprite(spr,sprite,0);
	return spr->texturearray[subimage % spr->subcount];
}

int sprite_get_xoffset(int sprite)
{
	get_sprite(spr,sprite,0);
	return spr->xoffset;
}

int sprite_get_yoffset(int sprite)
{
	get_sprite(spr,sprite,0);
	return spr->yoffset;
}
