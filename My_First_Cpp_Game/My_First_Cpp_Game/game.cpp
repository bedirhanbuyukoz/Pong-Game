//Oyun Ýçi 


#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)

float player_1_p, player_1_dp, player_2_p, player_2_dp;//Player ve Pointerlarý 
float arena_half_size_x = 85, arena_half_size_y = 45;//Arena Import
float player_half_size_x = 2.5, player_half_size_y = 12;//Player Import
float ball_p_x, ball_p_y, ball_dp_x = 130, ball_dp_y, ball_half_size = 1; //Top Import ve  Top Hýzý

int player_1_score, player_2_score;//Skorlar import

internal void //Oyuncu Pointer
simulate_player(float *p, float *dp, float ddp, float dt) {
	ddp -= *dp * 10.f;

	*p = *p + *dp * dt + ddp * dt * dt * .5f;
	*dp = *dp + ddp * dt;

	if (*p + player_half_size_y > arena_half_size_y) {
		*p = arena_half_size_y - player_half_size_y;
		*dp = 0;
	}
	else if (*p - player_half_size_y < -arena_half_size_y) {
		*p = -arena_half_size_y + player_half_size_y;
		*dp = 0;
	}
}

internal bool
aabb_vs_aabb(float p1x, float p1y, float hs1x, float hs1y,
	float p2x, float p2y, float hs2x, float hs2y) {
	return (p1x + hs1x > p2x - hs2x &&
		p1x - hs1x < p2x + hs2x &&
		p1y + hs1y > p2y - hs2y &&
		p1y + hs1y < p2y + hs2y);
}

enum Gamemode { //Oyun Menusu ve Mod 
	GM_MENU,
	GM_GAMEPLAY,
};

Gamemode current_gamemode;//Anlýk Oyun Modu 
int hot_button;//Game Modu Seçme
bool enemy_is_ai;//Yapay Zeka Seçme (Oyun Menusunden)

internal void
simulate_game(Input* input, float dt) {
	draw_rect(0, 0, arena_half_size_x, arena_half_size_y, 0x489338);//Saha Rengi Yeþil ve  R.Arenayý geçmemesi "half size"
	draw_arena_borders(arena_half_size_x, arena_half_size_y, 0x507f54);//Çerçeve Rengi Soluk Yeþil R.Arenayý geçmemesi "half size"

	 //Yapay Zeka 
	if (current_gamemode == GM_GAMEPLAY) {
		float player_1_ddp = 0.f;//Hareket Hýzý 1.oyuncu
		if (!enemy_is_ai) { //Single Player PC enemy
			if (is_down(BUTTON_UP)) player_1_ddp += 2000;
			if (is_down(BUTTON_DOWN)) player_1_ddp -= 2000;
		} else {
			

			//Avengers Yapay Zeka Level Katmaný Deactive
			//if (ball_p_y > player_1_p+2.f) player_1_ddp += 2000;
			//if (ball_p_y < player_1_p-2.f) player_1_ddp -= 2000;
			
			//Yapay Zeka 
			player_1_ddp = (ball_p_y - player_1_p) * 100; //Topa Yakýnsama H.
			if (player_1_ddp > 1300) player_1_ddp = 1300;
			if (player_1_ddp < -1300) player_1_ddp = -1300;
		}

		float player_2_ddp = 0.f;
		if (is_down(BUTTON_W)) player_2_ddp += 2000;//Sol oyuncu up
		if (is_down(BUTTON_S)) player_2_ddp -= 2000;//Sol oyuncu down h.

		simulate_player(&player_1_p, &player_1_dp, player_1_ddp, dt);
		simulate_player(&player_2_p, &player_2_dp, player_2_ddp, dt);


		// Top Simülasyonu  
		{
			ball_p_x += ball_dp_x * dt; //Top Hareketleri 
			ball_p_y += ball_dp_y * dt;
			 
			//Topun Fiziði
			if (aabb_vs_aabb(ball_p_x, ball_p_y, ball_half_size, ball_half_size, 80, player_1_p, player_half_size_x, player_half_size_y)) {
				ball_p_x = 80 - player_half_size_x - ball_half_size; 
				ball_dp_x *= -1; 
				ball_dp_y = (ball_p_y - player_1_p) * 2 + player_1_dp * .75f;
			} else if (aabb_vs_aabb(ball_p_x, ball_p_y, ball_half_size, ball_half_size, -80, player_2_p, player_half_size_x, player_half_size_y)) {
				ball_p_x = -80 + player_half_size_x + ball_half_size;
				ball_dp_x *= -1;
				ball_dp_y = (ball_p_y - player_2_p) * 2 + player_2_dp * .75f;
			}

			if (ball_p_y + ball_half_size > arena_half_size_y) { //Topu karþýlamasý iflerle Saðlanmýþtýr Sol Oyuncu
				ball_p_y = arena_half_size_y - ball_half_size;
				ball_dp_y *= -1;
			} else if (ball_p_y - ball_half_size < -arena_half_size_y) {
				ball_p_y = -arena_half_size_y + ball_half_size;
				ball_dp_y *= -1;
			}

			if (ball_p_x + ball_half_size > arena_half_size_x) { //Topu karþýlamasý iflerle Saðlanmýþtýr Sað Oyuncu
				ball_dp_x *= -1;
				ball_dp_y = 0;
				ball_p_x = 0;
				ball_p_y = 0;
				player_1_score++;//skor 1.Player +
			} else if (ball_p_x - ball_half_size < -arena_half_size_x) {
				ball_dp_x *= -1;
				ball_dp_y = 0;
				ball_p_x = 0;
				ball_p_y = 0;
				player_2_score++;//skor 2.player +
			}
		}

		draw_number(player_1_score, -10, 40, 1.f, 0xff0000);//Saðdaki Oyuncu Skoru G.Kýrmýzý
		draw_number(player_2_score, 10, 40, 1.f, 0xFFFF00);//Soldaki Oyuncu Skoru G. Sarý

		// Rendering
		draw_rect(ball_p_x, ball_p_y, ball_half_size, ball_half_size, 0xffffff);//Top. ve Top Rengi Beyaz

		draw_rect(80, player_1_p, player_half_size_x, player_half_size_y, 0xFFFF00);//Sað oyuncu Raket Kýrmýzý
		draw_rect(-80, player_2_p, player_half_size_x, player_half_size_y, 0xff0000);//Sol oyuncu Raket Sarý

	}
	else {

		//Menu
		if (pressed(BUTTON_LEFT) || pressed(BUTTON_RIGHT)) { //Menu Sað Sol seçme (Single-Multi)
			hot_button = !hot_button;//Hot button
		}



		if (pressed(BUTTON_ENTER)) {
			current_gamemode = GM_GAMEPLAY; // Oyun Menusunden Yapay Zeka Seçme 
			enemy_is_ai = hot_button ? 0 : 1;
		}

		if (hot_button == 0) {
			draw_text("SINGLEPLAYER", -80, -10, 1, 0xff0000);// Tek Oyunculu Yazýsý hot button Kýrmýzý Renk
			draw_text("MULTIPLAYER", 20, -10, 1, 0xaaaaaa);  // Çok Oyunculu Yazýsý  Gri Renk
		}
		else {
			draw_text("SINGLEPLAYER", -80, -10, 1, 0xaaaaaa);// Tek Oyunculu Yazýsý Gri Renk
			draw_text("MULTIPLAYER", 20, -10, 1, 0xff0000);// Çok Oyunculu Yazýsý hot button Kýrmýzý Renk
		}

		    draw_text("P", -30, 35, 2, 0xffff00);//Kýrmýzý
			draw_text("O", -15, 35, 2, 0xff0000);//Sarý
			draw_text("N", 2, 35, 2, 0xffff00);//Kýrmzý
			draw_text("G", 19, 35, 2, 0xff0000);//Sarý

			
			
	}
		  
		
		
		
	}
