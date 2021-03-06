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

#define RED ('a')
#define GREEN ('b')
#define BLUE ('c')
#define BROWN ('d')
#define YELLOW ('e')
#define PINK ('f')

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
#elif __WIN32__ || __MSC_VER || __POCC__
#define clrscr() system("cls");
#else
#error "Diese Platform wird nicht unterstuetzt"
#endif

#define NUM_GUESSES 10
#define NUM_ROUNDS 8

struct score
{
        int p1;
        int p2;
};

void generateCode(char *buf);
int is_valid(char *buf);
void update_table(const char *choice, const char *pins, char table[][2][5], int entry);
void print_table(char table[][2][5]);
int safe_strlen(char *str);
void print_score(struct score *score);
<<<<<<< HEAD
void get_pins(const char *cmp1, const char *cmp2, char *pins);

=======
void get_pins(char *cmp1, char *cmp2, char *pins);
>>>>>>> 525043060587cbe849b18bfd985ad4117fcdb12a
void print_help(void);

int num_rounds = NUM_ROUNDS, num_guesses = NUM_GUESSES;

const char COLORS[8] = { RED, GREEN, BLUE, BROWN, YELLOW, PINK, BLACK, WHITE };

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
                { int i;
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
                buf[i] = COLORS[rand()%6];
        
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
                ok &= (*choice == RED) || (*choice == GREEN) || (*choice == BLUE) || (*choice == BROWN) || (*choice == PINK) || (*choice == YELLOW);
        } while ( ok && *(++choice) != '\0' );
        return ok;
}

void update_table(const char *choice, const char *pins, char table[][2][5], int entry)
{
        memcpy(&(table[entry][0]), choice, 5);
        memcpy(&(table[entry][1]), pins, 5);
}

int safe_strlen(char *str) {
        int i = -1;
        while (i++ < 4)
<<<<<<< HEAD
                if (*(str++) == '\0') return i;
=======
                if (*(str++) == '\0')
			break;
>>>>>>> 525043060587cbe849b18bfd985ad4117fcdb12a
	return i;
}

void print_score(struct score *score)
{
        printf("%19d - %-d\n", score->p1, score->p2);
}

void get_pins(const char *choice, const char *code, char *pins)
{
        int i,j, k=0;
        char code_c[5], choice_c[5];
        strcpy(pins, "----");
        memcpy(code_c, code, 5);
	memcpy(choice_c, choice, 5);
        for (i = 0; i < 4; i++)
                if (choice_c[i] == code_c[i]) {
                        pins[k++] = BLACK;
                        code_c[i] = '?';
			choice_c[i] = '$';
                }
        for (i = 0; i < 4; i++) {
                for(j = 0; j < 4; j++) {
                        if (choice_c[i] == code_c[j]) {
                                pins[k++] = WHITE;
                                code_c[j] = '?';
				choice_c[i] = '$';
                                break;
                        }
                }
        }
}

void print_help(void)
{
        printf("USAGE: MasterMind <-r|--rounds> <-g|--guesses> <-h|--help>\n\n");
        printf("This is a game where you guess a code which contains only the characters:\n '!', '-', '+', '#'\n This is more or less a implementation of the game MasterMind (\"http://en.wikipedia.org/wiki/Mastermind_(board_game)\")\n");
        printf("configuration options:\n\n-r, --rounds: configures the round count\n-g, --guesses: configures the count of guesses per round\n");
}
