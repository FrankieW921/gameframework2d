#include <SDL.h>
#include <SDL_mixer.h>
#include "simple_logger.h"

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "gfc_input.h""
#include "gfc_vector.h"
#include "gfc_shape.h"
#include "gfc_audio.h"

#include "game.h"
#include "entity.h"
#include "player.h"
#include "enemy.h"
#include "world.h"
#include "camera.h"
#include "hud.h"
#include "particles.h"

static int gameActive = 0;

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;
    Sprite* sprite;
    
    int mx,my;
    float mousef = 0;
    Sprite *mouse;
    GFC_Color mouseGFC_Color = gfc_color8(255,255,255,255);

    entity_system_init(1024);
    particle_system_init(1024);
    
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
    SDL_Init(SDL_INIT_AUDIO);
    TTF_Init();
    camera_set_size(gfc_vector2d(1280, 720));
    
    //Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 8, 2048);

    gfc_audio_init(32, 8, 1, 1, true, false);
    Mix_VolumeMusic(24);
    

    /*demo setup*/
    sprite = gf2d_sprite_load_image("images/backgrounds/SMTtokyo.png");
    //mouse = gf2d_sprite_load_all("images/enemies/smtDemonIcon.png",-1,-1,1,0);
    mouse = gf2d_sprite_load_image("images/crosshair.png");
    slog("press [escape] to quit");
    main_menu();

    //set_current_world(world_load("maps/testWorld.json", 1));

    //start_game();
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
            player_draw(get_the_player());
            particle_draw_all();

            //UI elements last
            draw_all_huds();
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
        if (keys[SDL_SCANCODE_K]) main_menu(); 
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    world_free(get_current_world());
    entity_system_free_all();
    slog("---==== END ====---");
    return 0;
}

void start_game() {
    Entity* player;
    free_start_buttons();
    world_free(get_current_world());
    disable_start_buttons();

    player = player_new_entity(gfc_vector2d(150, 80));
    world_load("maps/room1.json", 0);
    world_setup_camera();
    init_huds();
}

void main_menu() {
    Mix_Music* puzzle_boy;
    world_free(get_current_world());
    entity_system_free_all();
    //free_huds();
    free_the_player();

    Mix_HaltMusic();
    puzzle_boy = Mix_LoadMUS("music/Puzzle_Boy.mp3");
    if (puzzle_boy) {
        Mix_PlayMusic(puzzle_boy, -1);
    }

    enable_start_buttons();
    set_current_world(world_load("maps/mainMenu.json", 0));
    world_setup_camera();

    init_start_buttons();
}

void start_edit() {
    Entity* player;
    free_start_buttons();
    disable_start_buttons();
    world_free(get_current_world());

    player = player_new_entity(gfc_vector2d(150, 80));
    enable_editor_mode();
    set_current_world(world_load("maps/mainMenu.json", 0));
    world_setup_camera();
}