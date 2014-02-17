/*
 * Copyright (C) 2014 Sebastian Sura
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define RED ('!')
#define GREEN ('=')
#define BLUE ('+')
#define BROWN ('#')

#define BLACK ('x')
#define WHITE ('o')

#define true (1)
#define false (0)

#ifdef DEBUG
#define clrscr()
#elif __unix__
#define clrscr() system("clear");
#elif __BORLANDC__ && __MSDOS__
#include <conio.h>
#elif __WIN32__ || __MSC_VER
#define clrscr() system("cls");
#else
#error "Diese Platform wird nicht unterstuetzt"
#endif

// TODO: make it changable through command arguments

#define NUM_GUESSES 10
#define NUM_ROUNDS 8

struct score
{
        int p1;
        int p2;
};

void generateCode(char *buf);
int is_valid(char *buf);
void update_table(char *data1, char *data2, char table[][2][5], int entry);
void print_table(char table[][2][5]);
int safe_strlen(char *str);
void print_score(struct score *score);
void get_pins(char *cmp1, char *cmp2, char *pins);

void print_help();

int num_rounds = NUM_ROUNDS, num_guesses = NUM_GUESSES;

const char COLORS[6] = { RED, GREEN, BLUE, BROWN, BLACK, WHITE };

int main(int argc, char **argv)
{
        { int i;
                for (i = 1; i < argc; i++) {
                        if ( !strcmp(argv[i], "-r") || !strcmp(argv[i], "--rounds") )
                                num_rounds = atoi(argv[++i]);
                        else if ( !strcmp(argv[i], "-g") || !strcmp(argv[i], "--guesses") )
                                num_guesses = atoi(argv[++i]);
                        else if ( !strcmp(argv[i], "-h") || !strcmp(argv[i], "--help") ) {
                                print_help();
                                exit(0);
                        }
                                
                }
        }
        char code[5] = { 0 };
        char choice[5] = "????";
        char pins[5] = { 0 };
        char table[num_guesses][2][5];
        int error = 0;
        struct score score = { 0 };
        int guess, round = 0;
        srand(time(NULL));

        while (round < num_rounds) {
                guess = 0;
                { int i, j;
                        for (i = 0; i < num_guesses; i++) {
                                memcpy(table[i][0], "----", 5);
                                memcpy(table[i][1], "????", 5);
                        }
                }
                
                generateCode(code);
                code[4] = '\0';
        
                clrscr();
#ifdef DEBUG
                printf("%s\n", code);
#endif
                do {
                        print_score(&score);
                        print_table(table);
                        if ( error ) {
                                printf("Eingabe war nicht korrekt.\n");
                                error = false;
                        }
                        printf("%16s", "Ihre Eingabe : ");
                        fflush(stdout);
                        fgets(choice, 5, stdin);
                        while ( getc(stdin) != '\n' );
                        fflush(stdin);
                        if ( !is_valid(choice) ) {
                                error = true;
                                clrscr();
                                continue;
                        }
                        get_pins(choice, code, pins);
                        update_table(choice, pins, table, guess);
                        clrscr();
                        guess++;
                } while ( strcmp(code, choice) && guess < num_guesses);
                if ( !strcmp(code, choice) ) {
                        if (guess == num_guesses)
                                score.p1 += 2;
                        else
                                score.p1++;
                } else {
                        score.p2++ ;
                }
                round++;
                printf("Der Code : %s\n", code);
                printf("Druecken Sie eine Taste um fortzufahren...");
                getc(stdin);

        }
        clrscr();
        print_score(&score);
        print_table(table);
        print_score(&score);
        if (score.p1 > score.p2)
                printf("Sie haben gewonnen !\n");
        else if (score.p1 == score.p2)
                printf("Gleichstand!\n");
        else
                printf("Sie haben verloren :(\n");

        return 0;
}

void generateCode(char *buf)
{
        int i = -1;
        while (++i < 4)
                buf[i] = COLORS[rand()%4];
        
}

void print_table(char table[][2][5])
{
        int i;
        for (i = 0; i < num_guesses; i++) {
                printf("%20s %4s\n", table[i][0], table[i][1]);
        }
}

int is_valid(char *choice)
{
        if ( strlen(choice) != 4 ) return false;
        int ok = true;
        do {
                ok &= (*choice == RED) || (*choice == GREEN) || (*choice == BLUE) || (*choice == BROWN);
        } while ( ok && *(++choice) != '\0' );
        return ok;
}

void update_table(char *data1, char *data2, char table[][2][5], int entry)
{
        memcpy(&(table[entry][0]), data1, 5);
        memcpy(&(table[entry][1]), data2, 5);
}

int safe_strlen(char *str) {
        int i = -1;
        while (i++ < 4)
                if (*(str++) = '\0') return i;
}

void print_score(struct score *score)
{
        printf("%19d - %-d\n", score->p1, score->p2);
}

void get_pins(char *cmp1, char *cmp2, char *pins)
{
        int i,j, k=0;
        char cmp2_c[5];
        strcpy(pins, "----");
        memcpy(cmp2_c, cmp2, 5);
        for (i = 0; i < 4; i++)
                if (cmp2_c[i] == cmp1[i]) {
                        pins[k++] = BLACK;
                        cmp2_c[i] = '?';
                }
        for (i = 0; i < 4; i++) {
                for(j = 0; j < 4; j++) {
                        if (cmp1[i] == cmp2_c[j]) {
                                pins[k++] = WHITE;
                                cmp2_c[j] = '?';
                                break;
                        }
                }
        }
}

void print_help()
{
        printf("USAGE: MasterMind <-r|--rounds> <-g|--guesses> <-h|--help>\n\n");
        printf("This is a game where you guess a code which contains only the characters:\n '!', '-', '+', '#'\n This is more or less a implementation of the game MasterMind (\"http://en.wikipedia.org/wiki/Mastermind_(board_game)\")\n");
               printf("configuration options:\n\n-r, --rounds: configures the round count\n-g, --guesses: configures the count of guesses per round\n");
}
