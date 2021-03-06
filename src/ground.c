//==============================================================================

#include "ground.h"

#include <math.h>
#include <SDL2_image/SDL_image.h>

//------------------------------------------------------------------------------

VB_Ground *
vb_ground_init(VB_View *view)
{
  VB_Ground *ground = malloc(sizeof(VB_Ground));
  if (ground == NULL)
  {
    return NULL;
  }

  ground->tex = IMG_LoadTexture(view->renderer, "assets/checkerboard.jpg");
  if (ground->tex == NULL)
  {
    vb_ground_free(ground);
    return NULL;
  }

  for (int y = 0; y < HEIGHT; ++y)
  {
    ground->raster[y].z = 0.0;
    ground->raster[y].distance = 0.0;
    ground->raster[y].height = 0.0;
    ground->raster[y].gradient = 0.0;
  }

  for (int i = 0; i < DEPTH; ++i)
  {
    ground->points[i].z = i*10.0;
    ground->points[i].height = 10000.0*sin((float)i/10.0)-50000.0;
  }

  return ground;
}

//------------------------------------------------------------------------------

void
vb_ground_free(VB_Ground *ground)
{
  if (ground->tex != NULL)
  {
    SDL_DestroyTexture(ground->tex);
    ground->tex = NULL;
  }

  free(ground);
}

//------------------------------------------------------------------------------

void
vb_ground_update(VB_Ground *ground)
{
  for (int i = DEPTH-1; i >= 0; --i)
  {
    int y = (int)(ground->points[i].height/(1.0+ground->points[i].z));
    if (y <= -HEIGHT/2 || y > HEIGHT/2)
    {
      continue;
    }
    ground->raster[HEIGHT/2-y].z = ground->points[i].z;
    ground->raster[HEIGHT/2-y].height = ground->points[i].height;
  }
}

//------------------------------------------------------------------------------

void
vb_ground_render(VB_View *view, VB_Ground *ground)
{
  SDL_Rect src = {0, 0, 256, 1};
  SDL_Rect dst = {0, 0, 256, 1};

  for (int y = 0; y < HEIGHT; ++y)
  {
    if (ground->raster[y].z <= 0.0 || ground->raster[y].z >= 10000.0)
    {
      continue;
    }
    dst.w = 100000 / ground->raster[y].z;
    src.x = 0;
    src.y = (int)ground->raster[y].z % 256;
    dst.x = WIDTH/2-dst.w/2.0;
    dst.y = y;
    SDL_RenderCopy(view->renderer, ground->tex, &src, &dst);
  }
}

//==============================================================================
