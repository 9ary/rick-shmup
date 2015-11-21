#include <string.h>
#include <SFML/Window.h>
#include "input.h"
#include "render.h"
#include "misc.h"

/*
 * Individual input element
 */
struct input_map
{
    sfKeyCode key; // Keyboard
    int button; // Controller
    int axis; // Analog controller
    float sign; // Direction of the axis
};

/*
 * Input buffers
 */
static float states_poll[input_num];
static float states_current[input_num];
static float states_previous[input_num];

/*
 * Controller to use
 */
static unsigned controller = 0;

/*
 * Actual input map
 * Hardcoded for now
 */
static struct input_map map[input_num] =
{
    { sfKeyReturn, -1, -1, 0 },
    { sfKeyBack, -1, -1, 0 },
    { sfKeyEscape, -1, -1, 0 },
    { sfKeyW, -1, -1, 0 },
    { sfKeyS, -1, -1, 0 },
    { sfKeyA, -1, -1, 0 },
    { sfKeyD, -1, -1, 0 }
};

void input_poll()
{
    for (input_t i = 0; i < input_num; i++)
    {
        if (map[i].key != sfKeyUnknown
            // Keyboard input depends on focus and the window existing
            && window && sfRenderWindow_hasFocus(window))
        {
            states_poll[i] = sfKeyboard_isKeyPressed(map[i].key);
            if (states_poll[i])
                continue;
        }

        if (map[i].button >= 0)
        {
            states_poll[i] =
                sfJoystick_isButtonPressed(controller, map[i].button);
            if (states_poll[i])
                continue;
        }

        if (map[i].axis >= 0)
        {
            states_poll[i] =
                min(sfJoystick_getAxisPosition(controller, map[i].axis) *
                        map[i].sign,
                    0);
            if (states_poll[i])
                continue;
        }

        states_poll[i] = 0;
    }
}

void input_copy()
{
    memcpy(states_previous, states_current, sizeof(float) * input_num);
    memcpy(states_current, states_poll, sizeof(float) * input_num);
}

int input_key_pressed(input_t key)
{
    if (states_current[key] && (states_previous[key] == 0))
        return 1;
    else
        return 0;
}

int input_key_held(input_t key)
{
    if (states_current[key])
        return 1;
    else
        return 0;
}

int input_key_released(input_t key)
{
    if ((states_current[key] == 0) && states_previous[key])
        return 1;
    else
        return 0;
}

float input_analog_value(input_t axis)
{
    return states_current[axis];
}

float input_analog_delta(input_t axis)
{
    return states_current[axis] - states_previous[axis];
}
