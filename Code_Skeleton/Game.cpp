#include "Game.hpp"
#include "utils.hpp"
#include "ThreadP.hpp"

static const char *colors[7] = {BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN};
inline static void print_board(const char* header) ;

/*--------------------------------------------------------------------------------

--------------------------------------------------------------------------------*/
Game::Game(game_params m_gp) : m_gen_num(m_gp.n_gen), m_thread_num(m_gp.n_thread), m_thread_temp(m_gp.n_thread),
                                m_tile_hist(), m_gen_hist(),  m_threadpool(), interactive_on(m_gp.interactive_on),
                                print_on(m_gp.print_on), m_board(), m_next_board(), m_filename(m_gp.filename), m_mutex() {}


void Game::run() { // TODO in makefile: TODO i added "-pthread" SH

	_init_game(); // Starts the threads and all other variables you need
	print_board("Initial Board");
	for (uint i = 0; i < m_gen_num; ++i) {
		auto gen_start = std::chrono::system_clock::now();
		_step(i); // Iterates a single generation 
		auto gen_end = std::chrono::system_clock::now();
		m_gen_hist.push_back((float)std::chrono::duration_cast<std::chrono::microseconds>(gen_end - gen_start).count());
		print_board(NULL);
	} // generation loop
	print_board("Final Board");
	_destroy_game();
}


void Game::_init_game() {
    // Create game fields
    vector<string> temp = utils::read_lines(m_filename);
    pthread_mutex_init(&m_mutex, nullptr);

    for (auto& it : temp) {
        vector<string> v = utils::split(it, ' ');
        vector<int> v_t = vector<int>();
        for (uint i= 0 ; i < v.size(); i++)
            v_t.push_back(atoi(&(v[i][0])));
        m_board.push_back(v_t);
        m_next_board.push_back(v_t);
    }

    if (m_thread_temp > m_board.size()) {
        m_thread_num = m_board.size();
    } else {
        m_thread_num = m_thread_temp;
    }

    int row_num = m_board.size() / m_thread_num;
    int remain = m_board.size() % m_thread_num;


    // Create threads n-1 first threads
    for (int i = 0; i < (int)m_thread_num - 1; i++) {
        m_threadpool.push_back(new ThreadP(i, &m_board, &m_next_board, row_num * i, row_num, &m_tile_hist, &m_mutex));
    }
    m_threadpool.push_back(new ThreadP(m_thread_num - 1, &m_board, &m_next_board, row_num * (m_thread_num - 1),
                                       row_num + remain, &m_tile_hist, &m_mutex));

    // Start the threads // TODO ???? SH
	// Testing of your implementation will presume all threads are started here
}

void Game::_step(uint curr_gen) {
	// Push jobs to queue
	// Wait for the workers to finish calculating
	// Swap pointers between current and next field

    //Todo what we do with curr_gen
    // start threads
    for (uint i = 0; i < m_thread_num; ++i) { // TODO phase 1 SH
        m_threadpool[i]->start();
    }
    for (uint i = 0; i < m_thread_num; ++i) {
        m_threadpool[i]->join();
    }

    m_board = m_next_board; // update curr board to mid-step board represent by m_next_board

    for (uint i = 0; i < m_thread_num; ++i) { // TODO phase 2 SH
        m_threadpool[i]->start();
    }
    for (uint i = 0; i < m_thread_num; ++i) {
        m_threadpool[i]->join();
    }

    // swap old and new board // TODO does not need swapping
//    vector<vector<int>>& temp = m_board;
    m_board = m_next_board;
//    m_next_board = temp;
}

void Game::_destroy_game(){
	// Destroys board and frees all threads and resources 
	// Not implemented in the Game's destructor for testing purposes. 
	// Testing of your implementation will presume all threads are joined here
    for(auto & i : m_threadpool) {
        delete i;
    }
}

const vector<float> Game::gen_hist() const {
    return m_gen_hist;
}

const vector<float> Game::tile_hist() const {
    return m_tile_hist;
}

uint Game::thread_num() const {
    return m_thread_num;
}

/*--------------------------------------------------------------------------------

--------------------------------------------------------------------------------*/
inline void Game::print_board(const char* header) {

	if(print_on){

		// Clear the screen, to create a running animation 
		if(interactive_on)
			system("clear");

		// Print small header if needed
		if (header != NULL) {
        cout << "<------------" << header << "------------>" << endl;
    }

    uint field_height  = m_board.size();
    uint field_width = m_board[0].size();
    cout << u8"╔" << string(u8"═") * field_width << u8"╗" << endl;
    for (uint i = 0; i < field_height; ++i) {
        cout << u8"║";
        for (uint j = 0; j < field_width; ++j) {
            if (m_board[i][j] > 0)
                cout << colors[m_board[i][j] % 7] << u8"█" << RESET;
            else
                cout << u8"░";
        }
        cout << u8"║" << endl;
    }
    cout << u8"╚" << string(u8"═") * field_width << u8"╝" << endl;


		// Display for GEN_SLEEP_USEC micro-seconds on screen
		if(interactive_on)
			usleep(GEN_SLEEP_USEC);
	}

}


