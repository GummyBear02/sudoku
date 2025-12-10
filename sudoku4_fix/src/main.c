#include "board.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "game.h"
#include "leaderboard.h"
#include "utils.h"

// main.c
// Berfungsi sebagai menu utama program Sudoku.
// Mengatur navigasi ke:
// - Main Sudoku (play_sudoku)
// - Leaderboard (show_leaderboard)
// - Keluar dari program

int main(void) {
    // Inisialisasi random seed
    srand((unsigned)time(NULL) ^ (unsigned)(time(NULL) >> 2));

    // Loop menu utama
    while (1) {
        clear_screen();
        printf("====== SUDOKU 4x4 ======\n");
        printf("1. Main Sudoku\n");
        printf("2. Lihat Leaderboard (Top %d)\n", TOP);
        printf("3. Keluar\n");
        printf("Pilihan: ");

        char in[64];
        if (!fgets(in,sizeof(in),stdin)) break;
        int op = atoi(in);

        if (op==1) {
            clear_screen();
            play_sudoku(); // Mulai permainan
        }
        else if (op==2) {
            clear_screen();
            show_leaderboard(); // Tampilkan leaderboard
        }
        else if (op==3) break; // Keluar program
    }
    return 0;
}