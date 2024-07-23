// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

/**
 * Custom Atcommands
 * Place the body of custom atcommands in this file.
 * Format:
 *	ACMD_FUNC(command_name)
 *	{
 *		<code>;
 *	}
 **/

//ACMD_FUNC(newcommand)
//{
//	clif_displaymessage(fd, "It works!");
//	clif_specialeffect(&sd->bl, EF_HEARTCASTING, AREA);
//	return 0;
//}

ACMD_FUNC(battlestats)
{

    const char *elements[] = {
        "Neutral",
        "Water",
        "Earth",
        "Fire",
        "Wind",
        "Poison",
        "Holy",
        "Dark",
        "Ghost",
        "Undead",
        "All"};
    const char *status_names[] = {
        "Stone Curse",
        "Freeze",
        "Stun",
        "Sleep",
        "Poison",
        "Curse",
        "Silence",
        "Confusion",
        "Blind",
        "Bleeding"};
    const char *races[] = {
        "Formless",
        "Undead",
        "Brute",
        "Plant",
        "Insect",
        "Fish",
        "Demon",
        "Demihuman",
        "Angel",
        "Dragon",
        "Player",
        "All"};
    std::string buf;

    nullpo_retr(-1, sd);

    buf.append("@battlestats - Personal Infos :\n");

    buf.append("      Name : ");
    buf.append(sd->status.name);
    buf.append("\n");
    buf.append("      Base Lv. ");
    buf.append(std::to_string(sd->status.base_level));
    buf.append("\n");
    buf.append("      Job Lv. ");
    buf.append(std::to_string(sd->status.job_level));
    buf.append("\n");

    buf.append("Battlestats :\n");

    buf.append("      Base ATK ");
    buf.append(std::to_string(sd->battle_status.batk));
    buf.append("\n");

    if (sd->battle_status.def_ele > ELE_NONE && sd->battle_status.def_ele < ELE_MAX)
    {
        buf.append("      Armor Element ");
        buf.append(elements[sd->battle_status.def_ele]);
        buf.append(" Lv ");
        buf.append(std::to_string(sd->battle_status.ele_lv));
        buf.append("\n");
    }
    buf.append("      Size ");
    buf.append(std::to_string(sd->battle_status.size));
    if (sd->battle_status.race > RC_NONE_ && sd->battle_status.race < RC_MAX)
    {
        buf.append(",      Race ");
        buf.append(races[sd->battle_status.race]);
    }
    buf.append("\n");

    // LH Weapon
    if (sd->battle_status.lhw.atk)
    {
        buf.append("Right-Hand Weapon :\n");
        buf.append("      ATK ");
        buf.append(std::to_string(sd->battle_status.lhw.atk));
        buf.append(", Refine ATK ");
        buf.append(std::to_string(sd->battle_status.lhw.atk2));
#ifdef RENEWAL
        buf.append(", MATK ");
        buf.append(std::to_string(sd->battle_status.lhw.matk));
#endif
        buf.append("\n");
        buf.append("      Range ");
        buf.append(std::to_string(sd->battle_status.lhw.range));
#ifdef RENEWAL
        buf.append(", Weapon Level ");
        buf.append(std::to_string(sd->battle_status.lhw.wlv));
#endif
        buf.append("\n");

        if (sd->battle_status.lhw.ele > ELE_NONE && sd->battle_status.lhw.ele < ELE_MAX)
        {
            buf.append("      Element ");
            buf.append(elements[sd->battle_status.lhw.ele]);
            buf.append("\n");
        }
    }
    // RH Weapon
    if (sd->battle_status.rhw.atk)
    {
        buf.append("Right-Hand Weapon :\n");
        buf.append("      ATK ");
        buf.append(std::to_string(sd->battle_status.rhw.atk));
        buf.append(", Refine ATK ");
        buf.append(std::to_string(sd->battle_status.rhw.atk2));
#ifdef RENEWAL
        buf.append(", MATK ");
        buf.append(std::to_string(sd->battle_status.rhw.matk));
#endif
        buf.append("\n");

        buf.append("      Range ");
        buf.append(std::to_string(sd->battle_status.rhw.range));
#ifdef RENEWAL
        buf.append(", Weapon Level ");
        buf.append(std::to_string(sd->battle_status.rhw.wlv));
#endif
        buf.append("\n");

        if (sd->battle_status.rhw.ele > ELE_NONE && sd->battle_status.rhw.ele < ELE_MAX)
        {
            buf.append("      Element ");
            buf.append(elements[sd->battle_status.rhw.ele]);
            buf.append("\n");
        }
    }

    buf.append("ASPD Stats :\n");
    struct status_change *sc = status_get_sc(&sd->bl);
    struct status_data *b_status = status_get_base_status(&sd->bl); // Base Status
    struct status_data *status = status_get_status_data(&sd->bl);

#ifdef RENEWAL_ASPD
    buf.append("      ");
    buf.append("Item ASPD Limit +");
    buf.append(std::to_string(sd->bonus.aspd_cap ));
    buf.append("\n");
#endif
    buf.append("      ");
    buf.append("Items Bonuses ASPD +");
    buf.append(std::to_string(sd->bonus.aspd_add * (-1) / 10));
    buf.append("\n");
#ifdef RENEWAL_ASPD
    buf.append("      ");
    buf.append("Items Bonuses ASPD +");
    buf.append(std::to_string(status->aspd_rate2));
    buf.append("%\n");
    buf.append("      ");
    buf.append("Statuses Bonuses ASPD +");
    buf.append(std::to_string(status_calc_aspd(&sd->bl, sc, false)));
    buf.append("%\n");
#endif

    buf.append("      ");
    buf.append("Final ASPD ");
    buf.append(std::to_string(static_cast<int>(floor((2000 - status->amotion) / 10))));
    buf.append("\n");

    buf.append("Misc Resistances :\n");
    for (int16 type = SC_COMMON_MIN; type < SC_COMMON_MAX - 1; type++)
    {
        int resistance = 0, rate = 0;
        for (const auto &it : sd->reseff)
        {
            if (it.id == type)
                rate = it.val;
        }
        resistance += rate / 100;
        if (sd->sc.data[SC_COMMONSC_RESIST])
            resistance += sd->sc.data[SC_COMMONSC_RESIST]->val1;
        if (resistance > 0)
        {
            buf.append("      ");
            buf.append(status_names[type]);
            buf.append(" = ");
            buf.append(std::to_string(resistance));
            buf.append("%\n");
        }
    }
    buf.append("Skill Boosts :\n");
    for (const auto &it : sd->skillatk)
    {
        std::shared_ptr<s_skill_db> skill = skill_db.find(it.id);
        if (skill != nullptr)
        {
            buf.append("      Increase the damage of '");
            buf.append(skill->desc);
            buf.append("' by ");
            buf.append(std::to_string(it.val));
            buf.append("%\n");
        }
    }
    buf.append("Drop Boosts :\n");
    for (const auto &it : sd->add_drop)
    {
        struct item_data *item_data = NULL;
        int rate = it.rate / 100;
        if ((item_data = itemdb_exists(it.nameid)) != NULL)
        {
            buf.append("      Drop rate of '");
            buf.append(item_data->name);
            buf.append("' increased by ");
            buf.append(std::to_string(rate));
            buf.append("%");
            if (it.race)
            {
                buf.append(" on race ");
                buf.append(races[it.race]);
            }
            buf.append("\n");
        }
    }
    buf.append("Auto Spells :\n");
    for (auto &it : sd->autospell)
    {
        int id = 0;
        if (it.id < 0)
            id = -it.id;
        else
            id = it.id;
        int rate = it.rate/10;
        std::shared_ptr<s_skill_db> skill = skill_db.find(id);
        if (skill != nullptr)
        {
            buf.append("      Auto spell skill '");
            buf.append(skill->desc);
            buf.append("' level ");
            buf.append(std::to_string(it.lv));
            buf.append(" by ");
            buf.append(std::to_string(rate));
            buf.append("% when attacking");
            buf.append("\n");
        }
    }
    for (auto &it : sd->autospell2)
    {
        int id = 0;
        if (it.id < 0)
            id = -it.id;
        else
            id = it.id;
        int rate = it.rate;
        std::shared_ptr<s_skill_db> skill = skill_db.find(id);
        if (skill != nullptr)
        {
            buf.append("      Auto spell skill '");
            buf.append(skill->desc);
            buf.append("' level ");
            buf.append(std::to_string(it.lv));
            buf.append(" by ");
            buf.append(std::to_string(rate));
            buf.append("% when hit");
            buf.append("\n");
        }
    }
    for (auto &it : sd->autospell3)
    {
        int id = 0;
        if (it.id < 0)
            id = -it.id;
        else
            id = it.id;
        int rate = it.rate;
        std::shared_ptr<s_skill_db> skill = skill_db.find(id);
        if (skill != nullptr)
        {
            buf.append("      Auto spell skill '");
            buf.append(skill->desc);
            buf.append("' level ");
            buf.append(std::to_string(it.lv));
            buf.append(" by ");
            buf.append(std::to_string(rate));
            buf.append("% when skill '");
            buf.append(skill->desc);
            buf.append("' is used\n");
        }
    }

#ifdef RENEWAL_CAST
    float fixtime_red = fminf(100, skill_vfcastfix(&sd->bl, 1, 1, 1, 2));
    float varcast_statuses = fminf(100, skill_vfcastfix(&sd->bl, 1, 1, 1, 3));
    float varcast_bonuses = fminf(100, skill_vfcastfix(&sd->bl, 1, 1, 1, 4));
    float varcast_basic = fminf(100, skill_vfcastfix(&sd->bl, 1, 1, 1, 5));
    float total = fminf(100, varcast_basic + varcast_statuses + varcast_bonuses);

    buf.append("Cast Time Reduction :\n");

    buf.append("      Variable = ");
    buf.append(std::to_string(total).substr(0, 4));
    buf.append("% (Stats: ");
    buf.append(std::to_string(varcast_basic).substr(0, 4));
    buf.append("%, Buffs: ");
    buf.append(std::to_string(varcast_statuses).substr(0, 4));
    buf.append("%, Bonuses: ");
    buf.append(std::to_string(varcast_bonuses).substr(0, 4));
    buf.append("%)\n");
    buf.append("      Fixed = ");
    buf.append(std::to_string(fixtime_red).substr(0, 4));
    buf.append("%\n");
#endif

    buf.append("--------------------------------------------------------\n");
    buf.append("    Bonuses ~ ATK/DEF \n");
    buf.append("--------------------------------------------------------\n");
    int64 atkPercentage = pc_readparam(sd, SP_ATK_RATE);
    if (!sd->state.lr_flag || sd->state.lr_flag == 3)
        atkPercentage += sd->right_weapon.addclass[CLASS_ALL];
    else if (sd->state.lr_flag == 1)
        atkPercentage += sd->left_weapon.addclass[CLASS_ALL];
    buf.append("ATK (%) : ");
    buf.append(std::to_string(atkPercentage));
    buf.append("%     MATK (%) : ");
    buf.append(std::to_string(pc_readparam(sd, SP_MATK_RATE) - 100));
    buf.append("%     LONG ATK (%) : ");
    buf.append(std::to_string(pc_readparam(sd, SP_LONG_ATK_RATE)));
    buf.append("%\n");
    buf.append("CRITICAL (%) : ");
    buf.append(std::to_string((pc_readparam(sd, SP_CRITICAL_RATE) - 100)));
    buf.append("%      CRITICAL ATK (%) : ");
    buf.append(std::to_string(pc_readparam(sd, SP_CRIT_ATK_RATE)));
    buf.append("%\n");
    buf.append("HIT (%) : ");
    buf.append(std::to_string((pc_readparam(sd, SP_HIT_RATE) - 100)));
    buf.append("%      FLEE (%) : ");
    buf.append(std::to_string((pc_readparam(sd, SP_FLEE_RATE) - 100)));
    buf.append("%     PERFECT DODGE (%) : ");
    buf.append(std::to_string((pc_readparam(sd, SP_FLEE2_RATE) - 100)));
    buf.append("%\n");
    buf.append("MOV. SPEED (%) : ");
    buf.append(std::to_string(pc_readparam(sd, SP_SPEED_ADDRATE)));
    buf.append("%     ASPD (%) : ");
    buf.append(std::to_string(pc_readparam(sd, SP_ASPD_RATE)));
    buf.append("%\n");
    buf.append("''ASPD (%) Not based on the final value but rather on the equation.''\n\n");
    buf.append("EQUIP DEF (%) : ");
    buf.append(std::to_string((pc_readparam(sd, SP_DEF_RATE) - 100)));
    buf.append("%      MDEF (%) : ");
    buf.append(std::to_string((pc_readparam(sd, SP_MDEF_RATE) - 100)));
    buf.append("%\n");
    buf.append("REDUCT. LONG ATK (%) : ");
    buf.append(std::to_string(pc_readparam(sd, SP_LONG_ATK_DEF)));
    buf.append("%     MELEE ATK (%) : ");
    buf.append(std::to_string(pc_readparam(sd, SP_NEAR_ATK_DEF)));
    buf.append("%\n");
    buf.append("REDUCT. MATK (%) : ");
    buf.append(std::to_string(pc_readparam(sd, SP_MAGIC_ATK_DEF)));
    buf.append("%     MISC ATK (%) : ");
    buf.append(std::to_string(pc_readparam(sd, SP_MISC_ATK_DEF)));
    buf.append("%\n");
    buf.append(" \n");
    buf.append("--------------------------------------------------------\n");
    buf.append("    Bonuses ~ Healing, HP/SP\n");
    buf.append("--------------------------------------------------------\n");
    buf.append("Healing Received (%) : ");
    buf.append(std::to_string(sd->bonus.add_heal2_rate));
    buf.append("%\n");
    buf.append("Leech Rate (%) : ");
    buf.append(std::to_string(sd->right_weapon.hp_drain_rate.rate/10));
    buf.append("%\n");
    buf.append("Leech Power (%) : ");
    buf.append(std::to_string(sd->right_weapon.hp_drain_rate.per));
    buf.append("%\n");
    buf.append("Regen HP (%) : ");
    buf.append(std::to_string((sd->hprecov_rate - 100)));
    buf.append("%\n");
    buf.append("Regen SP (%) : ");
    buf.append(std::to_string((sd->sprecov_rate - 100)));
    buf.append("%\n");
    buf.append("SP Consumption (%) : ");
    buf.append(std::to_string((sd->dsprate - 100)));
    buf.append("% \n");
    buf.append(" \n");
    buf.append("--------------------------------------------------------\n");
    buf.append("    Bonuses ~ Cast time/delay\n");
    buf.append("--------------------------------------------------------\n");
#ifdef RENEWAL_CAST
    buf.append("Variable Cast (%)   : -");
    buf.append(std::to_string(sd->bonus.varcastrate));
    buf.append("%\n");
#else
    buf.append("SKILL CAST TIME (%)  : - ");
    buf.append(std::to_string(sd->castrate));
    buf.append("%\n");
#endif
    buf.append("Fixed Cast (%) :  ");
    buf.append(std::to_string(sd->bonus.fixcastrate));
    buf.append("%\n");
    buf.append("Fixed Cast Time (ms)  : ");
    buf.append(std::to_string((sd->bonus.add_fixcast / 100)));
    buf.append("00s\n");
    buf.append("After Cast Delay (%)  : ");
    buf.append(std::to_string((-1) * sd->bonus.delayrate));
    buf.append("%\n");
    buf.append("--------------------------------------------------------\n");
    buf.append("Complete Battle Report for the Orphan.\n");
    clif_displaymessage(fd, buf.c_str());
    return 0;
}


/*=========================================
* Check values of resistance [keitenai]
*-----------------------------------------*/
ACMD_FUNC(resist) {
	char resis_state[100];
	char info[CHAT_SIZE_MAX];
	int j, count = 0;
	struct {
		const char* format;
		int value;
	} info_table[] = {
		{ NULL, 0 },
		{ "   [ %d ] Small size resist", 0 }, // 1
		{ "   [ %d ] Medium size resist", 0 },
		{ "   [ %d ] Large size resist", 0 },

		{ "   [ %d ] Normal class resist", 0 },
		{ "   [ %d ] Boss class resist", 0 },
		{ "   [ %d ] Guardian class resist", 0 },
		{ "   [ %d ] Battlefield class resist", 0 }, // 7

		{ "   [ %d ] Formless race resist", 0 },
		{ "   [ %d ] Undead race resist", 0 },
		{ "   [ %d ] Brute race resist", 0 },
		{ "   [ %d ] Plant race resist", 0 },
		{ "   [ %d ] Insect race resist", 0 },
		{ "   [ %d ] Fish race resist", 0 },
		{ "   [ %d ] Demon race resist", 0 },
		{ "   [ %d ] Demi-Human race resist", 0 },
		{ "   [ %d ] Angel race resist", 0 },
		{ "   [ %d ] Dragon race resist", 0 },
		{ "   [ %d ] Player race resist", 0 }, // 18

		{ "   [ %d ] Resistance from Goblin", 0 },
		{ "   [ %d ] Resistance from Kobold", 0 },
		{ "   [ %d ] Resistance from Orc", 0 },
		{ "   [ %d ] Resistance from Golem", 0 },
		{ "   [ %d ] Resistance from Guardian", 0 },
		{ "   [ %d ] Resistance from Ninja", 0 },
		{ "   [ %d ] Resistance from GVG", 0 },
		{ "   [ %d ] Resistance from Battlefield", 0 },
		{ "   [ %d ] Resistance from Treasure", 0 },
		{ "   [ %d ] Resistance from Biolab", 0 },
		{ "   [ %d ] Resistance from Manuk", 0 },
		{ "   [ %d ] Resistance from Splendide", 0 },
		{ "   [ %d ] Resistance from Scaraba", 0 }, // 31

		{ "   [ %d ] Neutral element resist", 0 },
		{ "   [ %d ] Water element resist", 0 },
		{ "   [ %d ] Earth element resist", 0 },
		{ "   [ %d ] Fire element resist", 0 },
		{ "   [ %d ] Wind element resist", 0 },
		{ "   [ %d ] Poison element resist", 0 },
		{ "   [ %d ] Holy element resist", 0 },
		{ "   [ %d ] Dark element resist", 0 },
		{ "   [ %d ] Ghost element resist", 0 },
		{ "   [ %d ] Undead element resist", 0 }, // 41

		{ "   [ %d ] Neutral element defense", 0 },
		{ "   [ %d ] Water element defense", 0 },
		{ "   [ %d ] Earth element defense", 0 },
		{ "   [ %d ] Fire element defense", 0 },
		{ "   [ %d ] Wind element defense", 0 },
		{ "   [ %d ] Poison element defense", 0 },
		{ "   [ %d ] Holy element defense", 0 },
		{ "   [ %d ] Dark element defense", 0 },
		{ "   [ %d ] Ghost element defense", 0 },
		{ "   [ %d ] Undead element defense", 0 }, // 51

		{ "   [ %d ] Long range damage resist", 0 },
		{ "   [ %d ] Melee damage resist", 0 }, // 53
		{ "   [ %d ] Weapon immunity", 0 },
		{ "   [ %d ] Magic immunity", 0 }, 
		{ "   [ %d ] Misc immunity", 0 }, // 56
		{ NULL, 0 }
	};

	memset(resis_state, '\0', sizeof(resis_state));
	memset(info, '\0', sizeof(info));

		
		info_table[0].format = resis_state;

		for (j = 0; j <= 2; j++)
			info_table[j+1].value = sd->indexed_bonus.subsize[SZ_ALL] + sd->indexed_bonus.subsize[j];

		for (j = 0; j <= 3; j++)
			info_table[j+4].value = sd->indexed_bonus.subclass[CLASS_ALL] + sd->indexed_bonus.subclass[j];

		for (j = 0; j <= 10; j++)
			info_table[j+8].value = sd->indexed_bonus.subrace[RC_ALL] + sd->indexed_bonus.subrace[j];

		for (j = 0; j <= 13; j++)
			info_table[j+19].value = sd->indexed_bonus.subrace2[j];

		for (j = 0; j <= 9; j++){
			info_table[j+32].value = (((
				sd->indexed_bonus.subele[ELE_ALL] +
				sd->indexed_bonus.subele_script[ELE_ALL] ) +
				sd->indexed_bonus.subele[j] ) +
				sd->indexed_bonus.subele_script[j] );
		}

		for (j = 0; j <= 9; j++)
			info_table[j+42].value = sd->indexed_bonus.subdefele[ELE_ALL] + sd->indexed_bonus.subdefele[j];

		info_table[52].value = sd->bonus.long_attack_def_rate;
		info_table[53].value = sd->bonus.near_attack_def_rate;
		info_table[54].value = sd->special_state.no_weapon_damage;
		info_table[55].value = sd->special_state.no_magic_damage;
		info_table[56].value = sd->special_state.no_misc_damage;

	for (j = 1; info_table[j].format != NULL; j++){
		if( info_table[j].value != 0 )
			count =+ 1;
	}

	if( count == 0 ){
		sprintf(resis_state, "You don't have any resistance.");
		clif_messagecolor(&sd->bl, color_table[COLOR_RED], resis_state, false, SELF);
	} else {
		sprintf(resis_state, "======== Resistance ========");
		clif_messagecolor(&sd->bl, color_table[COLOR_WHITE], resis_state, false, SELF);
	}

	for (j = 1; info_table[j].format != NULL; j++) {
		sprintf(info, info_table[j].format, info_table[j].value);
		if (info_table[j].value > 0 && info_table[j].value <= 70)
			clif_messagecolor(&sd->bl, color_table[COLOR_CYAN], info, false, SELF);
		else if (info_table[j].value > 70)
			clif_messagecolor(&sd->bl, color_table[COLOR_YELLOW], info, false, SELF);
		else  if (info_table[j].value < 0)
			clif_messagecolor(&sd->bl, color_table[COLOR_RED], info, false, SELF);
	}
	return 0;
}
