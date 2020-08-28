#ifndef AAIRACE_SOURCE_ENEMY_AI_H_
#define AAIRACE_SOURCE_ENEMY_AI_H_

#include <memory>
#include <sstream>
#include <vector>

#include "ai.h"
#include "ai_types.h"

std::vector<AIIOData> FilterAIIOData(const std::vector<AIIOData>& aiio_data);

class EnemyAI {
 public:
  EnemyAI(const std::shared_ptr<std::vector<AIIOData>>& collected_aiio_data);

  void TrainWithData(const std::vector<AIIOData>& aiio_data);
  AIOutputData GetOutputData(const AIInputData& input_data) const;
  std::stringstream ToStringStream() const;
 private:
  NeuralNetwork neural_network_;
  std::shared_ptr<std::vector<AIIOData>> collected_aiio_data_;
};

#endif  // AAIRACE_SOURCE_ENEMY_AI_H_