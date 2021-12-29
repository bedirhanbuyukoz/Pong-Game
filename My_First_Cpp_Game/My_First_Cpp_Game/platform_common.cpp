struct Button_State { //Yýðýnlar
	bool is_down;
	bool changed;
};

enum {              //Sýralama, Importlar
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_W,
	BUTTON_S,
	BUTTON_LEFT,
	BUTTON_RIGHT,
	BUTTON_ENTER, 

	BUTTON_COUNT, // Son seçenek olmalý
};

struct Input { //Buton Tanýmlama
	Button_State buttons[BUTTON_COUNT];
};