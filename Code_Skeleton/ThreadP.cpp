
#include "Game.hpp"
#include "utils.hpp"
#include "ThreadP.hpp"
#include "Thread.hpp"


ThreadP::ThreadP(uint thread_id, vector<vector<int>>* curr_board, vector<vector<int>>* next_board, int start_line,
                 int num_of_line) : Thread(thread_id), curr_board(curr_board), next_board(next_board),
                 start_line(start_line), num_of_line(num_of_line), time(0) {};

void ThreadP::thread_workload() {
//    cout << "start the work of thread number " << m_thread_id << endl; // TODO printing for testing only
    auto start = std::chrono::system_clock::now();

    for (int i = start_line; i < start_line+num_of_line; ++i) {
        for (int j = 0; j < (*curr_board)[i].size(); ++j) {
            int bottom = 0 > i-1 ? 0 : i-1;
            int top = curr_board->size()-1 < i+1 ? curr_board->size()-1 : i+1;
            int left = 0 > j-1 ? 0 : j-1;
            int right = (*curr_board)[i].size()-1 < j+1 ? (*curr_board)[i].size()-1 : j+1;

            int num_of_neighbours = 0;
            int colors_hist [8] = {0,0,0,0,0,0,0,0};
            for (int k = bottom; k <= top; ++k) { // count num of alive neighbours
                for (int l = left; l <= right; ++l) {
                    if (k == i && l == j) {
                        continue;
                    }
                    else if ((*curr_board)[k][l] != 0) {
                        if (++num_of_neighbours <= 3) {
                            int tmppp = ((*curr_board)[k][l]); // TODO
                            colors_hist[((*curr_board)[k][l])]++; // mapping dominant species of neighbours
                        }
                    }
                }
            }

            if ((*curr_board)[i][j] == 0 && num_of_neighbours == 3) {
                int dominant_species = 0;
                int max = 0;
                for (int k = 0; k < 8; ++k) {
                    if (colors_hist[k] > max) {
                        max = colors_hist[k];
                        dominant_species = k;
                    }
                    (*next_board)[i][j] = dominant_species;
                }
            }
            else if ((*curr_board)[i][j] != 0 && num_of_neighbours != 2 && num_of_neighbours != 3) {
                (*next_board)[i][j] = 0;
            }
        }
    }

    auto end = std::chrono::system_clock::now();
    time = start-end;

//    cout << "finish the work of thread number " << m_thread_id << endl;  // TODO printing for testing only

}

std::chrono::duration<float> ThreadP::get_iter_time() {
    return time;
}
