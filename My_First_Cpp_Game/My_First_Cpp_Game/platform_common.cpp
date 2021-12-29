struct Button_State { //Y���nlar
	bool is_down;
	bool changed;
};

enum {              //S�ralama, Importlar
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_W,
	BUTTON_S,
	BUTTON_LEFT,
	BUTTON_RIGHT,
	BUTTON_ENTER, 

	BUTTON_COUNT, // Son se�enek olmal�
};

struct Input { //Buton Tan�mlama
	Button_State buttons[BUTTON_COUNT];
};