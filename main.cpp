#include <iostream>
#include "bowling_frame.h"
#include <random>
#include <cstdlib>

using namespace std;

std::random_device dev_main;
std::mt19937 rng_main(dev_main());

int current_game_total(Bowling_Frame * first_frame)
{
	Bowling_Frame * temp = first_frame;
	int game_total = 0;
	int temp_total = 0;
	while(temp)
	{
		game_total += temp->frame_score();
		temp = temp->next_frame();
	}
	return game_total;	
}

int main(int argc, char* argv[])
{

	if (argc != 2) {
		cout << "Usage: " << argv[0] << " <num_players>" << endl;
		return 1;
	}

	int num_players = atoi(argv[1]);
	cout << "Player count is: " << num_players << endl;

	Bowling_Frame** players = new Bowling_Frame*[num_players];

	for (int i = 0; i < num_players; i++) {
		players[i] = new Bowling_Frame(1);
		Bowling_Frame* current = players[i];

		for (int frame_num = 2; frame_num <= 10; frame_num++) {
			Bowling_Frame* new_frame = new Bowling_Frame(frame_num);
			current->next_frame(new_frame);
			new_frame->prev_frame(current);
			current = new_frame;
		}
	}

	for (int i = 0; i < num_players; i++) {
		for (int roll = 0; roll < 21; roll++) {
			players[i]->update_scores();
		}
	}


	for (int i = 0; i < num_players; i++) {
		Bowling_Frame * traverse = players[i];

		while (traverse) {
			out << "Frame " << traverse->frame_num() << ":" << endl
			 	 << " Score 1: " << traverse->score1() << endl
			 	 << " Score 2: " << traverse->score2() << endl;

			traverse = traverse->next_frame();
		}


		cout << "Total Game Score for player " << (i + 1) << ": " << current_game_total(players[i]) << endl;
	}

	for (int i = 0; i < num_players; i++) {
		Bowling_Frame* current = players[i];
		while (current) {
			Bowling_Frame* next = current->next_frame();
			delete current;
			current = next;
		}
	}

	delete[] players;
	return 0;
}
