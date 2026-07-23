#include "controller.h"

void controller_init(Controller* controller) {
	controller->mouse_button_left = 0;
	controller->mouse_button_right = 0;
	for (int i = 0; i < 128; i++) {
		controller->keys_pressed[i] = 0;
	}
}
