/*
  GUI function implemetations. To use a different frontend,
  simply replace this file with a different one. Functions
  that have to be implemented:

  int gui_init()
  int gui_end()
  void gui_update_window(unsigned char *)
  void gui_wait(int)
*/

SDL_Window *window;
SDL_Surface* window_surface;
SDL_PixelFormat *window_pixel_format;

int gui_init()
  {
    printf("setting up GUI\n");

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
      {
	printf("ERROR: SDL could not be initialized.\n");
	return 0;
      }

    window = SDL_CreateWindow("gbc emu",10,10,DISPLAY_WIDTH,DISPLAY_HEIGHT,0);

    if (window == NULL)
      {
        printf("ERROR: Window could not be created.\n");
        return 0;
      }

    window_surface = SDL_GetWindowSurface(window);
    window_pixel_format = window_surface->format;

    if (window_pixel_format->BytesPerPixel != 4)
      {
        printf("ERROR: unsupported pixel format.");
        return 0;
      }

    return 1;
  }

void gui_update_window(unsigned char *screen_memory)
  {
    SDL_LockSurface(window_surface);

    for (uint i = 0; i < DISPLAY_PIXELS; i++)  // copy the pixels to the surface
      {
        // we suppose that the pixel format is 32 bit
        ((Uint32 *) window_surface->pixels)[i] =
        SDL_MapRGB(window_pixel_format,screen_memory[i],screen_memory[i],screen_memory[i]);
      }

    SDL_UpdateWindowSurface(window);

    SDL_UnlockSurface(window_surface);
  }

void gui_wait(int milliseconds)
  {
    SDL_Delay(milliseconds);
  }

int gui_end()
  {
    printf("destroying GUI\n");

    SDL_FreeSurface(window_surface);
    SDL_DestroyWindow(window);
    window = NULL;
    SDL_Quit();

    return 1;
  }
