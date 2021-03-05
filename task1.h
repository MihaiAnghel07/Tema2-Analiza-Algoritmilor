// Copyright 2020
// Authors: Radu Nichita, Matei Simtinică

#ifndef TASK1_H_
#define TASK1_H_

#include "task.h"

/*
 * Task1
 * You have to implement 4 methods:
 * read_problem_data         - read the problem input and store it however you see fit
 * formulate_oracle_question - transform the current problem instance into a SAT 
 							   instance and write the oracle input
 * decipher_oracle_answer    - transform the SAT answer back to the current problem's 
 							   answer
 * write_answer              - write the current problem's answer
 */
class Task1 : public Task {
 private:
    // TODO: define necessary variables and/or data structures
 	int N;
 	int M;
    int K;	
    int u[4000];
   	int v[4000]; 
    char answer[6];
    int ans[1000];
    int n2;

 public:
    void solve() override {
        read_problem_data();
        formulate_oracle_question();
        ask_oracle();
        decipher_oracle_answer();
        write_answer();
    }

    void read_problem_data() override {
        // TODO: read the problem input (in_filename) and store the data 
        // in the object's attributes
    	
    	std::ifstream infile(in_filename);
    	infile >> N >> M >> K;

		// citesc si adaug relatiile dintre familii
		int i = 1;
		while (infile >> u[i] >> v[i]) {
			i++;
		}

		infile.close();

    }

    void formulate_oracle_question() {
        // TODO: transform the current problem into a SAT problem and write it 
        // (oracle_in_filename) in a format understood by the oracle

    	int x[N + 1][K], cnt = 1;
    	std::ofstream outfile(oracle_in_filename);

    	// numarul variabilelor este N * K (N posibile familii cu K spioni posibili)
    	// din prima constrangere: N clauze
    	// din a doua constrangere: N * (K - 1) * K / 2
    	// din a treia constrangere: K * M clauze
    	// in total: N + N * K * (K - 1) / 2 + K * M clauze

    	outfile << "p cnf ";
    	outfile << N*K << " "; 
    	outfile << (N + N*K*(K - 1)/2 + K*M); 
    	outfile << "\n";

    	// formez variabilele
    	// linia 'i' va reprezenta familia 'i'
    	// coloana 'j' va reprezenta spionul 'j'
    	// se creeaza cate o variabila pentru fiecare posibila asignare 
    	// a spionului 'j' cu familia 'i'

    	for (int i = 1; i <= N; i++) {
    		for (int j = 1; j <= K; j++) {
    			x[i][j] = cnt;
    			cnt++;
    		}
    	}

    	// sunt 3 constrangeri pe care trebuie sa le respectam pentru 
    	// a alcatui corect clauzele

    	// in fiecare familie trebuie sa existe cel putin un spion infiltrat
    	for (int i = 1; i <= N; i++) {
    		for (int j = 1; j <= K; j++) {
    			outfile << x[i][j] << " ";
    		}
    		outfile << "0\n";
    	}

    	// in fiecare familie trebuie sa existe cel mult un spion infiltrat
    	for (int i = 1; i <= N; i++) {
    		for (int j = 1; j <= K - 1; j++) {
    			for (int l = j + 1; l <= K; l++) {
    				outfile << -x[i][j] << " " << -x[i][l] << " 0\n";
    			}
    		}
    	}

    	// in familiile care se inteleg trebuie sa fie infiltrati spioni
    	// diferiti 
    	for (int j = 1; j <= K; j++) {
    		for (int i = 1; i <= M; i++) {
    			outfile << -x[u[i]][j] << " " << -x[v[i]][j] << " 0\n";
    		}
    	}
    
    	outfile.close();

    }

    void decipher_oracle_answer() {
        // TODO: extract the current problem's answer from the answer given 
        // by the oracle (oracle_out_filename)
    	
    	std::ifstream infile(oracle_out_filename);

    	// answer = true / false
    	// ans = repartizarea spionilor in familii
    	// n2 = numarul variabilelor formulei

    	infile >> answer;
    	infile >> n2;
    	
    	int z;
    	for (int i = 1; i <= n2/K; i++) {
    		for (int j = 1; j <= K; j++) {
    			infile >> z;
    			if (z > 0) { 
    				// spionul 'j' este repartizat familiei 'i'
    				ans[i] = j;
    			}
    		}
    	}

    	
    }

    void write_answer() override {
        // TODO: write the answer to the current problem (out_filename)

    	std::ofstream outfile(out_filename);
    	outfile << answer << "\n";
    	
    	// N este echivalent cu n2/k
    	int i = 1;
    	while(i <= N) {
    		if (i != N) {
    			outfile << ans[i] << " ";
			}else {
				outfile << ans[i];
			}
    		i++;
    	}

    }
};

#endif  // TASK1_H_
