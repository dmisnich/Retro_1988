/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klee <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/08 16:12:25 by klee              #+#    #+#             */
/*   Updated: 2018/06/08 16:12:26 by klee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Game.hpp"

Game::Game(){

	std::cout << "Please input your name: ";
	std::cin >> this->_nameOfPlayer;
	std::cout << "Please input your mode. 1 - BLUE or 2 - DARK or other - DEFAUL: ";
	std::cin >> this->_flag;
	this->_bullets = nullptr;
	this->_enemies = nullptr;
	this->_stars = nullptr;
	this->_player1 = nullptr;
	this->_score = 0;
	this->_playersLives = 5;
	this->_bossLives = 0;
	this->_index = 4;
	this->_startAtGame = time(NULL);
	initscr();
	cbreak();
	noecho();
	curs_set(false);
	start_color();

	this->_win = newwin(BOX_Y_MAX, BOX_X_MAX, 0, 0);
	this->_menu = newwin(MENU_Y_MAX, MENU_X_MAX, 0, BOX_X_MAX + 1);
	
	keypad(this->_win, true);
	nodelay(this->_win, true);


	refresh();
	if (_flag > 2 || _flag < 1)
		_flag = 1;
	if (_flag == 1)
	{
		init_pair(1, COLOR_WHITE, COLOR_BLUE);
		init_pair(6, COLOR_RED, COLOR_BLUE); //bullets
		init_pair(7, COLOR_YELLOW, COLOR_BLUE); // boss
		init_pair(8, COLOR_GREEN, COLOR_BLUE);
		init_pair(9, COLOR_GREEN, COLOR_BLACK); // enemy
		wbkgd(this->_win, COLOR_PAIR(1));
	}
	else if (_flag == 2)
	{
		init_pair(2, COLOR_WHITE, COLOR_BLACK);
		init_pair(6, COLOR_RED, COLOR_BLACK); //bullets
		init_pair(7, COLOR_YELLOW, COLOR_BLACK); //boss
		init_pair(8, COLOR_GREEN, COLOR_BLACK); // enemy
		init_pair(9, COLOR_GREEN, COLOR_BLACK);
		wbkgd(this->_win, COLOR_PAIR(2));
	}	
	box(this->_win, 0, ' ');
	wrefresh(this->_win);
	
	box(this->_menu, 0, 0);
	wrefresh(this->_menu);
	
	for (int i = 0; i < NUMBER_0F_ENEMIES; i++){
		Objects * trooper = new Enemy(this->_win, 1);
		this->addObject(trooper, THIS_IS_ENEMIES);
	}

	for (int i = 0; i < NUMBER_0F_STARS; i++){
		Objects * stars = new Stars(this->_win);
		this->addObject(stars, THIS_IS_STARS);
	}
	system("afplay ./music/StarWars.mp3&");
}

Game::Game(Game const & obj){
	*this = obj;
}

Game::~Game(){
	delete this->_player1;
	this->delBulletsList();
	this->delEnemiesList();
	this->delStarsList();
	system("pkill afplay");
	endwin();	
}

Game& Game::operator= (Game const & obj){
	this->_nameOfPlayer = obj.getName();
	return *this;
}

std::string Game::getName() const{
	return this->_nameOfPlayer;
}

void		Game::NewGame(){
	this->_player1 = new Player(this->_win, BOX_Y_MAX - 2, BOX_X_MAX / 2, "^");
	this->_boss = new Boss(this->_win, this->_bossLives);
	this->_boss->setCoord(1, 1);
	Bullets * pew = nullptr;
	int c = 0;
	int count = 0;
	while (c != 27){
		c = wgetch(this->_win);	
		if (c == KEY_UP)
			_index = 1;
		else if (c == KEY_DOWN)
			_index = 7;
		else
			_index = 4;					// take input data from keyboard
		pew = this->_player1->getmv(c);				// move player and save bullets what his make
		if (this->checkShoot() == false)			// check hit
			return ;
		this->addObject(pew, THIS_IS_BULLETS);		// add bullets in list of bullets
		if (count % 6 == 0)
			this->moveEnemies();					// move enemies one time of three
		this->_player1->display();					// display change of player position
		this->_boss->moveOn();						// move boss
		if (count % 4 == 0)
			this->moveBullets();
		if (count % _index == 0)
			this->moveStars();
		if (count % 17 == 0){
			pew = this->_boss->shoot();
			this->addObject(pew, THIS_IS_BULLETS);
		}
		if (this->refresh() == false)				// if player still alive continue
			return ;
		usleep(20000);
		pew = nullptr;
		count++;
	}
}

void		Game::moveBullets(){
	t_list * tmp = this->_bullets;

	while (tmp != nullptr){
		if (tmp->obj->getYLoc() != false)
			tmp->obj->moveOn();
		tmp = tmp->next;
	}
}



void		Game::addObject(Objects * obj, int index){ // add object in t_list
	t_list * tmp = nullptr;
	
	if (obj == nullptr)
		return ;
	if (index == THIS_IS_BULLETS)
		tmp = this->_bullets;
	else if (index == THIS_IS_ENEMIES)
		tmp = this->_enemies;
	else if (index == THIS_IS_STARS)
		tmp = this->_stars;

	t_list * plus = nullptr;

	if (tmp == nullptr){
		t_list * tmp1 = new t_list;
		tmp1->obj = obj;
		tmp1->next = tmp;
		if (index == THIS_IS_BULLETS)
			this->_bullets = tmp1;
		else if (index == THIS_IS_ENEMIES)
			this->_enemies = tmp1;
		else if (index == THIS_IS_STARS)
			this->_stars = tmp1;
	}
	else{
		while(tmp->next != nullptr)
			tmp = tmp->next;
		obj->moveOn();
		plus = new t_list;
		plus->obj = obj;
		plus->next = nullptr;
		tmp->next = plus;
	}
	
}

void		Game::moveEnemies(){
	t_list * tmp = this->_enemies;
	int numbY = 0;
	int numbX = 0;
	srand (time(NULL));
	
	while (tmp != nullptr){
		if (tmp->obj->getXLoc() == false){
			numbX = rand() % (BOX_X_MAX - 3) + 1;
			numbY = (rand() % 50 + 1) * (-1);
			tmp->obj->setCoord(numbY, numbX); 			// set random coordinates for enemies
		}
		else
			tmp->obj->moveOn();
		tmp = tmp->next;
	}
}

void Game::moveStars()
{
	t_list * tmp = this->_stars;
	int numbY = 0;
	int numbX = 0;
	srand (time(NULL));
	
	while (tmp != nullptr){
		if (tmp->obj->getXLoc() == false){
			numbX = rand() % (BOX_X_MAX - 3) + 1;
			numbY = (rand() % 200 + 1) * (-1);
			tmp->obj->setCoord(numbY, numbX); 			// set random coordinates for stars
		}
		else
			tmp->obj->moveOn();
		tmp = tmp->next;
	}
}

void		Game::delBulletsList(){
	t_list * tmp;

	while (this->_bullets != nullptr){
		mvwprintw(this->_win, this->_bullets->obj->getYLoc(), this->_bullets->obj->getXLoc(), " ");
		tmp = this->_bullets;
		this->_bullets = this->_bullets->next;
		delete tmp->obj;
		delete tmp;
		tmp = nullptr;
	}
}

void		Game::delEnemiesList(){
	t_list * tmp;
	
	while (this->_enemies != nullptr){
		tmp = this->_enemies;
		this->_enemies = this->_enemies->next;
		delete tmp->obj;
		delete tmp;
		tmp = nullptr;
	}
}

void		Game::delStarsList(){
	t_list * tmp;
	
	while (this->_stars != nullptr){
		tmp = this->_stars;
		this->_stars = this->_stars->next;
		delete tmp->obj;
		delete tmp;
		tmp = nullptr;
	}
}

bool		Game::checkShoot(){
	t_list * bullets;
	Boss * boss;
	t_list * enemies = this->_enemies;
	Player * player = this->_player1;

	while (enemies != nullptr){																			// check hits in enemies 
		bullets = this->_bullets;
		while (bullets != nullptr){
			if (enemies->obj->getXLoc() && bullets->obj->getXLoc()) 									// enemy and bullet are true
				if (enemies->obj->getXLoc() == bullets->obj->getXLoc()) 								// same x location
					if (enemies->obj->getYLoc() == bullets->obj->getYLoc() || 							// same y location
					enemies->obj->getYLoc() == (bullets->obj->getYLoc() + 1)){ 							// bullet on the same vertical line, sometimes bullet fly through enemy
						mvwprintw(this->_win, enemies->obj->getYLoc(), enemies->obj->getXLoc(), " "); 	// set on previos position space
						mvwprintw(this->_win, bullets->obj->getYLoc(), bullets->obj->getXLoc(), " ");
						enemies->obj->setCoord(false, false);
						bullets->obj->setCoord(false, false);
						this->_score += 1;
					}
			bullets = bullets->next;
		}
		enemies = enemies->next;
	}

	boss = this->_boss;
	bullets = this->_bullets;

	if (this->_bossLives > 0){
		while (bullets != nullptr){
			if (boss->getXLoc() && bullets->obj->getXLoc())
				if (boss->getXLoc() == bullets->obj->getXLoc() ||
						boss->getXLoc() + 1 == bullets->obj->getXLoc() ||
							boss->getXLoc() + 2 == bullets->obj->getXLoc()) 							// three check because boss have a three character on horizontal line
					if (boss->getYLoc() == bullets->obj->getYLoc() || 
							boss->getYLoc() == (bullets->obj->getYLoc() + 1)){
						mvwprintw(this->_win, boss->getYLoc(), boss->getXLoc(), " ");
						mvwprintw(this->_win, bullets->obj->getYLoc(), bullets->obj->getXLoc(), " ");
						this->_bossLives -= 1;
						boss->setLive(this->_bossLives);
						bullets->obj->setCoord(false, false);
						this->_score += 10;
					}
			bullets = bullets->next;
		}
	}

	enemies = this->_enemies;

	while (enemies != nullptr){ 																		// check touches to player
		if (enemies->obj->getXLoc() == player->getXLoc()){
			if (enemies->obj->getYLoc() == player->getYLoc()){
				mvwprintw(this->_win, enemies->obj->getYLoc(), enemies->obj->getXLoc(), " ");
				enemies->obj->setCoord(false, false);
				player->setCoord(BOX_Y_MAX - 2, BOX_X_MAX / 2);
				this->_playersLives -= 1;
				this->_player1->setNumberOfBullets();
				this->delBulletsList();
				sleep(1);
			}
		}
		enemies = enemies->next;
	}
	
	bullets = this->_bullets;

	while (bullets != nullptr){ 																		// check hit in player
		if (bullets->obj->getXLoc() == player->getXLoc()){
			if (bullets->obj->getYLoc() == player->getYLoc()){
				mvwprintw(this->_win, bullets->obj->getYLoc(), bullets->obj->getXLoc(), " ");
				bullets->obj->setCoord(false, false);
				player->setCoord(BOX_Y_MAX - 2, BOX_X_MAX / 2);
				this->_player1->setNumberOfBullets();
				this->_playersLives -= 1;
				sleep(1);
			}
		}
		bullets = bullets->next;
	}
	return true;
}

int			Game::refresh(){
	mvwprintw(this->_menu, 1, 1, "Name: %s", this->_nameOfPlayer.c_str());				// show name of player
	mvwprintw(this->_menu, 2, 1, "Score: %05d", this->_score);				// show Score of player
	mvwprintw(this->_menu, 3, 1, "Lives: %5d", _playersLives);
	mvwprintw(this->_menu, 7, 1, "In game: %05d", (time(NULL) - this->_startAtGame));
	if (this->_player1 != nullptr)
		mvwprintw(this->_menu, 4, 1, "Bullets: %5d", this->_player1->getNumberOfBullets());
	if (this->_score > 0){
		if (this->_score % CYCLE_FOR_BOSS == 0 && this->_bossLives == 0){	// wake up BOSS
			this->_bossLives = this->_score / CYCLE_FOR_BOSS * 3;
			this->_boss->setLive(this->_bossLives);
		}
	}
	if (this->_playersLives <= 0){  									// if player die do exit
		wattron(this->_menu, COLOR_PAIR(9));
		mvwprintw(this->_menu, 9, 4, "REPLAY  y/n");
		wattroff(this->_menu, COLOR_PAIR(9));
		wrefresh(this->_menu);
		nodelay(this->_win, false);
		char c = wgetch(this->_win);
		nodelay(this->_win, true);
		if (c == 'y')
		{
			this->_playersLives = 5;
			this->_player1->setNumberOfBullets();
			mvwprintw(this->_menu, 9, 4, "           ");
			wrefresh(this->_menu);
		}
		else if (c == 'n')
			return false;
	}
	if (this->_bossLives > 0){
		mvwprintw(this->_menu, 5, 1, "BOSS");								// show info about BOSS
		mvwprintw(this->_menu, 6, 1, "Lives: %5d", this->_bossLives);
	}
	else{
		mvwprintw(this->_menu, 5, 1, "            ");
		mvwprintw(this->_menu, 6, 1, "            ", this->_bossLives);
	}
	wrefresh(this->_menu);
	wrefresh(this->_win);
	return true;
}
