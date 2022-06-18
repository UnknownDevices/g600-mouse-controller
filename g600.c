#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <linux/input.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct input_event events[64];
const char kDir[] = "/dev/input/by-id/";
const char kPrefix[] = "usb-Logitech_Gaming_Mouse_G600_";
const char kSuffix[] = "-if01-event-kbd";

enum {
  G9 = 30,
  G10 = 31,
  G11 = 32,
  G12 = 33,
  G13 = 34,
  G14 = 35,
  G15 = 36,
  G16 = 37,
  G17 = 38,
  G18 = 39,
  G19 = 45,
  G20 = 46,
  HWHEEL_LEFT = 4,
  HWHEEL_RIGHT = 7,
  MIDDLE_BUTTON = 11,
  G_SHIFT_BUTTON = 55,
  LAST_BUTTON = 55,
};

// ADD KEY->COMMAND MAPPINGS HERE:
const char *on_down_cmds[2][LAST_BUTTON + 1] = {
    //[g-shift][scancode] = "command to run",
    [0][G9] = "xdotool keydown 1",
    [0][G10] = "xdotool keydown 2",
    [0][G11] = "xdotool keydown 3",
    [0][G12] = "xdotool keydown 4",
    [0][G13] = "xdotool keydown 5",
    [0][G14] = "xdotool keydown 6",
    [0][G15] = "xdotool keydown 7",
    [0][G16] = "xdotool keydown 8",
    [0][G17] = "xdotool keydown 9",
    [0][G18] = "xdotool keydown 0",
    [0][G19] = "xdotool keydown minus",
    [0][G20] = "xdotool keydown equal",
    [0][HWHEEL_LEFT] = "xdotool keydown Alt+Left",
    [0][HWHEEL_RIGHT] = "xdotool keydown Alt+Right",
    [1][G9] = "xdotool keydown Home",
    [1][G10] = "xdotool keydown Up",
    [1][G11] = "xdotool keydown End",
    [1][G12] = "xdotool keydown Left",
    [1][G13] = "xdotool keydown Down",
    [1][G14] = "xdotool keydown Right",
    [1][G15] = "xdotool keydown Page_Up",
    [1][G16] = "xdotool keydown BackSpace",
    [1][G17] = "xdotool keydown Page_Down",
    // [1][G18] = "",
    [1][G19] = "xdotool keydown Delete",
    // [1][G20] = "",
    [1][HWHEEL_LEFT] = "xdotool keydown Ctrl+z",
    [1][HWHEEL_RIGHT] = "xdotool keydown Ctrl+y"};

const char *on_up_cmds[2][LAST_BUTTON + 1] = {
    //[g-shift][scancode] = "command to run",
    [0][G9] = "xdotool keyup 1",
    [0][G10] = "xdotool keyup 2",
    [0][G11] = "xdotool keyup 3",
    [0][G12] = "xdotool keyup 4",
    [0][G13] = "xdotool keyup 5",
    [0][G14] = "xdotool keyup 6",
    [0][G15] = "xdotool keyup 7",
    [0][G16] = "xdotool keyup 8",
    [0][G17] = "xdotool keyup 9",
    [0][G18] = "xdotool keyup 0",
    [0][G19] = "xdotool keyup minus",
    [0][G20] = "xdotool keyup equal",
    [0][HWHEEL_LEFT] = "xdotool keyup Alt+Left",
    [0][HWHEEL_RIGHT] = "xdotool keyup Alt+Right",
    [1][G9] = "xdotool keyup Home",
    [1][G10] = "xdotool keyup Up",
    [1][G11] = "xdotool keyup End",
    [1][G12] = "xdotool keyup Left",
    [1][G13] = "xdotool keyup Down",
    [1][G14] = "xdotool keyup Right",
    [1][G15] = "xdotool keyup Page_Up",
    [1][G16] = "xdotool keyup BackSpace",
    [1][G17] = "xdotool keyup Page_Down",
    // [1][G18] = "",
    [1][G19] = "xdotool keyup Delete",
    // [1][G20] = "",
    [1][HWHEEL_LEFT] = "xdotool keyup Ctrl+z",
    [1][HWHEEL_RIGHT] = "xdotool keyup Ctrl+y"};

int starts_with(const char *haystack, const char *prefix) {
  size_t prefix_length = strlen(prefix), haystack_length = strlen(haystack);
  if (haystack_length < prefix_length)
    return 0;
  return strncmp(prefix, haystack, prefix_length) == 0;
}

int ends_with(const char *haystack, const char *suffix) {
  size_t suffix_length = strlen(suffix), haystack_length = strlen(haystack);
  if (haystack_length < suffix_length)
    return 0;
  size_t haystack_end = haystack + haystack_length - suffix_length;
  return strncmp(suffix, haystack_end, suffix_length) == 0;
}

// Returns non-0 on error.
int find_g600(char *path) {
  //*path = kDir;
  DIR *dir;
  struct dirent *ent;
  if (!(dir = opendir(kDir))) {
    return 1;
  }
  while ((ent = readdir(dir))) {
    if (starts_with(ent->d_name, kPrefix) && ends_with(ent->d_name, kSuffix)) {
      strcpy(path, kDir);
      strcat(path, ent->d_name);

      printf("full path is %s\n", path);

      //*path += ent->d_name;
      closedir(dir);
      return 0;
    }
  }
  closedir(dir);
  return 2;
}

int main() {
  printf("Starting G600 Linux controller.\n\n");
  printf("It's a good idea to configure G600 with Logitech Gaming Software "
         "before running this program:\n");
  printf(" - assign left, right, middle mouse button and vertical mouse wheel"
         "to their normal functions\n");
  printf(" - assign the G-Shift button to \"G-Shift\"\n");
  printf(" - assign all other keys (including horizontal mouse wheel) to "
         "arbitrary (unique) keyboard keys\n");
  printf("\n");
  char path[1024];
  int find_error = find_g600(&path);
  if (find_error) {
    printf("Error: Couldn't find G600 input device.\n");
    switch (find_error) {
    case 1:
      printf(
          "Suggestion: Maybe the expected directory (%s) is wrong. Check "
          "whether this directory exists and fix it by editing \"g600.c\".\n",
          kDir);
      break;
    case 2:
      printf("Suggestion: Maybe the expected device prefix (%s) is wrong. "
             "Check whether a device with this prefix exists in %s and fix it "
             "by editing \"g600.c\".\n",
             kPrefix, kDir);
      break;
    }
    printf("Suggestion: Maybe a permission is missing. Try running this "
           "program with sudo.\n");
    return 1;
  }
  int fd = open(path, O_RDONLY);
  if (fd < 0) {
    printf("Error: Couldn't open \"%s\" for reading.\n", path);
    printf("Reason: %s.\n", strerror(errno));
    printf("Suggestion: Maybe a permission is missing. Try running this "
           "program with sudo.\n");
    return 1;
  }

  ioctl(fd, EVIOCGRAB, 1);
  printf("G600 controller started successfully.\n\n");

  int g_shift = 0;
  int prev_scancode = 0;
  int prev_pressed = 0;
  while (1) {
    size_t n = read(fd, events, sizeof(events));
    if (n <= 0)
      return 2;
    if (n < sizeof(struct input_event) * 2)
      continue;
    if (events[0].type != 4)
      continue;
    if (events[0].code != 4)
      continue;
    if (events[1].type != 1)
      continue;
    const int pressed = events[1].value;
    const int scancode = events[0].value & ~0x70000;

    if (scancode == 55) {
      printf("\n");
      printf("g-shift %s\n", pressed ? "on" : "off");

      if (prev_pressed) {
        if (on_up_cmds[g_shift][prev_scancode]) {
          printf("Executing: \"%s\"\n", on_up_cmds[g_shift][prev_scancode]);
          system(on_up_cmds[g_shift][prev_scancode]);
        }

        g_shift ^= 1;
        if (on_down_cmds[g_shift][prev_scancode]) {
          printf("Executing: \"%s\"\n", on_down_cmds[g_shift][prev_scancode]);
          system(on_down_cmds[g_shift][prev_scancode]);
        }
      } else {
        g_shift ^= 1;
      }
      continue;
    }
    prev_pressed = pressed;
    prev_scancode = scancode;

    const char *cmd = pressed ? on_down_cmds[g_shift][scancode]
                              : on_up_cmds[g_shift][scancode];
    if (cmd) {
      printf("\n");
      printf("Executing: \"%s\"\n", cmd);
      system(cmd);
    }
  }

  close(fd);
}