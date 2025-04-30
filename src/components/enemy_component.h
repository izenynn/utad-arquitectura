#ifndef ENEMY_COMPONENT_H_
#define ENEMY_COMPONENT_H_

enum class EnemyType {
	BouncingBall,
	HexagonBall
};

struct EnemyComponent {
	EnemyType type{EnemyType::BouncingBall};
	int tier{1}; // Used to determine size/split logic
};

#endif // ENEMY_COMPONENT_H_
