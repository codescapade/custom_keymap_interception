#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <linux/input.h>
#include <sys/time.h>

// Calculate milisecond difference.
#define DUR_MILLIS(start, end) \
    ((end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000)

enum key_state {K_UP = 0, K_DOWN = 1, K_REPEAT = 2};

struct key_cfg {
  unsigned int key; // the default key.
  int space_key; // the key when holding space.
  bool capital; // send capital when holding key without space.
  bool space_capital; // send capital when holding key with space.
  bool space_shift; // always send capitla when holding space (for keys like { using shift + [)
  struct timeval time; // store how long a key has been held down.
};

// the key config map.
struct key_cfg keys[] = {
  // number row
  {
    .key = KEY_GRAVE,
    .space_key = -1,
    .capital = true
  },
  {
    .key = KEY_1,
    .space_key = KEY_F1,
    .capital = true
  },
  {
    .key = KEY_2,
    .space_key = KEY_F2,
    .capital = true
  },
  {
    .key = KEY_3,
    .space_key = KEY_F3,
    .capital = true
  },
  {
    .key = KEY_4,
    .space_key = KEY_F4,
    .capital = true
  },
  {
    .key = KEY_5,
    .space_key = KEY_F5,
    .capital = true
  },
  {
    .key = KEY_6,
    .space_key = KEY_F6,
    .capital = true
  },
  {
    .key = KEY_7,
    .space_key = KEY_F7,
    .capital = true
  },
  {
    .key = KEY_8,
    .space_key = KEY_F8,
    .capital = true
  },
  {
    .key = KEY_9,
    .space_key = KEY_F9,
    .capital = true
  },
  {
    .key = KEY_0,
    .space_key = KEY_F10,
    .capital = true
  },
  {
    .key = KEY_MINUS,
    .space_key = -1,
    .capital = true
  },
  {
    .key = KEY_EQUAL,
    .space_key = -1,
    .capital = true
  },
  // q row
  {
    .key = KEY_TAB,
    .space_key = KEY_BACKSPACE,
    .capital = false
  },
  {
    .key = KEY_Q,
    .space_key = KEY_1,
    .capital = true,
    .space_capital = true
  },
  {
    .key = KEY_W,
    .space_key = KEY_2,
    .capital = true,
    .space_capital = true
  },
  {
    .key = KEY_E,
    .space_key = KEY_3,
    .capital = true,
    .space_capital = true
  },
  {
    .key = KEY_R,
    .space_key = KEY_4,
    .capital = true,
    .space_capital = true
  },
  {
    .key = KEY_T,
    .space_key = KEY_5,
    .capital = true,
    .space_capital = true
  },
  {
    .key = KEY_Y,
    .space_key = KEY_6,
    .capital = true,
    .space_capital = true
  },
  {
    .key = KEY_U,
    .space_key = KEY_7,
    .capital = true,
    .space_capital = true
  },
  {
    .key = KEY_I,
    .space_key = KEY_8,
    .capital = true,
    .space_capital = true
  },
  {
    .key = KEY_O,
    .space_key = KEY_9,
    .capital = true,
    .space_capital = true
  },
  {
    .key = KEY_P,
    .space_key = KEY_0,
    .capital = true,
    .space_capital = true
  },
  {
    .key = KEY_LEFTBRACE,
    .space_key = -1,
    .capital = true
  },
  {
    .key = KEY_RIGHTBRACE,
    .space_key = -1,
    .capital = true
  },
  {
    .key = KEY_BACKSLASH,
    .space_key = -1,
    .capital = true
  },
  // a row
  {
    .key = KEY_A,
    .space_key = KEY_9,
    .capital = true,
    .space_shift = true
  },
  {
    .key = KEY_S,
    .space_key = KEY_0,
    .capital = true,
    .space_shift = true
  },
  {
    .key = KEY_D,
    .space_key = KEY_LEFTBRACE,
    .capital = true,
    .space_shift = true
  },
  {
    .key = KEY_F,
    .space_key = KEY_RIGHTBRACE,
    .capital = true,
    .space_shift = true
  },
  {
    .key = KEY_G,
    .space_key = KEY_GRAVE,
    .capital = true,
    .space_capital = true
  },
  {
    .key = KEY_H,
    .space_key = KEY_LEFT,
    .capital = true
  },
  {
    .key = KEY_J,
    .space_key = KEY_DOWN,
    .capital = true
  },
  {
    .key = KEY_K,
    .space_key = KEY_UP,
    .capital = true
  },
  {
    .key = KEY_L,
    .space_key = KEY_RIGHT,
    .capital = true
  },
  {
    .key = KEY_SEMICOLON,
    .space_key = KEY_MINUS,
    .capital = true,
    .space_capital = true
  },
  {
    .key = KEY_APOSTROPHE,
    .space_key = KEY_EQUAL,
    .capital = true,
    .space_capital = true
  },
  // z row
  {
    .key = KEY_LEFTSHIFT,
    .space_key = KEY_ENTER
  },
  {
    .key = KEY_Z,
    .space_key = -1,
    .capital = true
  },
  {
    .key = KEY_X,
    .space_key = -1,
    .capital = true
  },
  {
    .key = KEY_C,
    .space_key = KEY_LEFTBRACE,
    .capital = true
  },
  {
    .key = KEY_V,
    .space_key = KEY_RIGHTBRACE,
    .capital = true
  },
  {
    .key = KEY_B,
    .space_key = -1,
    .capital = true
  },
  {
    .key = KEY_N,
    .space_key = -1,
    .capital = true
  },
  {
    .key = KEY_M,
    .space_key = -1,
    .capital = true
  },
  {
    .key = KEY_COMMA,
    .space_key = -1,
    .capital = true
  },
  {
    .key = KEY_DOT,
    .space_key = -1,
    .capital = true
  },
  {
    .key = KEY_SLASH,
    .space_key = -1,
    .capital = true
  }
};

// 200 milliseconds until capitals are sent.
const int shift_time = 200;

// shift up and down input events send send it easy.
const struct input_event
  shift_up = {.type = EV_KEY, .code = KEY_LEFTSHIFT, .value = 0},
  shift_down = {.type = EV_KEY, .code = KEY_LEFTSHIFT, .value = 1};

// Read an input even.
int read_event(struct input_event *event) {
  return fread(event, sizeof(struct input_event), 1, stdin) == 1;
}

// Write an input event.
void write_event(const struct input_event *event) {
  if (fwrite(event, sizeof(struct input_event), 1, stdout) != 1) {
    exit(EXIT_FAILURE);
  }
}

// Send a key. Have to send down event first and then up event. Same for shift is used.
void send_key(struct input_event *event, unsigned int key, bool use_shift) {
  if (use_shift) {
    write_event(&shift_down);
  }
  event->code = key;
  event->value = K_DOWN;
  write_event(event);

  event->value = K_UP;
  write_event(event);
  if (use_shift) {
    write_event(&shift_up);
  }
}

void handle_key(struct input_event *event, struct key_cfg *cfg, bool space_down) {
  if (event->value == K_DOWN) {
    // reset the time the key is down.
    cfg->time = event->time;
  } else if (event->value == K_UP) {
    // check if the key had been down long enough to send a key with shift.
    bool use_shift = DUR_MILLIS(cfg->time, event->time) > shift_time;

    // Check if the space bar is down and if the config has a space key.
    if (cfg->space_key > 0 && space_down) {
      send_key(event, cfg->space_key, (cfg->space_capital && use_shift) || cfg->space_shift);
    } else {
      // Send the normal key.
      send_key(event, cfg->key, use_shift);
    }
  }
}

int main(void) {
  struct input_event event;

  setbuf(stdin, NULL), setbuf(stdout, NULL);

  bool space_down = false;
  bool space_used = false;

  int cfg_size = sizeof(keys) / sizeof(struct key_cfg);

  // Loop through all key events.
  while (read_event(&event)) {
    bool handled = false;

    if (event.type == EV_MSC && event.code == MSC_SCAN) {
      continue;
    }

    // ignore event)that are not keyboard events.
    if (event.type != EV_KEY) {
      write_event(&event);
      continue;
    }
    
    // track space down.
    if (event.code == KEY_SPACE) {
      if (event.value == K_DOWN) {
        space_down = true;
        space_used = false;
      } else if (event.value == K_UP) {
        space_down = false;
        // if no other key was pressed send space key.
        if (!space_used) {
          send_key(&event, KEY_SPACE, 0);
        }
        space_used = false;
      }
      continue;
    }
    
    // Check if a key in the config array was pressed.
    for (int i = 0; i < cfg_size; i++) {
      if (keys[i].key == event.code) {
        space_used = true;
        handle_key(&event, &keys[i], space_down);
        handled = true;
        break;
      }
    }

    // If the key was handled above don't send the normal key event.
    if (handled) {
      continue;
    }

    write_event(&event);
  }

  return 0;
}
