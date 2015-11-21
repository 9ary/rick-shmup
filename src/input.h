typedef enum
{
    input_pri,
    input_sec,
    input_menu,
    input_up,
    input_down,
    input_left,
    input_right,
    input_num // Number of keys, keep at the end
} input_t;

/*
 * Poll input from the operating system
 */
void input_poll();

/*
 * Copy current buffer to previous buffer and poll buffer to current
 */
void input_copy();

/*
 * True when the key was just pressed
 */
int input_key_pressed(input_t key);

/*
 * True as long as the key is down
 */
int input_key_held(input_t key);

/*
 * True when the key was just released
 */
int input_key_released(input_t key);

/*
 * Absolute value of the axis
 */
float input_analog_value(input_t axis);

/*
 * Relative value of the axis compared to the previous value
 */
float input_analog_delta(input_t axis);
