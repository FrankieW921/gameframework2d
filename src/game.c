#include <SDL.h>
#include "simple_logger.h"

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "gfc_input.h""
#include "gfc_vector.h"
#include "gfc_shape.h"

#include "entity.h"
#include "player.h"
#include "enemy.h"
#include "world.h"
#include "camera.h"
#include "hud.h"


int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;
    Sprite* sprite;
    Entity *player;
    Hud* theHud;
    //Entity* enemy;

    //World *testWorld;
    
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
        1280,
        720,
        1280,
        720,
        gfc_vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
    gfc_input_init("gfc/sample_config/input.cfg");
    SDL_ShowCursor(SDL_DISABLE);
    camera_set_size(gfc_vector2d(1280, 720));
    
    /*demo setup*/
    sprite = gf2d_sprite_load_image("images/backgrounds/SMTtokyo.png");
    //mouse = gf2d_sprite_load_all("images/enemies/smtDemonIcon.png",-1,-1,1,0);
    mouse = gf2d_sprite_load_image("images/crosshair.png");
    slog("press [escape] to quit");
    //testWorld = world_test_new();
    //testWorld = world_load("maps/testWorld.json");
    set_current_world(world_load("maps/testWorld.json"));
    world_setup_camera();
    
    player = player_new_entity(gfc_vector2d(80, 80)); 
    theHud = new_hud(player);
    //enemy = enemy_new_entity(gfc_vector2d(200, 200));

    /*main game loop*/
    while(!done)
    {
        gfc_input_update();
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        SDL_GetMouseState(&mx,&my);
        
        /*mousef += 0.1;
        if (mousef >= 16.0)mousef = 0;
        */
        
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
            //gf2d_sprite_draw_image(sprite,gfc_vector2d(0,0));
            
            
            world_draw(get_current_world());

            entity_system_think_all();
            entity_system_update_all();
            entity_system_draw_all();
            
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
            draw_hud(theHud);

        gf2d_graphics_next_frame();// render current draw frame and skip to the next frame



        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    //entity_free(player);
    //entity_free(enemy);
    world_free(get_current_world());
    entity_system_free_all();
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
