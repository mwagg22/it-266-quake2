#include "g_local.h"
#include "throwup.h"
void magic_attack(edict_t *ent)
{
	int		index;
	gitem_t *item = FindItem("Hearts");
	gi.bprintf(PRINT_MEDIUM, "Hmmmmff hmmmf hhhuuuuuuurrrrrllll.\n");
	if (item)
	{
		index = ITEM_INDEX(item);
	}
	if (ent->client->pers.weapon->pickup_name == "Vail")
	{
		if (ent->client->pers.inventory[index] > 10)
		{
			gi.bprintf(PRINT_MEDIUM, "Inside.\n");
			ent->client->pers.inventory[index] -= 10;
			vec3_t	offset;
			vec3_t	forward, right;
			vec3_t	start;
			int		damage = 120;
			float	radius;

			radius = damage + 40;

			VectorSet(offset, 8, 8, ent->viewheight - 8);
			AngleVectors(ent->client->v_angle, forward, right, NULL);
			P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);

			VectorScale(forward, -2, ent->client->kick_origin);
			ent->client->kick_angles[0] = -1;
			fire_vail(ent, start, forward, damage, 600, 1.25, radius);

		}
	}
}