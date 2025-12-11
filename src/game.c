#include "game.h"
#include "board.h"
#include "leaderboard.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

// Fungsi utama untuk memainkan game Sudoku
void play_sudoku() {
    // Input nama pemain
    char name[128];

    while (1) {
        printf("Masukkan nama pemain: ");
        if (!fgets(name, sizeof(name), stdin)) return;
        name[strcspn(name, "\n")] = 0; // hapus newline

        // hilangkan spasi di depan belakang
        int len = strlen(name);
        while (len > 0 && name[len-1] == ' ') name[--len] = '\0';
        while (name[0] == ' ') memmove(name, name+1, --len);

        if (len > 0) break; // valid → keluar loop
        printf("Nama tidak boleh kosong!\n");
    }


    // Penjelasan game
    printf("\nPenjelasan\n");
    printf("Sudoku adalah permainan logika angka yang terdiri dari kotak 4 baris x 4 kolom,\n");
    printf("yang dibagi lagi menjadi empat blok kecil berukuran 2x2.\n");
    printf("Tujuan permainan ini adalah mengisi seluruh kotak dengan angka 1 sampai 4.\n");

    printf("\nAturan Dasar\n");
    printf("Pada Sudoku 4x4, setiap angka 1-4 harus muncul tepat sekali di:\n");
    printf("Setiap baris\n");
    printf("Setiap kolom\n");
    printf("Setiap blok 2x2\n");
    printf("Angka yang sudah terisi dari awal adalah petunjuk dan tidak boleh diubah.\n");

    printf("\nSistem Permainan\n");
    printf("1. Poin awal 1000 dan timer dimulai setelah menekan enter\n");
    printf("2. Masukkan angka dari 1-4 untuk kotak yang kosong (.)\n");
    printf("3. Masukkan dengan format (r c v)\n");
    printf("   r = row (baris)\n");
    printf("   c = coloumn (kolom)\n");
    printf("   v = variabel (angka)\n");
    printf("4. Ketik 'r' untuk menunjukkan jawaban\n");
    printf("5. Ketik 'q' untuk kembali ke menu\n");

    printf("Tekan Enter untuk mulai...\n");

    // Inisialisasi papan
    int board[N][N];
    copy_board(board, base_solved);
    randomize_solution(board);
    int solution[N][N];
    copy_board(solution, board);
    generate_puzzle(board, 6); // buat puzzle dengan beberapa sel kosong

    // Tandai sel yang merupakan petunjuk awal
    int given[N][N];
    for (int i=0;i<N;i++) for (int j=0;j<N;j++) given[i][j] = (board[i][j] != 0);

    char msg[128] = "";

    // Tunggu Enter
    {
        char _tmp[64];
        fgets(_tmp, sizeof(_tmp), stdin);
    }

    // Timer dimulai
    clock_t start_time = clock();

    // Loop utama permainan
    while(1) {
        double elapsed_seconds = (double)(clock()-start_time)/CLOCKS_PER_SEC;
        int score = calculate_score(elapsed_seconds);

        clear_screen();
        print_board(board, elapsed_seconds, score);  // Tampilkan papan
        if (msg[0]) { printf("%s\n\n",msg); msg[0]=0; }

        printf("\nPlayer: %s\n",name);
        printf("Input (r c v), 'r' (reveal), 'q' (keluar)': ");

        char in[64];
        if (!fgets(in,sizeof(in),stdin)) break;
        in[strcspn(in,"\n")] = 0;

        if (!strcmp(in,"q")) break; // keluar
        if (!strcmp(in,"r")) { // reveal jawaban
            clear_screen();
            print_board(solution, elapsed_seconds, score);
            printf("Tekan Enter...");
            fgets(in,sizeof(in),stdin);
            break;
        }

        // Parsing input pemain
        int br,bc,bv;
        if (sscanf(in,"%d %d %d",&br,&bc,&bv)!=3) {
            snprintf(msg,sizeof(msg),"Format salah.");
            continue;
        }
        if (br<1||br>4||bc<1||bc>4||bv<1||bv>4) {
            snprintf(msg,sizeof(msg),"Silahkan masukkan angka 1-4");
            continue;
        }
        int r=br-1, c=bc-1, v=bv;

        if (given[r][c]) { snprintf(msg,sizeof(msg),"Tidak boleh ubah angka awal."); continue; }
        if (solution[r][c]!=v) { snprintf(msg,sizeof(msg),"Salah."); continue; }

        board[r][c]=v; // input benar → simpan

        // Cek apakah puzzle sudah selesai
        int full=1;
        for (int i=0;i<N;i++) for (int j=0;j<N;j++) if (board[i][j]==0) full=0;

        if (full) {
            double elapsed_total=(double)(clock()-start_time)/CLOCKS_PER_SEC;
            int final_score=calculate_score(elapsed_total);

            clear_screen();
            print_board(board, elapsed_total, final_score);
            printf("\nSelesai!\n");
            printf("Waktu: %.2fs\n",elapsed_total);
            printf("Skor : %d\n",final_score);

            save_score(name,final_score,elapsed_total); // simpan leaderboard

            printf("\nSkor disimpan!\nTekan Enter...");
            fgets(in,sizeof(in),stdin);
            break;
        }
    }
}