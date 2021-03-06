/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
**                                                                              **
**  This file is a part of the ENIGMA Development Environment.                  **
**                                                                              **
**                                                                              **
**  ENIGMA is free software: you can redistribute it and/or modify it under the **
**  terms of the GNU General Public License as published by the Free Software   **
**  Foundation, version 3 of the license or any later version.                  **
**                                                                              **
**  This application and its source code is distributed AS-IS, WITHOUT ANY      **
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
**  details.                                                                    **
**                                                                              **
**  You should have recieved a copy of the GNU General Public License along     **
**  with this code. If not, see <http://www.gnu.org/licenses/>                  **
**                                                                              **
**  ENIGMA is an environment designed to create games and other programs with a **
**  high-level, fully compilable language. Developers of ENIGMA or anything     **
**  associated with ENIGMA are in no way responsible for its users or           **
**  applications created by its users, or damages caused by the environment     **
**  or programs made in the environment.                                        **
**                                                                              **
\********************************************************************************/

/*************************************************************
GM Global variables
******************/

bool argument_relative=false;
string caption_score="Score:", caption_lives="Lives:", caption_health="Health:";

/*
global:     cursor_sprite
global:     error_last
global:     error_occurred
global:     event_action
global:     event_number
global:     event_object
global:     event_type*/
double      fps;

double health=100;
#ifndef JUST_DEFINE_IT_RUN
#include <deque>
deque<int> instance_id;
#else
int *instance_id;
#endif

int keyboard_key=0;
/*global:     keyboard_lastchar
global:     keyboard_lastkey */
string keyboard_string="";

double lives=3;
double score=0;
bool secure_mode=false;
bool show_score=0, show_lives=0, show_health=0;

//string temp_directory="";
int transition_kind=0;
int transition_steps=80;
/*global:     transition_time
global:  working_directory*/
bool automatic_redraw = true;
string program_directory="";
int os_type;
int gamemaker_version=0;
//int transition_steps;
int cursor_sprite;
extern int room_first, room_last;

/*********************
End GM global variables
 *********************/
