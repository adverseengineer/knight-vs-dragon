//Nick Sells, 2021
//main.c

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define randf(limit) ((limit) * ((float) rand() / RAND_MAX))

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long s64;

u32 hero_maxHP = 300;
s32 hero_curHP = 300;
s32 hero_atk = 80;
s32 hero_def = 30;
float hero_dodgeChance = 0.9;
float hero_critChance = 0.33;
s32 hero_critMod = 3;
u32 hero_deaths = 0;

u32 mstr_maxHP = 1500;
s32 mstr_curHP = 1500;
s32 mstr_atk = 120;
s32 mstr_def = 60;
float mstr_dodgeChance = 0.0;
float mstr_critChance = 0.25;
s32 mstr_critMod = 255;
u32 mstr_deaths = 0;

void SIGINT_handler(int _unused) {
	printf("\nHero Deaths: %d\n", hero_deaths);
	printf("Monster Deaths: %d\n", mstr_deaths);
	exit(0);
}

//simulates the monster's actions
void mstr_act(void) {
	//if the monster hits
	
	
	if(randf(1) > hero_dodgeChance) {
		
		//roll for crit
		s32 mod = 1;
		if(randf(1) < mstr_critChance) {
			mod = mstr_critMod;
			printf("Monster: Critical Hit!!!\n");
		}
		
		//deal damage, but only if it's positive. no healing
		s32 dmg = mstr_atk * mod - hero_def;
		if(dmg > 0) {
			hero_curHP -= dmg;
			printf("Monster: Deals %d Damage!\n", dmg);
		}
	}
	else
		printf("Monster: Miss!\n");
}

//simulates the hero's actions
void hero_act(void) {
	//if the hero hits
	if(randf(1) > mstr_dodgeChance) {
		
		//roll for crit
		s32 mod = 1;
		if(randf(1) < hero_critChance) {
			mod = hero_critMod;
			printf("Hero: Critical Hit!!!\n");
		}
		
		//deal damage, but only if it's positive. no healing
		s32 dmg = hero_atk * mod - mstr_def;
		if(dmg > 0) {
			printf("Hero: Deals %d Damage!\n", dmg);
			mstr_curHP -= dmg;
		}
	}
	else
		printf("Hero: Miss!\n");
}

int main(int argc, char** argv) {
	
	signal(SIGINT, SIGINT_handler);
	srand(time(NULL));
	
	while(1) {
		
		hero_curHP = hero_maxHP;
		mstr_curHP = mstr_maxHP;
		
		while((hero_curHP > 0) && (mstr_curHP > 0)) {
			mstr_act();
			hero_act();
		}
		
		if(hero_curHP <= 0) {
			hero_deaths++;
			printf("Hero Died! %d/%d HP\n", hero_curHP, hero_maxHP);
		}
		
		if(mstr_curHP <= 0) {
			mstr_deaths++;
			printf("Monster Died! (%d/%d HP)\n", mstr_curHP, mstr_maxHP);
		}
		
		printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
	}
}