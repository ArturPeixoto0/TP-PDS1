#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define NUM_ENEMIES 15
#define TIRO_INATIVO 0
#define TIRO_ATIVO 1
#define TIRO_UPGRADE 2
#define RAIO_TIRO 100
#define TEMPO_TIRO 2.0
#define BORDA_TIRO 4
#define SCORE_PENALTY 0.3

#define ENEMY_R_MIN 10
#define ENEMY_R_MAX 30

#define VEL_MIN 1
#define VEL_MAX 2

#define QUANTIDADE_UPGRADES 1
#define TEMPO_UPGRADE 5
#define TAM_UP1 50


const float FPS = 100;

const int SCREEN_W = 960;
const int SCREEN_H = 540;
const int HERO_W = 40;
const int HERO_H = 40;
int ENEMY_W;
int ENEMY_H;
float decimais;

ALLEGRO_COLOR BKG_COLOR;
ALLEGRO_FONT *FONT_32;

        //Structs

typedef struct Tiro {
	int x, y;
	float raio;
	int modo;
	float timer;
	ALLEGRO_COLOR cor;
} Tiro;

typedef struct Ship {
	float x, y;
	float vel;
	ALLEGRO_COLOR cor;
	Tiro tiro;
} Ship;

typedef struct Emojis {

}Emojis;

typedef struct Hero {
	Ship ship;
	int dir_x;
	int dir_y;
	float score;
} Hero;

typedef struct Enemy {
	Ship ship;
	int active;
	int r;
	Emojis emojis;

} Enemy;

typedef struct Upgrade {
    Ship ship;
    int active;
    float timer;
} Upgrade;

//Funcoes:

        //Tiro

void initTiro(Ship *s) {

	//printf("\nInit tiro!");

	s->tiro.x = s->x;
	s->tiro.y = s->y;
	s->tiro.raio = 3;
	//s->tiro.cor = s->cor;
	s->tiro.timer = TEMPO_TIRO;
	s->tiro.modo = TIRO_INATIVO;

}
        //Hero

void initHero(Hero *s) {

	s->score = 0;
	s->ship.cor = al_map_rgb(100 + rand()%156, 100 + rand()%156, 100 + rand()%156);
	s->ship.x = SCREEN_W/2;
	s->ship.y = SCREEN_H - HERO_H - 10;
	s->ship.vel = 2;
	s->dir_x = 0;
	s->dir_y = 0;
	initTiro(&s->ship);

}

void drawHero(Hero s, ALLEGRO_BITMAP *jogador[], ALLEGRO_BITMAP *tiro[], Upgrade *u) {

    char score_txt[16];
	sprintf(score_txt, "%d", (int)s.score);
	//imprime o texto armazenado em my_text na posicao x=10,y=10 e com a cor rgb(128,200,30)
        al_draw_text(FONT_32, s.ship.cor, 100, 20, 0, score_txt);

    if (u->active == 0){
        al_draw_scaled_bitmap(jogador[1], 0, 0, al_get_bitmap_width(jogador[1]), al_get_bitmap_height(jogador[1]), s.ship.x - (HERO_W/2), s.ship.y, HERO_W, HERO_H, 0);
        al_draw_scaled_bitmap(tiro[1], 0, 0, al_get_bitmap_width(tiro[1]), al_get_bitmap_height(tiro[1]), s.ship.tiro.x - s.ship.tiro.raio, s.ship.tiro.y - s.ship.tiro.raio , 2*s.ship.tiro.raio, 2*s.ship.tiro.raio, 0);
    }

    else {
        al_draw_scaled_bitmap(jogador[0], 0, 0, al_get_bitmap_width(jogador[0]), al_get_bitmap_height(jogador[0]), s.ship.x - (HERO_W/2), s.ship.y, HERO_W, HERO_H, 0);
        //al_draw_filled_triangle(s.ship.x, s.ship.y, s.ship.x - HERO_W/2, s.ship.y + HERO_H, s.ship.x + HERO_W/2, s.ship.y + HERO_H, s.ship.cor);
         al_draw_scaled_bitmap(tiro[0], 0, 0, al_get_bitmap_width(tiro[0]), al_get_bitmap_height(tiro[0]), s.ship.tiro.x - s.ship.tiro.raio, s.ship.tiro.y - s.ship.tiro.raio , 2*s.ship.tiro.raio, 2*s.ship.tiro.raio, 0);
        //al_draw_circle(s.ship.tiro.x, s.ship.tiro.y, s.ship.tiro.raio, s.ship.tiro.cor, BORDA_TIRO);
    }
}

void updateHero(Hero *s) {


	s->ship.x += s->dir_x * s->ship.vel;

	if (s->ship.x <= 0 + (HERO_W/2)){
        s->ship.x = 0 + (HERO_W/2);
    }

    if (s->ship.x >= SCREEN_W - (HERO_W/2)){
        s->ship.x = SCREEN_W - (HERO_W/2);
    }

	s->ship.y += s->dir_y * s->ship.vel;

    if (s->ship.y <= 0){
        s->ship.y = 0;
    }

    if (s->ship.y >= SCREEN_H - HERO_H){
        s->ship.y = SCREEN_H - HERO_H;
    }

	if(s->ship.tiro.modo == TIRO_INATIVO) {
		s->ship.tiro.x = s->ship.x;
		s->ship.tiro.y = s->ship.y;
	}
	else if(s->ship.tiro.modo == TIRO_ATIVO) {
		if(s->ship.tiro.timer > 0)
			s->ship.tiro.timer -= 1.0/FPS;
		else
			initTiro(&s->ship);
	}
	else { //s->ship.tiro.modo == TIRO_UPGRADE
        s->ship.tiro.x = s->ship.x;
		s->ship.tiro.y = s->ship.y;
	}
}

        //Enemy

//Inicia os Inimigos em ondas (vetores)
void initEnemy(Enemy *e) { //iniciliza o inimigo (init = initialize)

        e->active = 1;
        e->ship.tiro.modo = TIRO_INATIVO;
        e->ship.tiro.timer = TEMPO_TIRO;

        e->ship.cor = al_map_rgb(100 + rand()%156, 100 + rand()%156, 100 + rand()%156);
        e->r = ENEMY_R_MIN + (rand()%(ENEMY_R_MAX + 1));
        e->ship.x = rand()%(SCREEN_W+1);
        e->ship.y = -1*(e->r) + (-1*(rand()%(2*(e->r)+1))); //vai spawnar entre 1 raio e 2 raios fora da tela

        decimais = 0 + rand()%(100);
        e->ship.vel = (VEL_MIN + rand()%(VEL_MAX+1)) + decimais/100;

        e->ship.tiro.x = e->ship.x;
        e->ship.tiro.y = e->ship.y;
}


void drawEnemy(Enemy *e, ALLEGRO_BITMAP *inimigo[], int pontos){
    if (e->active == 1){
        if (pontos < 3000){
            al_draw_scaled_bitmap(inimigo[0], 0, 0, al_get_bitmap_width(inimigo[0]), al_get_bitmap_height(inimigo[0]), e->ship.x - e->r, e->ship.y - e->r , 2*e->r, 2*e->r, 0);
        }
        else if (pontos < 7500){
            al_draw_scaled_bitmap(inimigo[1], 0, 0, al_get_bitmap_width(inimigo[1]), al_get_bitmap_height(inimigo[1]), e->ship.x - e->r, e->ship.y - e->r , 2*e->r, 2*e->r, 0);
        }
        else if (pontos < 12500){
            al_draw_scaled_bitmap(inimigo[2], 0, 0, al_get_bitmap_width(inimigo[2]), al_get_bitmap_height(inimigo[2]), e->ship.x - e->r, e->ship.y - e->r , 2*e->r, 2*e->r, 0);
        }
        else {
            al_draw_scaled_bitmap(inimigo[3], 0, 0, al_get_bitmap_width(inimigo[3]), al_get_bitmap_height(inimigo[3]), e->ship.x - e->r, e->ship.y - e->r , 2*e->r, 2*e->r, 0);
        }


    //al_draw_filled_circle(e->ship.x, e->ship.y, e->r, e->ship.cor);
    }
    if (e->active == 0 && e->ship.tiro.timer > 0 ){
        al_draw_circle(e->ship.x, e->ship.y, e->r, e->ship.cor, BORDA_TIRO);
    }
}

void updateEnemy(Enemy *e){
    if (e->active == 1) {
        e->ship.y += e->ship.vel;
        e->ship.tiro.y = e->ship.y; //define a posição y do tiro
        if (e->ship.y > SCREEN_H+(e->r)){
            e->active = -1;
        }
    }
    if (e->active == 0) { //MESMA lógica do tiro do hero

        if(e->ship.tiro.timer > 0){
			e->ship.tiro.timer -= 1.0/FPS;
			e->ship.tiro.y = e->ship.y; //define a posição y do tiro
			e->ship.tiro.raio = e->r;
        }
        else {
            e->ship.tiro.modo = TIRO_INATIVO; //desativa o tiro
			e->active = -1; //desativa o enemy
        }
    }
}

        //Upgrade

void initUpgrade (Upgrade *u){
    u->active = 1; //rand() % (QUANTIDADE_UPGRADES);
    u->ship.x = rand()%(SCREEN_W-TAM_UP1);
    u->ship.y = -TAM_UP1;
    decimais = 0 + rand()%(100);
    u->ship.vel = (VEL_MIN + rand()%(VEL_MAX+1)) + decimais/100;
    u->timer = TEMPO_UPGRADE;
}

void drawUpgrade (Upgrade *u, ALLEGRO_BITMAP *powerup){
    if (u->active == 1){
        al_draw_scaled_bitmap(powerup, 0, 0, al_get_bitmap_width(powerup), al_get_bitmap_height(powerup), u->ship.x, u->ship.y, TAM_UP1, TAM_UP1, 0);
        //al_draw_filled_rectangle(u->ship.x, u->ship.y, u->ship.x+TAM_UP1, u->ship.y+TAM_UP1, cor);
    }

}

void updateUpgrade (Upgrade *u, Hero *h){
    if (u->active > 0){
        u->ship.y += u->ship.vel;
        if (u->ship.y > SCREEN_H+TAM_UP1){
            u->active = -1;
        }
    }
    if (u->active == 0) { //o upgrade foi pego
        if(u->timer > 0){
			u->timer -= 1.0/FPS;
        }
        else {
            u->active = -1;
            initTiro(&h->ship);
        }
    }
}



        //Colisao

float distancia (float Hx, float Hy, float Ex, float Ey){
    return sqrt(pow(Hx-Ex, 2)+pow(Hy-Ey, 2));
}

float distanciaPontoMaisPerto (float V1x, float V1y, float V2x, float V2y, float Cx, float Cy){
    float TamanhoL  = distancia (V1x, V1y, V2x, V2y);
    float Lx = V2x - V1x; //coord X do vetor Lado (de V1 para V2)
    float Ly = V2y - V1y; //coord Y do vetor Lado (de V1 para V2)
    float Dx = Cx-V1x; //coord X do vetor Distancia (de V1 para centro do cículo)
    float Dy = Cy-V1y; //coord Y do vetor Distancia (de V1 para centro do cículo)

    float projecao = ((Lx*Dx)+(Ly*Dy))/(pow(Lx, 2)+pow(Ly, 2)); //fórmula da proj ortogonal
    if (projecao < 0){
        projecao = 0;
    }
    else if (projecao > 1){
        projecao = 1;
    }

    float Px = V1x +(projecao*Lx); //coord x do Ponto Mais Perto
    float Py = V1y +(projecao*Ly);//coord y do Ponto Mais Perto

    return distancia(Px, Py, Cx, Cy);
}


int semContato (Hero *hero, Enemy *enemy){
    if (enemy->active == 1){
        //colisão vértices
        if (distancia(hero->ship.x, hero->ship.y, enemy->ship.x, enemy->ship.y) <= enemy->r){ //vértice cima
            return 0;
        }
        if (distancia (hero->ship.x - HERO_W/2, hero->ship.y + HERO_H, enemy->ship.x, enemy->ship.y) <= enemy->r){ //vértice esquerda
            return 0;
        }
        if (distancia (hero->ship.x + HERO_W/2, hero->ship.y + HERO_H, enemy->ship.x, enemy->ship.y) <= enemy->r){ //vértice esquerda
            return 0;
        }


        //colisão arestas
        if (distanciaPontoMaisPerto(hero->ship.x, hero->ship.y, hero->ship.x - HERO_W/2, hero->ship.y + HERO_H, enemy->ship.x, enemy->ship.y) <= enemy->r){
            return 0;
        }
        if (distanciaPontoMaisPerto(hero->ship.x, hero->ship.y, hero->ship.x + HERO_W/2, hero->ship.y + HERO_H, enemy->ship.x, enemy->ship.y) <= enemy->r){
            return 0;
        }
        if (distanciaPontoMaisPerto(hero->ship.x - HERO_W/2, hero->ship.y + HERO_H, hero->ship.x + HERO_W/2, hero->ship.y + HERO_H, enemy->ship.x, enemy->ship.y) <= enemy->r){
            return 0;
        }

    }
    return 1;
}

void eliminaEnemy (Enemy *e, Tiro *t, Hero *h){
    if (e->active == 1 && (t->modo == TIRO_ATIVO || t->modo == TIRO_UPGRADE)) {
        if (distancia(t->x, t->y, e->ship.x, e->ship.y) <= (t->raio + e->r)){
            e->active = 0;
            e->ship.tiro.modo = TIRO_ATIVO;
            e->ship.tiro.raio = e->r;

            h->score += (e->r)*5;

        }
    }
}

int tocaUpgrade (Upgrade *u, Hero *hero) {
    if (u->active > 0){
        if (distanciaPontoMaisPerto(u->ship.x, u->ship.y+TAM_UP1, u->ship.x+TAM_UP1, u->ship.y+TAM_UP1, hero->ship.x, hero->ship.y) <= TAM_UP1/2){
            return 1;
        }
        if (distanciaPontoMaisPerto(u->ship.x, u->ship.y, u->ship.x, u->ship.y+TAM_UP1, hero->ship.x+(HERO_W/2), hero->ship.y+HERO_H) <= TAM_UP1/2){
            return 1;
        }
        if (distanciaPontoMaisPerto(u->ship.x+10, u->ship.y, u->ship.x+TAM_UP1, u->ship.y+TAM_UP1, hero->ship.x-(HERO_W/2), hero->ship.y+HERO_H) <= TAM_UP1/2){
            return 1;
        }

        if (distanciaPontoMaisPerto(u->ship.x, u->ship.y, u->ship.x+TAM_UP1, u->ship.y, hero->ship.x, hero->ship.y+HERO_H)<=TAM_UP1/2){
            return 1;
        }

        if (distanciaPontoMaisPerto(hero->ship.x, hero->ship.y, hero->ship.x + HERO_W/2, hero->ship.y + HERO_H, u->ship.x, u->ship.y+TAM_UP1) <= (TAM_UP1*sqrt(2)/2)){
            return 1;
        }
        if (distanciaPontoMaisPerto(hero->ship.x - HERO_W/2, hero->ship.y + HERO_H, hero->ship.x + HERO_W/2, hero->ship.y + HERO_H,  u->ship.x+TAM_UP1, u->ship.y+TAM_UP1) <= (TAM_UP1*sqrt(2)/2)){
            return 1;
        }
        return 0;
    }
    return 0;
}

void pegaUpgrade (Upgrade *u, Hero *h){
    if (tocaUpgrade(u, h) == 1){
        u->active = 0;

        h->ship.tiro.modo = TIRO_UPGRADE;
        h->ship.tiro.raio = RAIO_TIRO;
    }

}





        //Geral

void initGlobals() {
	BKG_COLOR = al_map_rgb(10, 10, 10);
	//carrega o arquivo arial.ttf da fonte Arial e define que sera usado o tamanho 32 (segundo parametro)
	FONT_32 = al_load_font("arial.ttf", 32, 1);
}

void drawScenario(Hero s, ALLEGRO_BITMAP *fundo[], int pontos) {

	char score_txt[16];
	al_clear_to_color(al_map_rgb(0, 0, 0));

    if (pontos < 3000){
        al_draw_scaled_bitmap(fundo[0], 0, 0, al_get_bitmap_width(fundo[0]), al_get_bitmap_height(fundo[0]), 0, 0, SCREEN_W, SCREEN_H, 0);
    }
    else if (pontos < 7500){
        al_draw_scaled_bitmap(fundo[1], 0, 0, al_get_bitmap_width(fundo[1]), al_get_bitmap_height(fundo[1]), 0, 0, SCREEN_W, SCREEN_H, 0);
    }
    else if (pontos < 12500){
        al_draw_scaled_bitmap(fundo[2], 0, 0, al_get_bitmap_width(fundo[2]), al_get_bitmap_height(fundo[2]), 0, 0, SCREEN_W, SCREEN_H, 0);
    }
    else{
        al_draw_scaled_bitmap(fundo[3], 0, 0, al_get_bitmap_width(fundo[3]), al_get_bitmap_height(fundo[3]), 0, 0, SCREEN_W, SCREEN_H, 0);
    }
}




















int main(int argc, char **argv){

	srand(time(NULL));

	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;


	//----------------------- rotinas de inicializacao ---------------------------------------

	//inicializa o Allegro
	if(!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}

    //inicializa o módulo de primitivas do Allegro
    if(!al_init_primitives_addon()){
		fprintf(stderr, "failed to initialize primitives!\n");
        return -1;
    }


	//cria um temporizador que incrementa uma unidade a cada 1.0/FPS segundos
    timer = al_create_timer(1.0 / FPS);
    if(!timer) {
		fprintf(stderr, "failed to create timer!\n");
		return -1;
	}

	//cria uma tela com dimensoes de SCREEN_W, SCREEN_H pixels
	display = al_create_display(SCREEN_W, SCREEN_H);
	if(!display) {
		fprintf(stderr, "failed to create display!\n");
		al_destroy_timer(timer);
		return -1;
	}

	//instala o teclado
	if(!al_install_keyboard()) {
		fprintf(stderr, "failed to install keyboard!\n");
		return -1;
	}

	//inicializa o modulo allegro que carrega as fontes
	al_init_font_addon();

	//inicializa o modulo allegro que entende arquivos tff de fontes
	if(!al_init_ttf_addon()) {
		fprintf(stderr, "failed to load tff font module!\n");
		return -1;
	}

	//carrega o arquivo arial.ttf da fonte Arial e define que sera usado o tamanho 32 (segundo parametro)
    ALLEGRO_FONT *size_32 = al_load_font("arial.ttf", 32, 1);
	if(size_32 == NULL) {
		fprintf(stderr, "font file does not exist or cannot be accessed!\n");
	}

 	//cria a fila de eventos
	event_queue = al_create_event_queue();
	if(!event_queue) {
		fprintf(stderr, "failed to create event_queue!\n");
		al_destroy_display(display);
		return -1;
	}



	//registra na fila os eventos de tela (ex: clicar no X na janela)
	al_register_event_source(event_queue, al_get_display_event_source(display));
	//registra na fila os eventos de tempo: quando o tempo altera de t para t+1
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	//registra na fila os eventos de teclado (ex: pressionar uma tecla)
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	//----------------------jogo-------------------

	al_init_image_addon();
    al_install_audio();
    al_init_acodec_addon();


	//inicializa globais
	initGlobals();

    ALLEGRO_BITMAP *fundo_img[5];
    fundo_img[0]= al_load_bitmap("imagens/fundo0.jpeg");
    fundo_img[1]= al_load_bitmap("imagens/fundo1.jpeg");
    fundo_img[2]= al_load_bitmap("imagens/fundo2.jpeg");
    fundo_img[3]= al_load_bitmap("imagens/fundo3.jpeg");
    fundo_img[4]= al_load_bitmap("imagens/fundoMorte.jpeg");

    ALLEGRO_BITMAP *jogador_img[2];
    jogador_img[0]= al_load_bitmap("imagens/jogador0.png");
    jogador_img[1]= al_load_bitmap("imagens/jogador1.png");

    ALLEGRO_BITMAP *tiro_img[2];
    tiro_img[0]= al_load_bitmap("imagens/tiro0.png");
    tiro_img[1]= al_load_bitmap("imagens/tiro1.png");

    ALLEGRO_BITMAP *inimigo_img[5];
    inimigo_img[0]= al_load_bitmap("imagens/inimigo0.png");
    inimigo_img[1]= al_load_bitmap("imagens/inimigo1.png");
    inimigo_img[2]= al_load_bitmap("imagens/inimigo2.png");
    inimigo_img[3]= al_load_bitmap("imagens/inimigo3.png");
    inimigo_img[4]= al_load_bitmap("imagens/inimigo4.png");

    ALLEGRO_BITMAP *upgrade_img = al_load_bitmap("imagens/PowerUp.png");



    al_reserve_samples(1);
    ALLEGRO_AUDIO_STREAM *musica = al_load_audio_stream("sons/somAmbiente.wav", 4, 2048);
    al_set_audio_stream_playmode(musica, ALLEGRO_PLAYMODE_LOOP);

	int segundos=1;
	float segundosPassados = 0;
	//Cria o heroi
	Hero Hero;
	Enemy Enemy[NUM_ENEMIES];
    Upgrade Upgrade;
	initHero(&Hero);

	int index=0;


    for (index =0; index < NUM_ENEMIES; index++){
        initEnemy(&Enemy[index]);
    }


    ALLEGRO_COLOR cor;

	//inicia o temporizador
	al_start_timer(timer);

	int playing = 1;
	while(playing) {
        al_attach_audio_stream_to_mixer(musica, al_get_default_mixer());
		ALLEGRO_EVENT ev;
		//espera por um evento e o armazena na variavel de evento ev
		al_wait_for_event(event_queue, &ev);

		//se o tipo de evento for um evento do temporizador, ou seja, se o tempo passou de t para t+1
		if(ev.type == ALLEGRO_EVENT_TIMER) {

            cor = al_map_rgb(100 + rand()%156, 100 + rand()%156, 100 + rand()%156);

            if (Hero.score <= 0){
                Hero.score -= SCORE_PENALTY;
            }
            else {
                Hero.score -= SCORE_PENALTY*(1+(Hero.score/1000)); //a cada 1000 pontos a  penalidade dobra
            }


            segundosPassados += (1.0/FPS);
            if ((int)segundosPassados==segundos){
                segundos += 1;
            }


            //confere se o hero tocou algum enemy
            for (index = 0; index< NUM_ENEMIES; index++){
                playing = semContato(&Hero, &Enemy[index]);
                if (playing == 0){
                    break;
                }
            }

			drawScenario(Hero, fundo_img, Hero.score);
			updateHero(&Hero);
			drawHero(Hero, jogador_img, tiro_img, &Upgrade);


            if (segundos >= 3){
                    for (index=0; index< NUM_ENEMIES; index++){
                    updateEnemy(&Enemy[index]);
                }
                    for (index=0; index< NUM_ENEMIES; index++){
                    drawEnemy(&Enemy[index], inimigo_img, Hero.score);
                }
            }


            for (index  =0; index< NUM_ENEMIES; index++){
                eliminaEnemy (&Enemy[index], &Hero.ship.tiro, &Hero);
            }

            for (index =0; index<NUM_ENEMIES; index++){
                for (int i =0; i<NUM_ENEMIES; i++){
                    if (i != index){
                        eliminaEnemy(&Enemy[i], &Enemy[index].ship.tiro, &Hero);
                    }
                }
            }


            for (index =0; index < NUM_ENEMIES; index++){
                if (Enemy[index].active == -1){
                    initEnemy(&Enemy[index]);
                }
            }

            if (segundos%5 == 0 && segundos%10 != 0){
                initUpgrade (&Upgrade);
            }

            if (Upgrade.active != -1) {
                updateUpgrade(&Upgrade, &Hero);
                drawUpgrade(&Upgrade, upgrade_img);
            }

            pegaUpgrade (&Upgrade, &Hero);


			//atualiza a tela (quando houver algo para mostrar)
			al_flip_display();


			//pausa o jogo por 3 segundos se o jogador morrer
			if(!playing)
				al_rest(3);





		}
		//se o tipo de evento for o fechamento da tela (clique no x da janela)
		else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			playing = 0;
		}
		//se o tipo de evento for um pressionar de uma tecla
		else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			//imprime qual tecla foi

			switch(ev.keyboard.keycode) {
			//se a tecla for o W
				case ALLEGRO_KEY_W:
				    Hero.dir_y--;
				break;

				case ALLEGRO_KEY_S:
				    Hero.dir_y++;
				break;

				case ALLEGRO_KEY_A:
				    Hero.dir_x--;
				break;

				case ALLEGRO_KEY_D:
				    Hero.dir_x++;
				break;

				case ALLEGRO_KEY_SPACE:
					if(Hero.ship.tiro.modo == TIRO_INATIVO) {
						Hero.ship.tiro.modo = TIRO_ATIVO;
						Hero.ship.tiro.raio = RAIO_TIRO;
					}
				break;
			}
		}


		else if(ev.type == ALLEGRO_EVENT_KEY_UP) {
			//imprime qual tecla foi
			//printf("\ncodigo tecla: %d", ev.keyboard.keycode);

			switch(ev.keyboard.keycode) {
			//se a tecla for o W
				case ALLEGRO_KEY_W:
					Hero.dir_y++;
				break;

				case ALLEGRO_KEY_S:
					Hero.dir_y--;
				break;

				case ALLEGRO_KEY_A:
					Hero.dir_x++;
				break;

				case ALLEGRO_KEY_D:
					Hero.dir_x--;
				break;

				/* case ALLEGRO_KEY_SPACE:
					if(Hero.ship.tiro.modo == TIRO_HOLDING)
						Hero.ship.tiro.modo = TIRO_ATIVO;
				break; */

			}

		}
	} //fim do while

	//procedimentos de fim de jogo (fecha a tela, limpa a memoria, etc)

	al_set_audio_stream_playing(musica, false);

    char my_text[100];
    char maior_ponto [100];


        FILE *arq_leitura = fopen ("historico.txt", "r");
        if (arq_leitura == NULL){
        printf ("erro ao abrir documento");
        return 1;
        }
        int recorde = 0;
        if (fscanf (arq_leitura, "%d", &recorde) != 1){;
            recorde = (int)Hero.score -1;
        }

        FILE *arq_escrita = fopen ("historico.txt", "w");
        if (arq_escrita == NULL){
            printf ("erro ao abrir documento");
            return 1;
        }


        al_clear_to_color(al_map_rgb(0,0,0));
        al_draw_scaled_bitmap(fundo_img[4], 0, 0, al_get_bitmap_width(fundo_img[4]), al_get_bitmap_height(fundo_img[4]), 0, 0, SCREEN_W, SCREEN_H, 0);

        if (recorde >= (int)Hero.score) {
            sprintf(my_text, "Pontuação: %d", (int)Hero.score);
            al_draw_filled_rectangle(SCREEN_W/3 - 20, SCREEN_H/2 + 45, SCREEN_W/3 + 300, SCREEN_H/2 + 90, al_map_rgb(0, 0, 0));
            al_draw_text(FONT_32, al_map_rgb(220, 30, 0), SCREEN_W/3, SCREEN_H/2 + 50, 0, my_text);

            sprintf(maior_ponto, "Recorde: %d", recorde);
            al_draw_filled_rectangle(SCREEN_W/3 - 20, SCREEN_H/2 + 145, SCREEN_W/3 + 300, SCREEN_H/2 + 190, al_map_rgb(0, 0, 0));
            al_draw_text(FONT_32, al_map_rgb(136, 231, 136), SCREEN_W/3, SCREEN_H/2 + 150, 0, maior_ponto);
            fprintf (arq_escrita, "%d", recorde);
        }
        else {
            sprintf(my_text, "NOVO RECORDE: %d", (int)Hero.score);
            al_draw_filled_rectangle(SCREEN_W/3 - 20, SCREEN_H/2 + 45, SCREEN_W/3 + 500, SCREEN_H/2 + 90, al_map_rgb(0, 0, 0));
            al_draw_text(FONT_32, al_map_rgb(136, 231, 136), SCREEN_W/3, SCREEN_H/2 + 50, 0, my_text);
            fprintf (arq_escrita, "%d", (int)Hero.score);
        }


        fclose(arq_escrita);
        fclose(arq_leitura);

		al_flip_display();
		al_rest(3);

	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
    for(int i =0; i<5; i++){
        al_destroy_bitmap(fundo_img[i]);
    }

    for(int i =0; i<2; i++){
         al_destroy_bitmap(jogador_img[i]);
    }

    for (int i =0; i< 2; i++){
        al_destroy_bitmap(tiro_img[i]);
    }
    for(int i =0; i<5; i++){
        al_destroy_bitmap(inimigo_img[i]);
    }
    al_destroy_bitmap(upgrade_img);


    al_destroy_audio_stream(musica);
    al_uninstall_audio();


	return 0;
}
