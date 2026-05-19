#pragma once
#include <vector>
#include <algorithm>
#include <random>

namespace CreatureRandomizer {
	static std::vector<int> shuffled_creature_id_list = std::vector<int>();
	static std::vector<cube::Creature::Race> creature_id_blacklist = std::vector<cube::Creature::Race>({
		cube::Creature::Race::Bomb,
		cube::Creature::Race::ChristmasTree,
		cube::Creature::Race::IceCrystalDeposit,
		cube::Creature::Race::SandstoneDeposit,
		cube::Creature::Race::Dragon,
		cube::Creature::Race::Egg,
		cube::Creature::Race::Golem,
		cube::Creature::Race::Horus,
		cube::Creature::Race::Koala,
		cube::Creature::Race::OldMan,
		cube::Creature::Race::Panther,
		cube::Creature::Race::RuneGiant,
		cube::Creature::Race::Santa,
	});
	static std::vector<cube::Creature::Race> creature_id_non_chaotic_blacklist = std::vector<cube::Creature::Race>({
		cube::Creature::Race::Bush,
		cube::Creature::Race::Cactus,
		cube::Creature::Race::CobwebScrub,
		cube::Creature::Race::CottonPlant,
		cube::Creature::Race::DiamondDeposit,
		cube::Creature::Race::EmeraldDeposit,
		cube::Creature::Race::GoldDeposit,
		cube::Creature::Race::IronDeposit,
		cube::Creature::Race::SapphireDeposit,
		cube::Creature::Race::RubyDeposit,
		cube::Creature::Race::SilverDeposit,
		cube::Creature::Race::Dummy,
		cube::Creature::Race::FireScrub,
		cube::Creature::Race::Ginseng,
		cube::Creature::Race::Scarecrow,
		cube::Creature::Race::Scrub,
		cube::Creature::Race::SnowBerryBush,
		cube::Creature::Race::SnowBush,
		cube::Creature::Race::ThornTree,
		cube::Creature::Race::Vase,
	});

	static bool chaotic = false;


	// FNV-1a hash implementation
	size_t fnv1a_hash(const std::string& s) {
		size_t hash = 2166136261U; // FNV_offset_basis
		for (char c : s) {
			hash ^= static_cast<size_t>(c);
			hash *= 16777619U; // FNV_prime
		}
		return hash;
	}

	static std::vector<int> GenerateNonShuffledMonsterList() {
		std::vector<int> id_list = std::vector<int>();
		for (int i = 0; i < (int) cube::Creature::Race::Count; i++) {
			if (std::find(creature_id_blacklist.begin(), creature_id_blacklist.end(), (cube::Creature::Race)i) != creature_id_blacklist.end()) continue;
			if (chaotic == false && std::find(creature_id_non_chaotic_blacklist.begin(), creature_id_non_chaotic_blacklist.end(), (cube::Creature::Race)i) != creature_id_non_chaotic_blacklist.end()) continue;
			id_list.push_back(i);
		}
		return id_list;
	}

	static void InitRandomizedMonsterList() {
		std::vector<int> id_list = GenerateNonShuffledMonsterList();
		//AP_RoomInfo roomInfo = AP_RoomInfo();
		//AP_GetRoomInfo(&roomInfo);
		std::shuffle(id_list.begin(), id_list.end(), std::default_random_engine(2)); // fnv1a_hash(roomInfo.seed_name)
		// Apply the shuffled list while keeping track of the blacklisted ids.
		shuffled_creature_id_list.clear();
		int offset = 0;
		for (int i = 0; i < (int)cube::Creature::Race::Count; i++) {
			if (
				(std::find(creature_id_blacklist.begin(), creature_id_blacklist.end(), (cube::Creature::Race)i) != creature_id_blacklist.end()) ||
				(chaotic == false && std::find(creature_id_non_chaotic_blacklist.begin(), creature_id_non_chaotic_blacklist.end(), (cube::Creature::Race)i) != creature_id_non_chaotic_blacklist.end())
			) {
				shuffled_creature_id_list.push_back(i);
				offset++;
			};
			shuffled_creature_id_list.push_back(id_list.at(i - offset));
		}
	}

	static int GetRandomRaceFromID(int race) {
		if (shuffled_creature_id_list.empty()) return race;
		if (shuffled_creature_id_list.size() <= race) return race;
		return shuffled_creature_id_list.at(race);
	}
}