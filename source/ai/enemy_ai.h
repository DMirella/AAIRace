#ifndef AAIRACE_SOURCE_ENEMY_AI_H_
#define AAIRACE_SOURCE_ENEMY_AI_H_

#include <memory>
#include <sstream>
#include <vector>

#include "ai.h"
#include "ai_types.h"

class EnemyAI {
 public:
  EnemyAI() = delete;
  EnemyAI(const EnemyAI& enemy_ai) = delete;
  EnemyAI(EnemyAI&& enemy_ai) = delete;
  EnemyAI& operator=(const EnemyAI& enemy_ai) = delete;
  EnemyAI& operator=(EnemyAI&& enemy_ai) = delete;

  EnemyAI(const std::shared_ptr<std::vector<AIIOData>>& collected_aiio_data);

  void TrainWithData(const std::vector<AIIOData>& aiio_data);
  AIOutputData GetOutputData(const AIInputData& input_data);
 private:
  NeuralNetwork neural_network_;
  int last_applied_action_index_;
  std::shared_ptr<std::vector<AIIOData>> collected_aiio_data_;
};

#endif  // AAIRACE_SOURCE_ENEMY_AI_H_