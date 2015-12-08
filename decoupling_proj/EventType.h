#ifndef EVENTTYPE_H
#define EVENTTYPE_H

namespace EventType{
	enum ID{
		None,
		PlayerHPChanged,
		PlayerCollidedWithTile,
		PlayerStaminaChanged,
		FireSpiritCollided,
		CreateNewEntity,
		SoulHookStruck,
		SpiritCoreChanged,
		EntityGotDamaged,
		EventTypeCount
	};
};

#endif