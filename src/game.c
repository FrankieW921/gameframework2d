#include <SDL.h>
#include "simple_logger.h"

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "gfc_input.h""

#include "entity.h"
#include "player.h"


int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;
    Sprite *sprite;
    Entity *player;
    
    int mx,my;
    float mousef = 0;
    Sprite *mouse;
    GFC_Color mouseGFC_Color = gfc_color8(255,255,255,255);
    entity_system_init(1024);
    
    
    /*program initializtion*/
    init_logger("gf2d.log",0);
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        1200,
        720,
        1200,
        720,
        gfc_vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
    SDL_ShowCursor(SDL_DISABLE);
    
    /*demo setup*/
    sprite = gf2d_sprite_load_image("images/backgrounds/SMTtokyo.png");
    mouse = gf2d_sprite_load_all("images/smtDemonIcon.png",-1,-1,1,0);
    //mouse = gf2d_sprite_load_image("images/smtDemonIcon.png");
    slog("press [escape] to quit");
    
    player = player_new_entity(gfc_vector2d(20, 20)); 

    /*main game loop*/
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        SDL_GetMouseState(&mx,&my);
        
        /*mousef += 0.1;
        if (mousef >= 16.0)mousef = 0;
        */
        
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
            gf2d_sprite_draw_image(sprite,gfc_vector2d(0,0));

            entity_system_draw_all();
            entity_system_think_all();
            
            //UI elements last
            gf2d_sprite_draw(
                mouse,
                gfc_vector2d(mx,my),
                NULL,
                NULL,
                NULL,
                NULL,
                &mouseGFC_Color,
                (int)mousef);

        gf2d_graphics_next_frame();// render current draw frame and skip to the next frame



        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    entity_free(player);
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
