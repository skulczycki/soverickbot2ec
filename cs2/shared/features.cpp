#include "shared.h"

#include <iostream>
#include <fstream>

//
// private data. only available for features.cpp
//

namespace features
{
	//
	// global shared variables
	// can be used at any features.cpp function
	//
	cs::WEAPON_CLASS weapon_class;
	BOOL             b_aimbot_button;
	BOOL             b_triggerbot_button;


	//
	// triggerbot
	//
	static DWORD mouse_down_ms;
	static DWORD mouse_up_ms;

	//
	// rcs
	//
	static vec2  rcs_old_punch;

	//
	// aimbot
	//
	static BOOL  aimbot_active;
	static QWORD aimbot_target;
	static int   aimbot_bone;
	static DWORD aimbot_ms;

	//
	// infov event state
	//
	static BOOL event_state;


	void reset(void)
	{
		mouse_down_ms    = 0;
		mouse_up_ms      = 0;
		rcs_old_punch    = {};
		aimbot_active    = 0;
		aimbot_target    = 0;
		aimbot_bone      = 0;
		aimbot_ms        = 0;
	}
}

namespace config
{
	static BOOL  rcs;
	static BOOL  aimbot_enabled;
	static DWORD aimbot_button;
	static float aimbot_fov;
	static float aimbot_smooth;
	static BOOL  aimbot_multibone;
	static DWORD triggerbot_button;
	static BOOL  visuals_enabled;
}

inline DWORD random_number(DWORD min, DWORD max)
{
	return min + cs::engine::get_current_ms() % (max + 1 - min);
}


void features::run(void)
{
	QWORD local_player_controller = cs::entity::get_local_player_controller();
	if (local_player_controller == 0)
	{
		return; // Not in game
	}

	QWORD local_player = cs::entity::get_player(local_player_controller);

	
	if (local_player == 0)
	{
		return; // Not in game
	}

	weapon_class = cs::player::get_weapon_class(local_player);
	if (weapon_class == cs::WEAPON_CLASS::Invalid)
	{
		return; // Not in game
	}

	dump_player_positions(local_player, local_player_controller);
}

/*

Vec2 RevolveCoordinatesSystem(float RevolveAngle, Vec2 OriginPos, Vec2 DestPos)
{
	Vec2 ResultPos;
	if (RevolveAngle == 0)
		return DestPos;
	ResultPos.x = OriginPos.x + (DestPos.x - OriginPos.x) * cos(RevolveAngle * M_PI / 180) + (DestPos.y - OriginPos.y) * sin(RevolveAngle * M_PI / 180);
	ResultPos.y = OriginPos.y - (DestPos.x - OriginPos.x) * sin(RevolveAngle * M_PI / 180) + (DestPos.y - OriginPos.y) * cos(RevolveAngle * M_PI / 180);
	return ResultPos;

	https://github.com/clauadv/cs2_webradar/blob/main/react/public/data/de_dust2/data.json
}


*/

void features::dump_player_positions(QWORD local_player, QWORD local_controller)
{
	//std::cout << "-------------------------------" << std::endl;
	std::ofstream pos_file;
  	pos_file.open("/root/git/soverickbot2ec/projects - CS2/GUI/player_pos.json");
	pos_file << "{" << std::endl;
	bool first = true;
	for (int i = 1; i < 32; i++)
	{
		QWORD ent = cs::entity::get_client_entity(i);
		// if (ent == 0 || (ent == local_controller))
		// {
		// 	continue;
		// }

		//
		// is controller
		//
		if (!cs::entity::is_player(ent))
		{
			continue;
		}

		QWORD player = cs::entity::get_player(ent);
		if (player == 0)
		{
			continue;
		}

		QWORD node = cs::player::get_node(player);
		if (node == 0)
		{
			continue;
		}

		if (!cs::player::is_valid(player, node))
		{
			continue;
		}

		vec3 head{};
		if (!cs::node::get_bone_position(node, 6, &head))
		{
			continue;
		}

		QWORD teamid = cs::player::get_team_num(player);

		//std::cout << teamid << ": (" << head.x << ", " << head.y << ")" << std::endl;

		// std::cout << i << "," << teamid << "," << head.x << ", " << head.y << std::endl;
		if (!first) { 
			pos_file << "," << std::endl; 
		}
		pos_file << "	\"" << i << "\": { ";
		pos_file << "\"team\": " << teamid << ",";
		pos_file << "\"x\": " << head.x << ",";
		pos_file << "\"y\": " << head.y << "}";

		first = false;
	}
	pos_file << "}" << std::endl;
	pos_file.close();
}

