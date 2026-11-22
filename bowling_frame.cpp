#include "bowling_frame.h"
#include <random>

std::random_device dev;
std::mt19937 rng(dev());

Bowling_Frame::Bowling_Frame(int fnum)
{
	score1_ = 0;
	score2_ = 0;
	score_3 = 0;
	strike_ = false;
	spare_ = false;
	frame_num_ = fnum;
	frame_score_ = 0;
	bool roll_one_ = false;
	bool roll_two_ = false;
	bool roll_three_ = false;

	next_frame_ = NULL;
	prev_frame_ = NULL;

}

Bowling_Frame::~Bowling_Frame()
{
	
}

Bowling_Frame* Bowling_Frame::next_frame() { return next_frame_; }
void Bowling_Frame::next_frame( Bowling_Frame * new_frame ) { next_frame_ = new_frame; }

Bowling_Frame* Bowling_Frame::prev_frame() { return prev_frame_; }
void Bowling_Frame::prev_frame( Bowling_Frame * new_frame ) { prev_frame_ = new_frame; }

int Bowling_Frame::score1()				{ return score1_; }
void Bowling_Frame::score1( int score )	{ score1_ = score; }

int Bowling_Frame::score2()				{ return score2_; }
void Bowling_Frame::score2( int score )	{ score2_ = score; }

int Bowling_Frame::score3()				{ return score3_; }
void Bowling_Frame::score3( int score ) {score3_ = score; }

bool Bowling_Frame::strike() 			{ return strike_; }
void Bowling_Frame::strike(bool value )	{ strike_ = value; }

bool Bowling_Frame::spare() 			{ return spare_; }
void Bowling_Frame::spare(bool value )	{ spare_ = value; }

int Bowling_Frame::frame_num()
{
	return frame_num_;
}

int Bowling_Frame::frame_score()
{
	return frame_score_; 
}

void Bowling_Frame::update_scores()
{
	std::uniform_int_distribution<std::mt19937::result_type> dist10(0, 10);

	if (!roll_one_)
	{
		score1_ = dist10(rng);
		roll_one_ = true;

		if(score1_ == 10) strike_ = true; 
	}
	else if (!roll_two_ && (!strike_ || frame_num_ == 10)) 	
	{
		int max_pins = (frame_num_ == 10 && strike_) ? 10 : (10 - score1_);
		std::uniform_int_distribution<std::mt19937::result_type> distX(0, max_pins);
		score2_ = distX(rng); 
		roll_two_ = true;

		if (!strike_ && (score1_ + score2_) == 10) spare_ = true;
	}
	else if (frame_num_ == 10 && (strike_ || spare_) && !roll_three_)
	{
		int max_pins = 10;
		if (strike_ && score2_ > 0 && score2_ < 10)
		{
			max_pins = 10 - score2_;
		}
		std::uniform_int_distribution<std::mt19937::result_type> dist3(0, max_pins);
		score3_ = dist3(rng);
		roll_three_ = true;
	}

	else if (next_frame_) 
	{
		next_frame_->update_scores();
	}
	if (frame_num_ == 10)
	{
		frame_score_ = score1_ + score2_ + score3_;
	}
	else if (strike_)
	{
		if (next_frame_)
		{
			if (next_frame_->strike() && next_frame_->frame_num() < 10 && next_frame_->next_frame())
			{
				frame_score_ = 10 + next_frame_->score1() + next_frame_->next_frame()->score1();
			}
			else
			{
				frame_score_ = 10 + next_frame_->score1() + next_frame_->score2();
			}
		}
	}
	else if (spare_)
	{
		if (next_frame_)
		{
			frame_score_ = 10 + next_frame_->score1();
		}
	}
	else
	{
		frame_score_ = score1_ + score2_;
	}
}
