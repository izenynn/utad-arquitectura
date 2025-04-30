#ifndef ENEMY_COMPONENT_H_
#define ENEMY_COMPONENT_H_

enum class EnemyType {
	BouncingBall,
	HexagonBall
};

struct EnemyComponent {
	EnemyType type;
	int tier;

	EnemyComponent(
		EnemyType type = EnemyType::BouncingBall,
		int tier = 1)
		: type(type), tier(tier) {}
};

#endif // ENEMY_COMPONENT_H_
