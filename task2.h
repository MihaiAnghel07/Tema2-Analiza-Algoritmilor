// Copyright 2020
// Authors: Radu Nichita, Matei Simtinică

#ifndef TASK2_H_
#define TASK2_H_

#include "task.h"

/*
 * Task2
 * You have to implement 4 methods:
 * read_problem_data         - read the problem input and store it however you see fit
 * formulate_oracle_question - transform the current problem instance into a SAT instance and write the oracle input
 * decipher_oracle_answer    - transform the SAT answer back to the current problem's answer
 * write_answer              - write the current problem's answer
 */
class Task2 : public Task {
 private:
    // TODO: define necessary variables and/or data structures
    int N;
    int M;
    int K;
    char answer[6];
    int ans[10000];
    int adj[1000][1000];
    int cnt2;

 public:
    void solve() override {
        read_problem_data();
        formulate_oracle_question();
        ask_oracle();
        decipher_oracle_answer();
        write_answer();
    }

    void read_problem_data() override {
        // TODO: read the problem input (in_filename) and store the data in the object's attributes
        
        std::ifstream infile(in_filename);
        infile >> N >> M >> K;

        // citesc si adaug relatiile dintre familii intr-o matrice de adiacenta
        int i = 1, y, z;
        while (infile >> y >> z) {
            adj[y][z] = adj[z][y] = 1;
            i++;
        }

        infile.close();
    }

    void formulate_oracle_question() {
        // TODO: transform the current problem into a SAT problem and write it (oracle_in_filename) in a format
        //  understood by the oracle
        
        int x[K + 1][N], cnt = 1;
        std::ofstream outfile(oracle_in_filename);

        // numarul variabilelor este N * K (N posibile familii de dimensiune K)
        // din prima constrangere: K clauze
        // din a doua constrangere: K*(K - 1)*N*N - 2*M - N clauze
        // din a treia constrangere: K*(K - 1)*N + K*N*(N - 1) clauze
        // in total: K + K*(K - 1)*N*N - 2*M - N + K*(K - 1)*N + K*N*(N - 1)clauze

        outfile << "p cnf ";
        outfile << N * K << " ";
        outfile << (K + K*(K - 1)*N*N - 2*M - N + K*(K - 1)*N + K*N*(N - 1));
        outfile << "\n";

        // formez variabilele conform pergamentului
        for (int i = 1; i <= K; i++) {
            for (int j = 1; j <= N; j++) {
                x[i][j] = cnt;
                cnt++;
            }
        }

        // sunt 3 constrangeri pe care trebuie sa le respectam pentru 
        // a alcatui corect clauzele (constrangerile din pergament)

        // prima constrangere
        for (int i = 1; i <= K; i++) {
            for (int v = 1; v <= N; v++) {
                outfile << x[i][v] << " ";
            }
            outfile << "0\n";
        }

        // a doua constrangere
        for (int i = 1; i <= K; i++) {
            for (int j = 1; j <= K; j++) {
                if (i != j) {
                    for (int v = 1; v <= N; v++) {
                        for (int w = 1; w <= N; w++) {
                            if (v != w) {
                                if (adj[v][w] == 0) {
                                    outfile << -x[i][v] << " " << -x[j][w] << " 0\n";
                                }
                            }
                        }
                    }
                }
            }
        }

        // a treia constrangere
        for (int i = 1; i <= K; i++) {
            for (int j = 1; j <= K; j++) {
                if (i != j) {
                    for (int v = 1; v <= N; v++) {            
                        outfile << -x[i][v] << " " << -x[j][v] << " 0\n";
                    }
                }
            }
        }

        for (int i = 1; i <= K; i++) {
            for (int v = 1; v <= N; v++) {
                for (int w = 1; w <= N; w++) {                
                    if (v != w) {
                        outfile << -x[i][v] << " " << -x[i][w] << " 0\n";
                    }
                }
            }
        }


        outfile.close();
    }

    void decipher_oracle_answer() {
        // TODO: extract the current problem's answer from the answer given by the oracle (oracle_out_filename)
        
        int n2;
        
        std::ifstream infile(oracle_out_filename);
        infile >> answer;
        infile >> n2;

        // familiile 'j' din ans formeaza o familie extinsa
        int z;
        for (int i = 1; i <= K; i++) {
            for (int j = 1; j <= N; j++) {
                infile >> z;
                if (z > 0) { 
                    ans[++cnt2] = j;
                }
            }
        }
        
    }

    void write_answer() override {
        // TODO: write the answer to the current problem (out_filename)
        
        std::ofstream outfile(out_filename);
        outfile << answer << "\n";
        
        int i = 1;
        while(i <= cnt2) {
            if (i != cnt2) {
                outfile << ans[i] << " ";
            }else {
                outfile << ans[i];
            }
        
            i++;
        }

    }
};

#endif  // TASK2_H_
