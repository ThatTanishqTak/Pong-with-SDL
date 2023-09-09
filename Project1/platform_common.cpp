// Define button identifiers using enum
enum { BUTTON_UP, BUTTON_DOWN, BUTTON_W, BUTTON_S, BUTTON_COUNT };

// Define a structure 'Button_State' to represent the state of a button
struct Button_State
{
    bool is_down;   // Flag indicating whether the button is currently pressed
    bool changed;   // Flag indicating whether the button state has changed
};

// Define an 'Input' structure to hold the state of multiple buttons
struct Input { Button_State buttons[BUTTON_COUNT]; }; // An array of Button_State structs for each button