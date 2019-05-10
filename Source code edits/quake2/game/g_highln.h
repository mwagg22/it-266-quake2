//::Pridkett

/*
* Highlander Quake 2
* Main Header File
* Author(s): Pridkett (pridkett@null.net)
*
* Last Revision:   1/1/98 - Wow...the sword actually works now
*                  12/29/97 - Initial Code
*/


/*
* sword variable definitions, I have this here because the sword is most likely still unbalanced
* I know this because I killed a super tank with it without taking ANY damaga
*/
#define SWORD_NORMAL_DAMAGE 100
#define SWORD_DEATHMATCH_DAMAGE 150
#define SWORD_KICK 500
#define SWORD_RANGE 100
/*
=============
fire_sword
attacks with the beloved sword of the highlander
(or use the blaster as a bludgeon)

edict_t *self - entity producing it, yourself
vec3_t start - The place you are
vec3_t aimdir - Where you are looking at in this case
int damage - the damage the sword inflicts
int kick - how much you want that bitch to be thrown back
Modified by DanE on 1-13-98
=============
*/

void fire_sword(edict_t *self, vec3_t start, vec3_t aimdir, int damage, int kick)
{
	trace_t tr; //detect whats in front of you up to range "vec3_t end"

	vec3_t end;

	// Figure out what we hit, if anything:

	VectorMA(start, SWORD_RANGE, aimdir, end);  //calculates the range vector                      

	tr = gi.trace(self->s.origin, NULL, NULL, end, self, MASK_SHOT);
	// figuers out what in front of the player up till "end"

	// Figure out what to do about what we hit, if anything
	int random_sound = (rand()%2);
	if (random_sound > 1){
		gi.sound(self, CHAN_AUTO, gi.soundindex("../players/male/whip.wav"), 1, ATTN_NORM, 0); }
	if (!((tr.surface) && (tr.surface->flags & SURF_SKY)))
	{
		if (tr.fraction < 1.0)
		{
			if (tr.ent->takedamage)
			{
				//This tells us to damage the thing that in our path...hehe
				T_Damage(tr.ent, self, self, aimdir, tr.endpos, tr.plane.normal, damage, kick, 0,0);
				gi.sound(self, CHAN_AUTO, gi.soundindex("misc/fhit3.wav"), 1, ATTN_NORM, 0);

			}
			else
			{
				gi.WriteByte(svc_temp_entity);
				gi.WriteByte(TE_SPARKS);
				gi.WritePosition(tr.endpos);
				gi.WriteDir(tr.plane.normal);
				gi.multicast(tr.endpos, MULTICAST_PVS);

				gi.sound(self, CHAN_AUTO, gi.soundindex("weapons/grenlb1b.wav"), 1, ATTN_NORM, 0);

			}
		}
	}
	return;
}  // 1-13-98 DanE

void sword_attack(edict_t *ent, vec3_t g_offset, int damage)
{
	vec3_t  forward, right;
	vec3_t  start;
	vec3_t  offset;

	if (is_quad)
		damage *= 4;
	AngleVectors(ent->client->v_angle, forward, right, NULL);
	VectorSet(offset, 24, 8, ent->viewheight - 8);
	VectorAdd(offset, g_offset, offset);
	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);

	VectorScale(forward, -2, ent->client->kick_origin);
	ent->client->kick_angles[0] = -1;

	fire_sword(ent, start, forward, damage, SWORD_KICK);
}

void Weapon_Sword_Fire(edict_t *ent)
{
	int damage;
	if (deathmatch->value)
		damage = SWORD_DEATHMATCH_DAMAGE;
	else
		damage = SWORD_NORMAL_DAMAGE;
	int random_sound = (rand() % 2);
	
		gi.sound(ent, CHAN_VOICE, gi.soundindex("../players/male/diemon.wav"), 1, ATTN_NORM, 0);
	
	sword_attack(ent, vec3_origin, damage);
	
	ent->client->ps.gunframe++;
}

void Weapon_Sword(edict_t *ent)
{
	static int      pause_frames[] = { 19, 32, 0 };
	static int      fire_frames[] = { 5, 0 };

	Weapon_Generic(ent, 4, 11, 52, 55, pause_frames, fire_frames, Weapon_Sword_Fire);
}

//
//!Pridkett