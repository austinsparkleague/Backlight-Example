#include <pebble.h>

/* Sometime you need to be able to manually control the backlight on the pebble, maybe your making a flashlight
app, or a metronome that uses the backlight, whatever the case you are going to have to learn how to control the 
backlight! This app has a few examples of turning the backlight on and off using button presses. */

//create our window and text layer before we use them
static Window *window;
static TextLayer *text_layer;

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  //this function will turn the backlight on for a short period of time, and then off again. This is the default behavior for the pebble anytime a button is pressed.
  light_enable_interaction();
  
  //Change our text layer to dispaly "On for a moment"
  text_layer_set_text(text_layer, "On for a moment");
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  //this function will enable the backlight until it is turned off again manually
  light_enable(true);
  
  //change out text layer to say "On until turned off"
  text_layer_set_text(text_layer, "On until turned off");
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  //this function will put the backlight back into automatic control so it will turn off automatically
  light_enable(false);
  
  //change out text layer to say "Off after a moment"
  text_layer_set_text(text_layer, "Off after a moment");
}

static void click_config_provider(void *context) {
  
  //register handlers for each of the button presses
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void window_load(Window *window) {
   //create a main window and give it the bounds of the screen
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  //set up our text layer
  text_layer = text_layer_create(GRect(0, 72, bounds.size.w, 20));
  //set the default text of our text layer
  text_layer_set_text(text_layer, "Press a button");
  //change our text layers alignment
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  //finally add it to our main window so the user can see it
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

static void window_unload(Window *window) {
   // when our main window is destroyed, we need to remove all of the things on it when we destroy it
  text_layer_destroy(text_layer);
}

static void init(void) {
  //when we first initialize we need to do some setup, first create a window
  window = window_create();
  //then we set its click providers to be the ones we set up on line 39 - 45
  window_set_click_config_provider(window, click_config_provider);
  //then set up the window handlers for loading and unloading
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  //then we push the window onto the screen and tell it to animate on
  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit(void) {
  // when our app stops we have to destroy all our windows
  window_destroy(window);
}

int main(void) {
  // this is our main loop, we want to first initialize everything
  init();

  //then print to the console so we can see the app has started
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  //then our main loop runs which handles all of our uer interface, vibrations, and button presses
  app_event_loop();
  
  //finally when the app is done, destroy everything! Muhaha
  deinit();
}