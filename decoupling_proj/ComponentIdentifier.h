#ifndef COMPONENTIDENTIFIER_H
#define COMPONENTIDENTIFIER_H


namespace ComponentIdentifier{
	enum ID{
		EmptyComponent = 0,
		TransformableComponent = 1 << 0,
		VelocityComponent = 1 << 1,
		SpriteComponent = 1 << 2,
		HealthComponent = 1 << 3,
		EntityParentComponent = 1 << 4,
		BoxCollisionComponent = 1 << 5,
		CircleCollisionComponent = 1 << 6,
		RotatedBoxCollisionComponent = 1 << 7,
		AutomaticPathComponent = 1 << 8,
		CategoryComponent = 1 << 9,
		AnimationComponent = 1 << 10,
		ScriptUpdateComponent = 1 << 11,
		VertexNodeComponent = 1 << 12,
		StaminaComponent = 1 << 13,
		MeleeRectComponent = 1 << 14,
		ScriptAIComponent = 1 << 15,
		LightPointComponent = 1 << 16,
		CircularPathComponent = 1 << 17,
		AvoidanceBoxComponent = 1 << 18,
		AttackModeComponent = 1 << 19,
		TextDisplayComponent = 1 << 20,
		HarmfulBoxesComponent = 1 << 21,
		SpiritGrabberComponent = 1 << 22,
		SpritFormComponent = 1 << 23,
		DestroyableComponent = 1 << 24,
		TimerComponent = 1 << 25,
		CollisionComponent = BoxCollisionComponent | CircleCollisionComponent | RotatedBoxCollisionComponent,
		DisplayComponent = SpriteComponent | TextDisplayComponent,
		ComponentCounts
	};
}

#endif